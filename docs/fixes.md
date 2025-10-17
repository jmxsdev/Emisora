# Historial de Cambios y Mejoras (Fixes & Refactors)

Este documento resume las correcciones de errores (`fixes`) y las mejoras de código (`refactors`) más importantes que se han aplicado al proyecto a lo largo del tiempo.

## Refactors (Mejoras de Código)

*   **Ajuste de `HORA_INICIO` (cee00f3):** Se modificó la constante `HORA_INICIO` a `0` para que la programación comience oficialmente a las 00:00:00, simplificando los cálculos de tiempo.
*   **Reorganización de Estructuras (85068da):** Se reorganizaron las constantes y se introdujo la estructura `BloqueEstelar` para hacer más clara y mantenible la lógica de planificación de shows.
*   **Implementación de Planificación de Shows (37e0072):** Se integró en `generar_programacion_dia` un algoritmo de dos pasadas para asignar shows a bloques estelares (mañana, mediodía, noche) de forma más inteligente, priorizando los de mayor preferencia.
*   **Simplificación de Lógica de Publicidad (1e48b89):** Se refactorizó la selección de publicidad para hacerla más simple y evitar la repetición consecutiva del mismo anuncio.
*   **Optimización de Selección de Contenido (2bdebb1):** Se mejoró la lógica general para evitar la repetición de canciones (regla de 4 horas) y optimizar la selección de contenido para rellenar la parrilla.
*   **Actualización de Nomenclatura (2580156):** Se cambió el nombre de la estructura `Canciones` a `Cancion` y se ajustó su uso en todo el código para mayor consistencia.

## Fixes (Correcciones de Errores)

*   **Sintaxis de Diagramas Mermaid (1f0cba7, aa5538d, 982b72a):** Se corrigieron múltiples errores de sintaxis en los diagramas `mermaid` dentro de los archivos `README.md` y `docs/functions.md` para que se rendericen correctamente.
*   **Lectura de Archivos de Entrada (c37355f):** Se solucionó un error crítico donde se perdía el primer carácter del nombre de canciones y shows al leer los archivos `.in`. La lectura ahora usa `fgets` y un parseo manual más robusto para capturar las líneas completas.