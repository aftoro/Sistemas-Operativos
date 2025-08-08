# Simulador de Planificación Round-Robin

Este proyecto es un simulador de un planificador de procesos con cambio de contexto, basado en el algoritmo de planificación **Round-Robin**. Cada proceso se representa con registros simulados (AX, BX, CX), instrucciones básicas tipo ensamblador y un `quantum` configurable.

---

## Estructura del proyecto

```
simulador_procesos/
├── src/
    ├── main.c
    ├── planificador.c
    ├── instrucciones.c
├── include/
    ├── instrucciones.h
    ├── planificador.h
    ├── proceso.h
├── procesos.txt         // Archivo con los procesos
├── 1.txt                // Instrucciones del proceso 1
├── 2.txt                // Instrucciones del proceso 2
└── ...
```

---

## Compilación

Usa `gcc` para compilar todos los archivos fuente:

```bash
gcc src/main.c src/planificador.c src/instrucciones.c -Iinclude -o bin/simulador
```

Esto generará un ejecutable llamado `simulador`.

---

## Ejecución

Tienes dos formas de ejecutar el simulador:

### 1. **Desde archivo de entrada**

Puedes pasarle un archivo `.txt` con los procesos como argumento:

```bash
./simulador procesos.txt
```

El archivo debe tener líneas como estas:

```
PID: 1, AX=0, BX=2, CX=1, Quantum=3
PID: 2, AX=1, BX=0, CX=0, Quantum=2
```

Cada proceso debe tener un archivo `PID.txt` con instrucciones. Ejemplo para `1.txt`:

```
ADD AX, 5
MUL BX, 2
NOP
```

### 2. **Desde consola**

Si ejecutas sin argumentos:

```bash
./simulador
```

Podrás ingresar los procesos manualmente. Ejemplo:

```
>> PID: 1, AX=1, BX=0, CX=0, Quantum=3
>> PID: 2, AX=0, BX=3, CX=2, Quantum=2
>> fin
```

---

## Instrucciones soportadas

Estas son las instrucciones válidas en los archivos `.txt` de cada proceso:

| Instrucción | Descripción                                      |
|-------------|--------------------------------------------------|
| `ADD R, X`  | Suma `X` al registro `R`                         |
| `SUB R, X`  | Resta `X` al registro `R`                        |
| `MUL R, X`  | Multiplica el registro `R` por `X`               |
| `INC R`     | Incrementa el valor del registro `R` en 1        |
| `JMP N`     | Salta a la instrucción número `N` (índice base 0)|
| `NOP`       | No hace nada (salto de ciclo simulado)           |

---

## Ejemplo de ejecución

```bash
./simulador procesos.txt
```

Salida esperada:

```
Procesos cargados:
PID: 1, AX=1, BX=0, CX=0, Quantum=3, Estado=Listo
PID: 2, AX=0, BX=3, CX=2, Quantum=2, Estado=Listo

[Cambio de contexto]
Guardando estado de Proceso 1...
Cargando estado de Proceso 1...
Proceso 1 ejecutando instrucción 0: ADD AX, 5
Valores después: AX=6, BX=0, CX=0
...
```

---

## Limpieza

Para borrar el ejecutable:

```bash
rm simulador
```

---

## Requisitos

- Compilador GCC
- Entorno POSIX (Linux, WSL, MacOS)
- Archivos `.txt` de instrucciones por proceso

---

## Autor

Simulador desarrollado como proyecto educativo para comprender el funcionamiento de un **planificador Round-Robin**, con cambio de contexto simulado y registros de CPU.