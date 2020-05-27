#include "test_appeared_pokemon.h"

void empaquetar_y_desempaquetar_appeared_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;

	t_paquete *paquete = crear_paquete(APPEARED_POKEMON,
			buffer_appeared_pokemon(pokemon, pos_x, pos_y), 0);

	t_mensaje_appeared* mensaje_appeared = get_mensaje_appeared_by_buffer(paquete->buffer);

	CU_ASSERT_EQUAL(mensaje_appeared->length_pokemon, strlen(pokemon)+1);
	CU_ASSERT_STRING_EQUAL(mensaje_appeared->pokemon, pokemon);
	CU_ASSERT_EQUAL(mensaje_appeared->posx, pos_x);
	CU_ASSERT_EQUAL(mensaje_appeared->posy, pos_y);

	eliminar_paquete(paquete);
	eliminar_mensaje_appeared(mensaje_appeared);
}

void serializar_y_deserializar_appeared_pokemon() {
	char* pokemon = "pikachu";
	int pos_x = 1;
	int pos_y = 2;
	int bytes;

	void* mensaje_serializado = serializar_appeared_pokemon(&bytes, pokemon,
			pos_x, pos_y, 0);

	test_deserializar_buffer_appeared_pokemon(mensaje_serializado, pokemon, pos_x, pos_y);

}

void test_deserializar_buffer_appeared_pokemon(void* mensaje_serializado, char* pokemon, int pos_x, int pos_y) {

	//POR HACER: TODO ESTO SE PUEDE ABSTRAER EN UNA FUNCION GENERICA QUE SIRVA PARA TESTEAR OTROS MENSAJES
	t_paquete* paquete = (t_paquete*) malloc(sizeof(t_paquete));
	paquete->buffer = (t_buffer*) malloc(sizeof(t_buffer));
	t_mensaje_appeared* mensaje_appeared;
	int size = 0;
	int offset = 0;


	memcpy(&(paquete->codigo_operacion), mensaje_serializado + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(paquete->id_correlativo), mensaje_serializado + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&(size), mensaje_serializado + offset, sizeof(int));
	offset += sizeof(int);

	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(size);

	memcpy(paquete->buffer->stream, mensaje_serializado + offset, paquete->buffer->size);


	// ESTA ES LA FUNCION QUE SE TESTEA
	mensaje_appeared = get_mensaje_appeared_by_buffer(paquete->buffer);

	CU_ASSERT_EQUAL(paquete->codigo_operacion, APPEARED_POKEMON);
	CU_ASSERT_EQUAL(mensaje_appeared->length_pokemon, strlen(pokemon)+1);
	CU_ASSERT_STRING_EQUAL(mensaje_appeared->pokemon, pokemon);
	CU_ASSERT_EQUAL(mensaje_appeared->posx, pos_x);
	CU_ASSERT_EQUAL(mensaje_appeared->posy, pos_y);

	eliminar_paquete(paquete);
	eliminar_mensaje_appeared(mensaje_appeared);
}

void agregar_tests_appeared_pokemon() {
	CU_pSuite suite_appeared_pokemon = CU_add_suite("Mensajes Appeared Pokemon",
			NULL, NULL);

	CU_add_test(suite_appeared_pokemon,
			"Empaquetar y desempaquetar Appeared Pokemon",
			empaquetar_y_desempaquetar_appeared_pokemon);

	CU_add_test(suite_appeared_pokemon,
			"Serializar y deserializar Appeared Pokemon",
			serializar_y_deserializar_appeared_pokemon);

}

