#ifndef PROCESO_H
#define PROCESO_H

// Define límites para el número de instrucciones y longitud de línea
#define MAX_INSTRUCCIONES 100  // Máximo número de instrucciones que puede tener un proceso
#define LINEA_MAX 100          // Tamaño máximo de cada línea/instrucción

// Estructura que representa un proceso en el sistema
typedef struct {
    int pid;             // Identificador único del proceso (Process ID)
    int pc;              // Program Counter simulado: indica qué instrucción se ejecutará
    int ax, bx, cx;      // Registros simulados del CPU. Guardan datos internos del proceso.
    int quantum;         // Cuánto tiempo (ciclos de CPU) puede ejecutarse antes de ser interrumpido
    char estado[10];     // Estado del proceso: "Listo", "Ejecutando", o "Finalizado"   
    char **instrucciones;    // Arreglo dinámico de cadenas con las instrucciones del proceso
    int numInstrucciones;    // Número total de instrucciones cargadas desde el archivo
} Proceso;

#endif
