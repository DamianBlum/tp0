#include "client.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola! Soy un log"); 

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor=config_get_string_value(config,"CLAVE");
	ip=config_get_string_value(config,"IP");
	puerto=config_get_string_value(config,"PUERTO");
	// Loggeamos el valor de config
	log_info(logger,"Clave: %s IP: %s Puerto: %s", valor,ip,puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	//conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	//paquete(conexion);

	//terminar_programa(conexion, logger, config);*/

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger=log_create("tp0.log","/log",1,LOG_LEVEL_INFO);
	if (nuevo_logger==NULL) {
		printf("No se pudo hacer el logger");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if (nuevo_config==NULL)
	{
		printf("No se pudo hacer el config");
		exit(1);
	}
	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char *linea;
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (1) {
		linea = readline(">");
		if (!linea) {
			break;
		}
		log_info(logger,"Linea: %s", linea);
		free(linea);
    }

	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);
	config_destroy(config);
}
