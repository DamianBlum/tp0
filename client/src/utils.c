#include "utils.h"


void* serializar_paquete(t_paquete* paquete, int bytes) //llega con 14 bytes
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	printf("DESPLAZAMIENTO 1: %d\n", desplazamiento);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	printf("DESPLAZAMIENTO 2: %d\n", desplazamiento);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;
	printf("DESPLAZAMIENTO FINAL: %d\n", desplazamiento);

	return magic;
}

int crear_conexion(char *ip, char* puerto,t_log* logger)
{
	int socket_cliente;
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	socket_cliente = getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int fd_conexion = socket(server_info->ai_family,
                         server_info->ai_socktype,
                         server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	socket_cliente = connect(fd_conexion, server_info->ai_addr, server_info->ai_addrlen);
	if (socket_cliente== -1)
	{
		log_error(logger, "Cliente: Fallo la conexion, se cierra el cliente.");
		return -1;
	}
	log_info(logger, "Cliente: Pude establecer conexion con el servidor");
	freeaddrinfo(server_info);
	// handshake
	uint32_t handshake = 1;
	uint32_t result;
	log_info(logger, "Cliente: Tratando de hacer el handshake");
	send(fd_conexion, &handshake, sizeof(uint32_t), NULL);
	recv(fd_conexion, &result, sizeof(uint32_t), MSG_WAITALL);
	if (result == -1)
	{
		log_error(logger, "Cliente: error al hacer el handshake");
		return -1;
	}

	log_info(logger, "Cliente: handshake exitoso");


	return fd_conexion;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	printf("tamanio del size %d\n",paquete->buffer->size);
	printf("tamanio de un int %d\n", sizeof(int));
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);
	printf("SIZE BYTES: %d\n", bytes);
	void* a_enviar = serializar_paquete(paquete, bytes);

	int c=send(socket_cliente, a_enviar, bytes, 0);
	printf("BYTES ENVIADOS: %d\n", c);
	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);
	printf("BUFFER ANTES DE ENVIAR: %d\n", paquete->buffer->size);
	printf("BYTES ANTES DE ENVIAR: %d\n", bytes);
	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
