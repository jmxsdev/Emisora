# Documentación de Funciones del Proyecto

Este documento ofrece una explicación detallada de cada una de las funciones implementadas en el sistema de planificación de Éxitos FM.

## Librerías Utilizadas

*   `<stdio.h>`: Para operaciones de entrada/salida (archivos `.in`, `.out`, consola).
*   `<string.h>`: Para manipulación de cadenas (`strcpy`, `strncpy`, `strrchr`).
*   `<stdlib.h>`: Para conversión de texto (`atoi`) y números aleatorios (`rand`, `srand`, `qsort`).
*   `<time.h>`: Para la semilla del generador de números aleatorios (`time()`).

---

## Funciones de Lectura de Contenido

Estas funciones leen los archivos `.in` para cargar el catálogo de contenido en memoria.

### `void leer_canciones()`
*   **Propósito:** Lee `tests/files/canciones1.in` y carga las canciones en el array global `canciones[]`.
*   **Lógica:** Usa `fgets` y `strrchr` para un parseo robusto de cada línea, evitando errores si los nombres contienen espacios.

### `void leer_publicidad()`
*   **Propósito:** Lee `tests/files/publicidad1.in` y carga las publicidades en `publicidades[]`.

### `void leer_shows()`
*   **Propósito:** Lee `tests/files/shows1.in` y carga los shows en `shows[]`.

---

## Funciones del Motor de Planificación

### `int comparar_shows(const void *a, const void *b)`
*   **Propósito:** Función de comparación para `qsort`. Permite ordenar un array de índices de shows en orden descendente según su `preferencia`.
*   **Parámetros:** Punteros `a` y `b` a los índices de show que se están comparando.
*   **Retorno:** Un entero negativo, cero o positivo si el show `a` es, respectivamente, mayor, igual o menor que el show `b` en preferencia.
*   **Uso:** Esencial en la Fase 1 de `generar_programacion_dia` para priorizar los shows más importantes.

### `int puede_repetir_cancion(...)`
*   **Propósito:** Verifica si una canción puede ser reproducida, aplicando la regla de negocio de no repetir la misma canción en un lapso de 4 horas.
*   **Retorno:** `1` si puede repetirse, `0` en caso contrario.

### `void generar_programacion_dia(int dia_semana)`
*   **Propósito:** Orquesta toda la generación de la parrilla para un día. Es la función más compleja del sistema.
*   **Lógica de Planificación (en dos fases):**
    1.  **Fase 1: Planificación Estratégica de Shows**
        *   **Ordenamiento:** Crea una lista de índices de shows y la ordena de mayor a menor `preferencia` usando `qsort` con `comparar_shows`.
        *   **Definición de Bloques:** Establece tres `BloqueEstelar`: Mañana (7:00-9:00), Mediodía (12:00-14:00) y Noche (18:00-19:00).
        *   **Asignación Híbrida:**
            *   *Pasada 1:* Intenta asignar al menos un show (el de mayor preferencia disponible) a cada bloque para asegurar que todos tengan contenido estelar.
            *   *Pasada 2:* Asigna los shows restantes al bloque que tenga más espacio libre, optimizando el uso de los horarios de alta audiencia.

    2.  **Fase 2: Generación de la Grilla Final**
        *   **Iteración Diaria:** Recorre el día segundo a segundo, desde `HORA_INICIO`.
        *   **Inserción de Bloques:** Cuando el `tiempo_actual` llega a la hora de inicio de un bloque estelar (7:00, 12:00, 18:00), inserta todos los shows planificados para ese bloque en la parrilla.
        *   **Relleno con Contenido:** En el tiempo restante entre los bloques de shows, el algoritmo rellena la parrilla alternando entre:
            *   **Canciones:** Selecciona una canción aleatoria que cumpla la regla de no repetición (`puede_repetir_cancion`).
            *   **Publicidad:** Si no encuentra una canción adecuada, inserta una cuña publicitaria, asegurándose de no repetir la misma que la anterior.

---

## Funciones de Persistencia

### `int verificar_grilla_existe(const char* dia_nombre)`
*   **Propósito:** Comprueba si un archivo `grilla_*.out` ya existe en el disco.
*   **Retorno:** `1` si existe, `0` si no.

### `int cargar_grilla_desde_archivo(const char* dia_nombre)`
*   **Propósito:** Lee un archivo `grilla_*.out` y carga su contenido en `programacion_dia[]`.
*   **Retorno:** El número de elementos cargados. `0` si hay un error.
*   **Lógica:** Usa `fgets` y `sscanf` para un parseo seguro, previniendo desbordamientos de búfer.

### `void guardar_grilla_en_archivo(int dia_semana)`
*   **Propósito:** Escribe el contenido de `programacion_dia[]` a un archivo `grilla_{dia}.out`.

---

## Funciones de Consulta y Menú

### `void consultar_momento_especifico(...)`
*   **Propósito:** Muestra qué se está transmitiendo en un momento exacto del día cargado en memoria.

### `void mostrar_programacion_completa()`
*   **Propósito:** Imprime en consola la programación completa del día cargado.

### `void mostrar_menu()`
*   **Propósito:** Imprime las opciones del menú principal.

### `int main()`
*   **Propósito:** Punto de entrada. Orquesta la carga de contenido, el menú y las acciones del usuario, interactuando con las funciones de persistencia y planificación.
