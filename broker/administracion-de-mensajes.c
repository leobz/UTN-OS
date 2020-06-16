#include "administracion-de-mensajes.h"

t_adm_mensaje*iniciar_administracion(t_mensaje*mensaje){

	t_adm_mensaje *administrador=malloc(sizeof(administrador));
		administrador->id_mensaje= mensaje->id_mensaje;
		administrador->id_correlativo=mensaje->id_correlativo;
		administrador->codigo_operacion=mensaje->codigo_operacion;
		administrador->suscriptores_confirmados=list_create();
		administrador->suscriptores_enviados=list_create();

		meter_en_diccionario(administracion_por_id,mensaje->id_mensaje,administrador);

		list_add(administradores[mensaje->codigo_operacion],administrador);

		return administrador;
}

void *generar_mensaje(t_adm_mensaje* actual_administrator, int*bytes){

	void* payload; //para el mensaje en cache
	int payload_size = 0; //para el tamaño del mensaje en cache

	if (es_particion_dinamica()){ //aplicar funcion es_particion_dinamica
		payload = leer_particion_dinamica(actual_administrator->particion_dinamica);
		payload_size = actual_administrator->particion_dinamica->tamanio_particion;
	}

	if(es_buddy_system()){ //aplicar funcion es_buddy_system
		payload = leer_particion_bs(actual_administrator->particion_bs);
		payload_size = actual_administrator->particion_bs->size_mensaje;
	}

	int offset = 0;
	*bytes = sizeof(int)*4 + payload_size;
	void* mensaje_para_enviar = malloc(*bytes);

	memcpy(mensaje_para_enviar,&actual_administrator->codigo_operacion,sizeof(int));
		offset=sizeof(int);
	memcpy(mensaje_para_enviar+offset,&actual_administrator->id_mensaje,sizeof(int));
		offset=sizeof(int);
	memcpy(mensaje_para_enviar+offset,&actual_administrator->id_correlativo,sizeof(int));
		offset=sizeof(int);
	memcpy(mensaje_para_enviar+offset,&payload_size,sizeof(int));
		offset=sizeof(int);
	memcpy(mensaje_para_enviar+offset,payload,payload_size);

	return mensaje_para_enviar;
}

void agregar_mensaje_memoria_cache(t_adm_mensaje* actual_administrator, t_mensaje* mensaje) {
	if (es_particion_dinamica()){
		actual_administrator->particion_dinamica = agregar_mensaje_memoria_cache_particion_dinamica(mensaje);
	}
	else if(es_buddy_system()){
		actual_administrator->particion_bs = agregar_mensaje_memoria_cache_bs(mensaje);
	}
}
