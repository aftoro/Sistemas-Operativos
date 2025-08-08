#include <stdio.h>
#include <string.h>
#include "proceso.h"
#include "instrucciones.h"

// Planificador Round-Robin con detección inmediata de procesos finalizados
void planificadorRoundRobin(Proceso procesos[], int cantidad) {
    int todosFinalizados = 0;
    int anterior = -1;  // Índice del proceso anterior

    while (!todosFinalizados) {
        todosFinalizados = 1;

        for (int i = 0; i < cantidad; i++) {
            Proceso* p = &procesos[i];

            // Solo ejecutamos si el proceso aún no ha finalizado
            if (strcmp(p->estado, "Finalizado") != 0) {
                todosFinalizados = 0;

                printf("\n[Cambio de contexto]\n");

                // Guardar estado del proceso anterior (si hay)
                if (anterior != -1 && strcmp(procesos[anterior].estado, "Finalizado") != 0) {
                    printf("Guardando estado de Proceso %d: PC=%d, AX=%d, BX=%d, CX=%d\n",
                           procesos[anterior].pid,
                           procesos[anterior].pc,
                           procesos[anterior].ax,
                           procesos[anterior].bx,
                           procesos[anterior].cx);
                }

                // Cargar estado del proceso actual
                printf("Cargando estado de Proceso %d: PC=%d, AX=%d, BX=%d, CX=%d\n",
                       p->pid, p->pc, p->ax, p->bx, p->cx);

                anterior = i;

                int instruccionesEjecutadas = 0;

                while (instruccionesEjecutadas < p->quantum) {
                    // Verificación inicial
                    if (p->pc >= p->numInstrucciones) {
                        printf("Proceso %d ha terminado sus instrucciones.\n", p->pid);
                        strcpy(p->estado, "Finalizado");
                        break;
                    }

                    printf("Proceso %d ejecutando instrucción %d: %s\n",
                           p->pid, p->pc, p->instrucciones[p->pc]);

                    int salto = ejecutarInstruccion(p);

                    printf("Valores después de la instrucción: AX=%d, BX=%d, CX=%d\n",
                           p->ax, p->bx, p->cx);

                    if (!salto) {
                        p->pc++;
                    }

                    // Verificación adicional después de avanzar el PC
                    if (p->pc >= p->numInstrucciones) {
                        printf("Proceso %d ha terminado sus instrucciones.\n", p->pid);
                        strcpy(p->estado, "Finalizado");
                        break;
                    }

                    instruccionesEjecutadas++;
                }
            }
        }
    }

    printf("\nTodos los procesos han finalizado.\n");
}
