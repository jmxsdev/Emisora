# Seguimiento de Issues y Soluciones

Este documento sirve como un registro de los problemas técnicos encontrados en el proyecto, el proceso de diagnóstico y las soluciones implementadas.

---

## Issue #1: Pérdida del primer carácter al leer archivos

*   **Fecha:** 28 de septiembre de 2025
*   **Estado:** Abierto

### Descripción del Problema

Al ejecutar el programa, se observa que los nombres de las canciones (a excepción de la primera) y de los shows son incorrectos: les falta el primer carácter. Por ejemplo, "Let It Be" podría leerse como "et It Be".

El problema se origina en las funciones `leer_canciones()` y `leer_shows()`, que son responsables de parsear los archivos `canciones1.in` y `shows1.in`.

### Diagnóstico Inicial

La hipótesis principal es un manejo incorrecto del puntero del archivo al mezclar `fscanf()` con `fgets()`.

1.  La función `fscanf(archivo, "%d", &num_canciones);` lee el número de la primera línea.
2.  Importante: `fscanf` deja el puntero del archivo justo después del número, pero **antes** del carácter de nueva línea (`\n`) que finaliza esa línea.
3.  Dentro del bucle `for` que lee cada canción, una llamada a `fgetc(archivo);` consume un carácter.
    *   En la **primera iteración**, este `fgetc` consume correctamente el `\n` residual de la línea leída por `fscanf`. Luego, `fgets` lee la primera línea de la canción completa y correctamente.
    *   En la **segunda iteración** (y subsiguientes), el `fgetc` se ejecuta de nuevo. Sin embargo, como la llamada `fgets` anterior ya había consumido la línea completa (incluyendo su `\n`), el puntero ya está al inicio de la siguiente línea. Por lo tanto, este `fgetc` consume el **primer carácter** del nombre de la canción/show, causando el error.

### Solución Propuesta

La solución es mover la llamada a `fgetc(archivo);` para que se ejecute **una sola vez**, justo después de `fscanf` y antes de que comience el bucle `for`. De esta manera, solo consumirá el `\n` inicial y no interferirá con las lecturas posteriores de `fgets`.

El código corregido debería verse así:

```c
// En leer_canciones() y leer_shows()

// Se lee el númeroscanf(archivo, "%d", &num_canciones);

// Se consume el newline UNA SOLA VEZ, fuera del bucle
fgetc(archivo); 

for (int i = 0; i < num_canciones; i++) {
    // Ya no hay fgetc aquí dentro
    fgets(linea, sizeof(linea), archivo);
    // ... resto del parseo
}
```

```