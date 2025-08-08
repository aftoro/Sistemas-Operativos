#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

// Incluimos la definición del tipo Proceso desde el archivo proceso.h,
// para poder usarlo en la declaración del planificador
#include "proceso.h"

// Declaración de la función principal del planificador Round Robin.
// Esta función se encargará de repartir el tiempo de CPU entre los procesos,
// respetando su quantum y cambiando de contexto cuando sea necesario.
void planificadorRoundRobin(Proceso procesos[], int cantidad);

#endif
