# Seguimiento de Issues y Soluciones

Este documento sirve como un registro de los problemas técnicos encontrados en el proyecto, el proceso de diagnóstico y las soluciones implementadas.

---

## Issue #1: Pérdida del primer carácter al leer archivos de contenido

*   **Fecha:** 28 de septiembre de 2025
*   **Estado:** Solucionado

### Descripción del Problema

Al leer los archivos `canciones.in` y `shows.in`, el primer carácter del nombre de cada item (excepto el primero) se perdía. Por ejemplo, "Let It Be" se leía como "et It Be".

### Diagnóstico

El error se debía a una llamada a `fgetc()` posicionada incorrectamente dentro de un bucle `for` en las funciones `leer_canciones()` y `leer_shows()`. Esta función, destinada a consumir un único carácter de nueva línea (`\n`) dejado por una llamada previa a `fscanf()`, se ejecutaba en cada iteración, consumiendo así el primer carácter de las líneas subsiguientes.

### Solución

Se movió la llamada a `fgetc()` fuera y antes del bucle `for` en ambas funciones. De este modo, se ejecuta una sola vez y solo consume el carácter de nueva línea original, solucionando el problema de parseo.

---

## Issue #2: Error "grilla vacía o corrupta" al leer grillas guardadas

*   **Fecha:** 28 de septiembre de 2025
*   **Estado:** Solucionado

### Descripción del Problema

Después de implementar la capa de persistencia, el programa guardaba los archivos `grilla_*.out` correctamente. Sin embargo, al intentar leerlos con las opciones 2 o 3 del menú, la función `cargar_grilla_desde_archivo()` fallaba y retornaba `0`, lo que provocaba el mensaje de error "La grilla está vacía o corrupta".

### Diagnóstico

El problema residía en una vulnerabilidad de **desbordamiento de búfer (buffer overflow)** dentro de la función `cargar_grilla_desde_archivo()`. La llamada a `sscanf()` utilizaba el especificador de formato `%[^
]` para leer el nombre del evento, pero no se le había asignado un límite de ancho.

```c
// El código problemático
sscanf(linea, "%d %d %d %c %s[^\n]", ...);
```

Si un nombre de show en el archivo `.out` tenía una longitud cercana al límite del búfer (100 caracteres), `sscanf` escribía más allá de la memoria asignada para la variable `nombre_buffer`, corrompiendo el stack de la función. Esto resultaba en un comportamiento indefinido, haciendo que la función terminara prematuramente y retornara `0`.

### Solución

Se aplicó un parche de seguridad a la llamada a `sscanf()` añadiendo un límite de ancho de `100` al especificador de formato. Esto asegura que `sscanf` no escribirá más caracteres de los que el búfer puede alojar de forma segura.

```c
// La solución final y segura
sscanf(linea, "%d %d %d %c %100[^
]", ...);
```

Este cambio resolvió la corrupción de memoria y permitió que la función parseara los archivos de grilla de manera robusta y predecible.
