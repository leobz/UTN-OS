echo **** Parte 2 *****

echo
echo Cargando configuracion de la Cátedra
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.base team.config

echo
echo Levanto Team en segundo plano:
../team/Debug/team &
