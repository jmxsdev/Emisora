# Documentación de Funciones del Proyecto

Este documento ofrece una explicación detallada de cada una de las funciones implementadas en el sistema de planificación de Éxitos FM. Se incluye su propósito, parámetros, valores de retorno, lógica interna y diagramas de flujo para ilustrar su funcionamiento.

## Librerías Utilizadas

El proyecto utiliza las siguientes librerías estándar de C:

*   `<stdio.h>`: Para operaciones de entrada y salida, como leer y escribir todos los archivos (`.in` y `.out`) y la interacción por consola.
*   `<string.h>`: Para la manipulación de cadenas de texto (`strcpy`, `strncpy`, `strrchr`).
*   `<stdlib.h>`: Para la conversión de texto a números (`atoi`) y la generación de números aleatorios (`rand`, `srand`).
*   `<time.h>`: Para obtener la hora actual (`time()`), que sirve como semilla para el generador de números aleatorios.

---

## Diagrama de Flujo General del Programa

Este diagrama ilustra el flujo de ejecución principal, incluyendo la nueva lógica de persistencia de datos.

```mermaid
graph TD
    A[Inicio] --> B[Cargar Contenido (.in)];
    B --> C{Mostrar Menú Principal};
    
    C --> D{Opción 1: Generar Semana};
    D --> E[Bucle 7 días];
    E --> F[generar_programacion_dia];
    F --> G[guardar_grilla_en_archivo];
    G --> E;
    E -- Finalizado --> C;

    C --> H{Opción 2: Consultar Día};
    H --> I[Solicitar Día];
    I --> J{verificar_grilla_existe?};
    J -- Sí --> K[cargar_grilla_desde_archivo];
    K --> L[mostrar_programacion_completa];
    L --> C;
    J -- No --> M{Generar ahora? (s/n)};
    M -- Sí --> N[generar_programacion_dia];
    N --> O[guardar_grilla_en_archivo];
    O --> L;
    M -- No --> C;

    C --> P{Opción 3: Consultar Momento};
    P --> Q[Solicitar Día y Hora];
    Q --> R{verificar_grilla_existe?};
    R -- Sí --> S[cargar_grilla_desde_archivo];
    S --> T[consultar_momento_especifico];
    T --> C;
    R -- No --> U{Generar ahora? (s/n)};
    U -- Sí --> V[generar_programacion_dia];
    V --> W[guardar_grilla_en_archivo];
    W --> T;
    U -- No --> C;

    C --> X[Opción 4: Salir];
    X --> Y[Fin];
```

---

## Funciones de Lectura de Contenido

Estas funciones leen los archivos `.in` para cargar el catálogo de contenido disponible en memoria.

### `void leer_canciones()`
*   **Propósito:** Lee `tests/files/canciones1.in` para cargar las canciones en el array global `canciones[]`.
*   **Parámetros:** Ninguno.
*   **Retorno:** `void`.

### `void leer_publicidad()`
*   **Propósito:** Lee `tests/files/publicidad1.in` para cargar las publicidades en el array global `publicidades[]`.
*   **Parámetros:** Ninguno.
*   **Retorno:** `void`.

### `void leer_shows()`
*   **Propósito:** Lee `tests/files/shows1.in` para cargar los shows en el array global `shows[]`.
*   **Parámetros:** Ninguno.
*   **Retorno:** `void`.

---

## Funciones del Motor de Planificación

### `int puede_repetir_cancion(...)`
*   **Propósito:** Verifica si una canción puede ser reproducida (regla de no repetición de 4 horas).
*   **Retorno:** `1` si puede repetirse, `0` en caso contrario.

### `void generar_programacion_dia(int dia_semana)`
*   **Propósito:** Genera la parrilla de programación completa para un día, llenando el array global `programacion_dia[]`.
*   **Parámetros:** `dia_semana` (0-6) para inicializar la semilla de `rand()`.
*   **Retorno:** `void`.

---

## Funciones de Persistencia

Este conjunto de funciones maneja el guardado y la carga de las grillas de programación en archivos `.out`.

### `int verificar_grilla_existe(const char* dia_nombre)`
*   **Propósito:** Comprueba si un archivo de grilla (e.g., `grilla_lunes.out`) ya existe.
*   **Parámetros:** `dia_nombre` (e.g., "lunes").
*   **Retorno:** `1` si el archivo existe, `0` si no.
*   **Lógica:** Intenta abrir el archivo en modo lectura (`"r"`). Si `fopen` devuelve un puntero válido, el archivo existe. Se asegura de cerrarlo inmediatamente.

### `int cargar_grilla_desde_archivo(const char* dia_nombre)`
*   **Propósito:** Lee un archivo `grilla_*.out` y carga su contenido en el array global `programacion_dia[]`.
*   **Parámetros:** `dia_nombre`.
*   **Retorno:** El número de elementos cargados (`num_elementos`). Retorna `0` si el archivo no se puede abrir o está vacío/corrupto.
*   **Lógica:**
    1.  Abre el archivo.
    2.  Lee el archivo línea por línea usando `fgets` para mayor robustez.
    3.  Parsea cada línea con `sscanf` usando el formato `"%d %d %d %c %100[^
]"`. El límite `%100` es una medida de seguridad crucial para prevenir desbordamientos de búfer.
    4.  Calcula la `duracion_segundos` de cada elemento (excepto el último) restando su `hora_inicio` de la del siguiente elemento.

### `void guardar_grilla_en_archivo(int dia_semana)`
*   **Propósito:** Escribe el contenido del array `programacion_dia[]` a un archivo de salida `grilla_{dia}.out`.
*   **Parámetros:** `dia_semana` (0-6).
*   **Retorno:** `void`.
*   **Lógica:** Construye el nombre del archivo, lo abre en modo escritura (`"w"`) y escribe cada elemento de la programación en el formato `HH MM SS T Nombre`.

---

## Funciones de Consulta y Menú

### `void consultar_momento_especifico(int hora, int minuto, int segundo)`
*   **Propósito:** Muestra por pantalla qué se está transmitiendo en un momento exacto del día cargado en memoria.
*   **Parámetros:** `hora`, `minuto`, `segundo` del momento a consultar.
*   **Retorno:** `void`.
*   **Nota:** Esta función opera sobre los datos del array global `programacion_dia[]`. La lógica en `main` es responsable de asegurar que los datos del día correcto hayan sido cargados (desde archivo o generados) antes de llamar a esta función.

### `void mostrar_programacion_completa()`
*   **Propósito:** Imprime en consola la programación completa del día actualmente cargado en `programacion_dia[]`.
*   **Parámetros:** Ninguno.
*   **Retorno:** `void`.

### `void mostrar_menu()`
*   **Propósito:** Imprime en pantalla las opciones del menú principal.
*   **Parámetros:** Ninguno.
*   **Retorno:** `void`.

### `int main()`
*   **Propósito:** Punto de entrada del programa. Orquesta la carga de contenido, la muestra del menú y la ejecución de las opciones del usuario.
*   **Lógica del Menú:**
    *   **Casos 2 y 3:** La lógica fue modificada para implementar persistencia. Primero se llama a `verificar_grilla_existe()`. Si la grilla existe, se carga con `cargar_grilla_desde_archivo()`. Si no, se le pregunta al usuario si desea generarla en ese momento. Esto asegura que los datos en disco se usen preferentemente.