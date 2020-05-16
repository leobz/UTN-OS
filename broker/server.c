/*
 * server.c
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */


#include"colas.h"

t_log* logger;
t_broker_config* broker_config;

void procesar_mensaje_recibido(t_paquete_socket* paquete) {

	inicializar_broker(&broker_config,&logger);

	log_info(logger, "[NUEVA CONEXION]: Numero de socket: %d", paquete->socket_cliente);


	if((paquete->codigo_operacion>=0)&&(paquete->codigo_operacion<=2)){

	t_mensaje_sc* mensaje_a_encolar_sc;
	t_mensaje_sc* mensaje_a_enviar_sc;

		switch(paquete->codigo_operacion) {
		case NEW_POKEMON:

			log_info(logger,"Mensaje recibido con codigo_de_mensaje:  %d",paquete->codigo_operacion);


			mensaje_a_encolar_sc= preparar_mensaje_sc(paquete);

			insertar_mensaje_sc(mensaje_a_encolar_sc,NEW_POKEMON);
			mensaje_a_enviar_sc= extraer_mensaje_sc(NEW_POKEMON);

			log_info(logger,"Recibiendo de cola:  %d",mensaje_a_enviar_sc->codigo_operacion);
			break;


		case GET_POKEMON:


			mensaje_a_encolar_sc= preparar_mensaje_sc(paquete);
			insertar_mensaje_sc(mensaje_a_encolar_sc,GET_POKEMON);
			mensaje_a_enviar_sc= extraer_mensaje_sc(GET_POKEMON);
			log_info(logger,"Recibiendo de cola:  %d",paquete->codigo_operacion);

			break;

			//case CATCH_POKEMON:

			//	mensaje_a_encolar_sc= preparar_mensaje_sc(paquete,mensaje_a_preparar_sc);
			/*insertar_mensaje_sc(mensaje_a_encolar_sc,CATCH_POKEMON);
			mensaje_a_enviar= extraer_mensaje(CATCH_POKEMON);*/

			//break;
		default:
			pthread_exit(NULL);
		break;
	}

		free(mensaje_a_encolar_sc->payload); //Hacer solo despues de guardar el mensaje en la cache y en la cola auxiliar
		free(mensaje_a_encolar_sc);
		liberar_paquete(paquete);

}

	if((paquete->codigo_operacion>=3)&&(paquete->codigo_operacion<=5)){

		t_mensaje_cc* mensaje_a_encolar_cc;
		t_mensaje_cc* mensaje_a_enviar_cc;

				switch(paquete->codigo_operacion){

			case APPEARED_POKEMON:


			mensaje_a_encolar_cc= preparar_mensaje_cc(paquete);
			insertar_mensaje_cc(mensaje_a_encolar_cc,APPEARED_POKEMON);
			mensaje_a_enviar_cc= extraer_mensaje_cc(APPEARED_POKEMON);
			log_info(logger,"Mensaje recibido con codigo_de_mensaje:  %d", mensaje_a_enviar_cc->codigo_operacion);
			break;

			//case LOCALIZED_POKEMON:

			//mensaje_a_encolar_cc= preparar_mensaje_cc(paquete,mensaje_a_preparar_cc);
			/*insertar_mensaje_sc(mensaje_a_encolar_cc,LOCALIZED_POKEMON);
			mensaje_a_enviar_cc= extraer_mensaje_cc(LOCALIZED_POKEMON);*/

			//break;

			//case CAUGHT_POKEMON:

			//mensaje_a_encolar_cc= preparar_mensaje_cc(paquete,mensaje_a_preparar_cc);
			/*insertar_mensaje_cc(mensaje_a_encolar_cc,CATCH_POKEMON);
			mensaje_a_enviar_cc= extraer_mensaje_cc(CATCH_POKEMON);*/

			//break;
			default:
					pthread_exit(NULL);

			break;
	}

		free(mensaje_a_encolar_cc->payload); //Hacer solo despues de guardar el mensaje en la cache y en la cola auxiliar
		free(mensaje_a_encolar_cc);
		liberar_paquete(paquete);
}


	else{

		switch(paquete->codigo_operacion){
			case SUSCRIPCION:
				log_info(logger,"Mensaje recibido con cola a suscribir:  %d",paquete->cola);
				//encolar_proceso(paquete->socket_cliente,paquete->cola);

				break;

			case OP_ERROR:

				printf("Error al recibir mensaje del socket: %d\n",paquete->socket_cliente);
				pthread_exit(NULL);
				break;

			default:
				pthread_exit(NULL);
				break;
		}
		free(paquete);
	}
	finalizar_broker(broker_config,logger);
}


t_mensaje_sc* preparar_mensaje_sc(t_paquete_socket* paquete){ //esta funcion pasa el paquete a una mensaje encolable

				t_mensaje_sc* mensaje_a_preparar = malloc(sizeof(t_mensaje_sc));

				id_cola[paquete->codigo_operacion]++;

				mensaje_a_preparar->codigo_operacion= paquete->codigo_operacion;
				mensaje_a_preparar->id_mensaje=id_cola[paquete->codigo_operacion];
				mensaje_a_preparar->payload_size= paquete->buffer->size;
				mensaje_a_preparar->payload = malloc(paquete->buffer->size);
				memcpy(mensaje_a_preparar->payload, paquete->buffer->stream,mensaje_a_preparar->payload_size);
				mensaje_a_preparar->siguiente=NULL;



	return mensaje_a_preparar;
}

t_mensaje_cc* preparar_mensaje_cc(t_paquete_socket* paquete){

				t_mensaje_cc* mensaje_a_preparar = malloc(sizeof(t_mensaje_cc));

				id_cola[paquete->codigo_operacion]++;

				mensaje_a_preparar->codigo_operacion= paquete->codigo_operacion;
				mensaje_a_preparar->id_mensaje=id_cola[paquete->codigo_operacion];
				mensaje_a_preparar->id_correlativo=6; //solo de prueba
				mensaje_a_preparar->payload_size= paquete->buffer->size;
				mensaje_a_preparar->payload = malloc(paquete->buffer->size);
				memcpy(mensaje_a_preparar->payload, paquete->buffer->stream,mensaje_a_preparar->payload_size);
				mensaje_a_preparar->siguiente=NULL;

	return mensaje_a_preparar;
}

void liberar_paquete(t_paquete_socket* paquete){

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
}
