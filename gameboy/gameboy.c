#include "gameboy.h"
#include "test/testing.h"

t_socket_config* gameboy_config;
t_log* logger;

int main(int argc, char ** argv) {
	inicializar_gameboy(&gameboy_config, &logger);

	if (argc == 2) {
		if (strcmp(argv[1], "test") == 0)
			correrTests();

	}
	if (strcmp(argv[1], "BROKER") == 0){

		if((strcmp(argv[2], "NEW_POKEMON") == 0) && argc==7){
			char* pokemon = argv[3];
			int pos_x= argv[4];
			int pos_y= argv[5];
			int cantidad= argv[6];

			int conexion = crear_conexion(gameboy_config->ip_broker, gameboy_config->puerto_broker);

			if (conexion == -1) {
								printf("ERROR: Conexion con [Broker] no estable1cida");
								exit(-1);
								}

			log_info(logger, "Conexion establecida con [Broker]");

			int bytes;
			void* a_enviar = serializar_new_pokemon(&bytes, pokemon, pos_x, pos_y);

			enviar_mensaje(conexion, a_enviar, bytes);

			log_info(logger, "Mensaje enviado a [Broker]: NEW_POKEMON %s %d %d %d", pokemon, pos_x, pos_y, cantidad);

			liberar_conexion(conexion);

		}




	}



		if (argc == 6) {
			if (strcmp(argv[1], "TEAM") == 0
					&& strcmp(argv[2], "APPEARED_POKEMON") == 0) {
				char *pokemon = argv[3];
				int pos_x = atoi(argv[4]);
				int pos_y = atoi(argv[5]);

				int conexion = crear_conexion(gameboy_config->ip_team,
						gameboy_config->puerto_team);

				if (conexion == -1) {
					printf("ERROR: Conexion con [Team] no estable1cida");
					exit(-1);
				}

				log_info(logger, "Conexion establecida con [Team]");

				int bytes;
				void* a_enviar = serializar_appeared_pokemon(&bytes, pokemon,
						pos_x, pos_y);

				enviar_mensaje(conexion, a_enviar, bytes);

				log_info(logger,"Mensaje enviado a [Team]: APPEARED_POKEMON %s %d %d",
						pokemon, pos_x, pos_y);

				liberar_conexion(conexion);

			}
		}



	finalizar_gameboy(gameboy_config, logger);
	return 0;
}

void inicializar_gameboy(t_socket_config **gameboy_config, t_log **logger) {
	*gameboy_config = cargar_gameboy_config("gameboy.config");
	*logger = iniciar_logger("gameboy.log", "gameboy", LOG_LEVEL_INFO);
}

void finalizar_gameboy(t_socket_config* gameboy_config, t_log* logger) {
	destruir_socket_config(gameboy_config);
	destruir_logger(logger);
}


t_socket_config *cargar_gameboy_config(char *path_archivo) {
	t_config *config;
	t_socket_config *gameboy_config;

	config = leer_config(path_archivo);
	gameboy_config = malloc(sizeof(t_socket_config));

	parsear_socket_config(gameboy_config, config);
	destruir_config(config);
	return gameboy_config;
}
