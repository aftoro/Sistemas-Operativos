#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

// Incluye la estructura Proceso para poder trabajar con sus campos
#include "proceso.h"

// Función que ejecuta una sola instrucción del proceso apuntado por 'p'.
// Esta función simula la ejecución de instrucciones como ADD, SUB, JMP, etc.
// Equivalente a cuando la CPU ejecuta instrucciones de un programa en tiempo real.
int ejecutarInstruccion(Proceso* p);

// Función que carga las instrucciones desde un archivo de texto (por ejemplo: "1.txt")
// al campo 'instrucciones' del proceso. Simula cargar el "código ejecutable" de un proceso.
// Es como si el sistema operativo leyera el programa desde disco y lo cargara en memoria.
int cargarInstrucciones(Proceso *p);

#endif
