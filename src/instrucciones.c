#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "proceso.h"
#include "instrucciones.h"

//  Función auxiliar que recibe el nombre de un registro como texto ("AX", "BX", "CX")
// y devuelve un puntero al campo correspondiente del proceso.
// Esto permite modificar directamente el valor del registro.
int* obtenerRegistro(Proceso *p, const char *reg) {
    if (strcmp(reg, "AX") == 0) return &p->ax;
    if (strcmp(reg, "BX") == 0) return &p->bx;
    if (strcmp(reg, "CX") == 0) return &p->cx;
    return NULL;
}

//  Esta función simula la ejecución de una sola instrucción en el proceso.
// Lee la instrucción actual del proceso (según el contador de programa `pc`)
// y la interpreta como si fuera la CPU ejecutándola.
int ejecutarInstruccion(Proceso *p) {
    // Si ya no hay más instrucciones, no se ejecuta nada.
    if (p->pc >= p->numInstrucciones) return 0;

    // Toma la instrucción actual (por ejemplo, "ADD AX, 5")
    char *inst = p->instrucciones[p->pc];
    char op[10], arg1[10], arg2[10];

    // Divide la instrucción en partes: operador (op), argumento 1 y 2
    // Por ejemplo: "ADD AX, 5" → op="ADD", arg1="AX", arg2="5"
    int n = sscanf(inst, "%9s %9[^,], %9s", op, arg1, arg2);

    // Instrucción NOP: no hace nada, pero consume un ciclo de CPU.
    if (strcmp(op, "NOP") == 0) {
        return 0;
    }

    // Instrucciones tipo ADD, SUB, MUL (con 2 argumentos)
    if (n == 3) {
        int *reg1 = obtenerRegistro(p, arg1);  // Registro destino
        int valor;

        if (reg1 == NULL) return 0;

        // El segundo argumento puede ser otro registro o un número literal
        if (strcmp(arg2, "AX") == 0 || strcmp(arg2, "BX") == 0 || strcmp(arg2, "CX") == 0)
            valor = *obtenerRegistro(p, arg2);  // valor del segundo registro
        else
            valor = atoi(arg2);  // convierte cadena a entero

        // Simulación de instrucciones aritméticas
        if (strcmp(op, "ADD") == 0) *reg1 += valor;
        else if (strcmp(op, "SUB") == 0) *reg1 -= valor;
        else if (strcmp(op, "MUL") == 0) *reg1 *= valor;

        return 0;  // No hubo salto, se avanza a la siguiente instrucción
    } 
    
    // Instrucciones con solo un argumento: INC y JMP
    else if (n == 2) {
        if (strcmp(op, "INC") == 0) {
            int *reg = obtenerRegistro(p, arg1);
            if (reg) *reg += 1;
            return 0;
        } 
        // Salto incondicional: cambia manualmente el PC (contador de programa)
        else if (strcmp(op, "JMP") == 0) {
            int nuevaPC = atoi(arg1);  // A qué línea saltar
            if (nuevaPC >= 0 && nuevaPC < p->numInstrucciones) {
                p->pc = nuevaPC;
                return 1;  // Se hizo un salto manual
            }
        }
    }

    return 0;  // Si no reconoció nada, no hace nada
}

//  Esta función carga las instrucciones del archivo "PID.txt" al proceso.
// Por ejemplo: si el PID es 3, busca "3.txt". Simula cargar código ejecutable desde disco.
int cargarInstrucciones(Proceso *p) {
    char filename[50];
    sprintf(filename, "%d.txt", p->pid);  // construye el nombre del archivo

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return 0;
    }

    char linea[100];
    int count = 0;

    // Cuenta cuántas líneas (instrucciones) tiene el archivo
    while (fgets(linea, sizeof(linea), file)) {
        count++;
    }

    rewind(file);  // Vuelve al inicio del archivo para empezar a leerlo de nuevo

    // Reserva memoria para todas las instrucciones del proceso
    p->numInstrucciones = count;
    p->instrucciones = malloc(sizeof(char *) * count);
    if (!p->instrucciones) {
        printf("Error al asignar memoria para instrucciones\n");
        fclose(file);
        return 0;
    }

    int i = 0;
    // Guarda cada línea como una instrucción individual
    while (fgets(linea, sizeof(linea), file)) {
        linea[strcspn(linea, "\n")] = 0; // Elimina el salto de línea final

        if (strlen(linea) >= 100) {
            printf("Instrucción demasiado larga en %s\n", filename);
            fclose(file);
            return 0;
        }

        p->instrucciones[i] = strdup(linea);  // Copia la línea
        if (!p->instrucciones[i]) {
            printf("Error al copiar instrucción %d\n", i);
            fclose(file);
            return 0;
        }
        i++;
    }

    fclose(file);  // Cierra el archivo
    return 1;  // Carga exitosa
}

