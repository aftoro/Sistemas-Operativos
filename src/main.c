#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "proceso.h"
#include "planificador.h"
#include "instrucciones.h"

#define MAX_PROCESOS 100
#define LINEA_MAX 100

// Intenta parsear una línea de texto con la información del proceso
int parseLinea(const char* linea, Proceso* p) {
    // Intentar formato con etiquetas (ej. "PID: 1, AX=2, BX=3, CX=1, Quantum=3")
    if (sscanf(linea, "PID: %d, AX=%d, BX=%d, CX=%d, Quantum=%d",
               &p->pid, &p->ax, &p->bx, &p->cx, &p->quantum) == 5) {
        return 1;
    }

    // Intentar formato de solo números (ej. "1 2 3 1 3")
    if (sscanf(linea, "%d %d %d %d %d",
               &p->pid, &p->ax, &p->bx, &p->cx, &p->quantum) == 5) {
        return 1;
    }

    return 0; // ninguno válido
}

// Cargar procesos desde archivo
int cargarDesdeArchivo(const char* nombre, Proceso procesos[], int* cantidad) {
    FILE* archivo = fopen(nombre, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return 0;
    }

    char linea[LINEA_MAX];
    int i = 0;

    while (fgets(linea, sizeof(linea), archivo) && i < MAX_PROCESOS) {
        // Inicializar registros en 0 por si no están presentes
        procesos[i].ax = 0;
        procesos[i].bx = 0;
        procesos[i].cx = 0;

        // Extraer PID buscando la cadena "PID:"
        char* ptr = strstr(linea, "PID:");
        if (!ptr) continue;
        sscanf(ptr, "PID: %d", &procesos[i].pid);

        // Buscar registros y quantum en la línea
        ptr = strchr(linea, ',');
        while (ptr) {
            if (strncmp(ptr, ", AX=", 5) == 0) {
                sscanf(ptr, ", AX=%d", &procesos[i].ax);
            } else if (strncmp(ptr, ", BX=", 5) == 0) {
                sscanf(ptr, ", BX=%d", &procesos[i].bx);
            } else if (strncmp(ptr, ", CX=", 5) == 0) {
                sscanf(ptr, ", CX=%d", &procesos[i].cx);
            } else if (strncmp(ptr, ", Quantum=", 10) == 0) {
                sscanf(ptr, ", Quantum=%d", &procesos[i].quantum);
                break; // Ya tomamos el quantum, terminamos
            }

            ptr = strchr(ptr + 1, ',');
        }

        // Inicializamos el PC y el estado
        procesos[i].pc = 0;
        strcpy(procesos[i].estado, "Listo");
        i++;
    }

    fclose(archivo);
    *cantidad = i;
    return 1;
}

// Cargar procesos desde la entrada por consola
int cargarDesdeConsola(Proceso procesos[], int* cantidad) {
    int i = 0;
    char linea[LINEA_MAX];

    printf("Ingresa procesos\n");
    printf("Escribe 'fin' para terminar.\n");

    while (i < MAX_PROCESOS) {
        printf(">> ");
        fgets(linea, sizeof(linea), stdin);

        if (strncmp(linea, "fin", 3) == 0)
            break;

        Proceso temp;
        if (parseLinea(linea, &temp)) {
            temp.pc = 0;
            strcpy(temp.estado, "Listo");
            procesos[i++] = temp;
        } else {
            printf("Formato inválido. Intenta de nuevo.\n");
        }
    }

    *cantidad = i;
    return 1;
}

// Imprimir todos los procesos cargados para revisión
void imprimirProcesos(Proceso procesos[], int cantidad) {
    printf("\nProcesos cargados:\n");
    for (int i = 0; i < cantidad; i++) {
        printf("PID: %d, AX=%d, BX=%d, CX=%d, Quantum=%d, Estado=%s\n",
               procesos[i].pid,
               procesos[i].ax,
               procesos[i].bx,
               procesos[i].cx,
               procesos[i].quantum,
               procesos[i].estado);
    }
}

// Función principal del simulador
int main(int argc, char* argv[]) {
    Proceso procesos[MAX_PROCESOS];
    int cantidad = 0;

    // Modo archivo: si se pasa un argumento al ejecutar
    if (argc == 2) {
        if (!cargarDesdeArchivo(argv[1], procesos, &cantidad)) {
            printf("No se pudieron cargar procesos desde el archivo.\n");
            return 1;
        }
    } else {
        // Modo manual: entrada desde consola
        cargarDesdeConsola(procesos, &cantidad);
    }
    
    // Cargar instrucciones individuales desde archivos como "1.txt", "2.txt", etc.
    for (int i = 0; i < cantidad; i++) {
        if (!cargarInstrucciones(&procesos[i])) {
            printf("Error al cargar instrucciones del proceso %d\n", procesos[i].pid);
            return 1;
        }
    }

    imprimirProcesos(procesos, cantidad);
    planificadorRoundRobin(procesos, cantidad);  // Ejecutar el planificador
    return 0;
}

