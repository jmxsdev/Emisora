# Éxitos FM - Planificador de Programación Automático

## Descripción del Proyecto

Este proyecto es una solución de software "inteligente" diseñada para la emisora radial Éxitos FM. Su objetivo principal es automatizar la creación de la parrilla de programación diaria y semanal, optimizando el rating al asignar estratégicamente canciones, shows y publicidad en los horarios más convenientes.

El software lee la información sobre canciones, shows y patrocinadores desde archivos de entrada, y genera una programación para los 7 días de la semana, guardando cada día en un archivo de salida independiente. Además, ofrece una interfaz de consola para que el operador pueda consultar la programación en momentos específicos.

## Arquitectura del Proyecto

El sistema está desarrollado en C y sigue una arquitectura modular simple.

### Componentes Principales:

1.  **Módulo de Carga de Datos:**
    *   Funciones: `leer_canciones()`, `leer_publicidad()`, `leer_shows()`.
    *   Responsabilidad: Leer y parsear los archivos de entrada (`canciones.in`, `publicidad.in`, `shows.in`) y cargar los datos en arrays de estructuras en memoria (e.g., `Cancion canciones[]`, `Show shows[]`).
    *   Nota: Actualmente, las rutas a estos archivos están codificadas para apuntar a `tests/files/`. Para un uso en producción, esto debería ser parametrizable.

2.  **Motor de Planificación (`generar_programacion_dia()`):**
    *   Este es el núcleo del software. Es una función que genera la programación para un día completo (desde las 00:05:00 hasta las 23:59:59).
    *   Utiliza un algoritmo basado en reglas y aleatoriedad (`rand()`) para tomar decisiones, asegurando que cada día la programación sea única.
    *   **Lógica de Priorización:**
        *   **Horarios Estelares (07:00-09:00, 12:00-14:00, 18:00-19:00):** Se da alta prioridad a los shows con mayor preferencia y a las canciones con mayor puntuación.
        *   **Publicidad:** Se asegura de cumplir con el número de repeticiones diarias para cada patrocinador.
        *   **Canciones:** Se evita la repetición de una misma canción en un lapso de 4 horas para no saturar a la audiencia.

3.  **Módulo de Salida (`guardar_grilla_dia()`):**
    *   Responsabilidad: Escribir la programación generada en los archivos de salida correspondientes (`grilla_lunes.out`, `grilla_martes.out`, etc.), siguiendo el formato `HH MM SS T Nombre`.

4.  **Interfaz de Usuario (CLI en `main()`):**
    *   Un menú interactivo en la consola que permite al usuario:
        *   Generar la programación para toda la semana.
        *   Consultar qué está programado en un día y hora específicos.
        *   Mostrar la programación completa de un día por pantalla.

### Estructuras de Datos:

El programa utiliza `structs` para modelar las entidades principales:

*   `Cancion`: Almacena nombre, duración y puntuación.
*   `Publicidad`: Almacena nombre de la empresa, duración y número de repeticiones.
*   `Show`: Almacena nombre, duración por segmento, número de segmentos y preferencia.
*   `ElementoProgramacion`: Una estructura genérica para representar un item en la parrilla final (sea canción, show o publicidad).

## Diagrama de Flujo del Proyecto

A continuación se presenta un diagrama de flujo que describe la operación del software.

```mermaid
graph TD
    A[Inicio] --> B{Leer Archivos de Entrada};
    B --> C[Cargar Datos en Memoria];
    C --> D{Mostrar Menú Principal};
    D --> E{Opción: Generar Semana};
    D --> F{Opción: Consultar Momento};
    D --> G{Opción: Mostrar Día};
    D --> H[Opción: Salir];

    E --> I[Bucle de 7 días];
    I --> J[llamar a generar_programacion_dia(dia)];
    J --> K[llamar a guardar_grilla_dia(dia)];
    K --> I;
    I -- Finalizado --> D;

    F --> L[Solicitar Día y Hora];
    L --> M[llamar a generar_programacion_dia(dia)];
    M --> N[Buscar en la programación generada];
    N --> O[Mostrar resultado en pantalla];
    O --> D;

    G --> P[Solicitar Día];
    P --> Q[llamar a generar_programacion_dia(dia)];
    Q --> R[Iterar y mostrar programación del día];
    R --> D;

    H --> S[Fin];
```

## Posibles Mejoras

1.  **Algoritmo de Planificación Avanzado:**
    *   Implementar un algoritmo de *backtracking* o *genético* para encontrar una parrilla "óptima" en lugar de una "suficientemente buena" basada en aleatoriedad.
    *   Añadir más reglas, como la compatibilidad entre géneros de canciones o la transición suave entre temas de diferente ritmo.

2.  **Flexibilidad de Archivos de Entrada:**
    *   Permitir que el usuario especifique las rutas de los archivos de entrada como argumentos de línea de comandos en lugar de tenerlas codificadas en el fuente.

3.  **Persistencia de Datos:**
    *   Utilizar una base de datos simple (como SQLite) para almacenar el catálogo de canciones, shows y publicidades. Esto facilitaría la gestión y evitaría tener que leer los archivos cada vez que se ejecuta el programa.

4.  **Interfaz Gráfica de Usuario (GUI):**
    *   Desarrollar una interfaz gráfica que permita arrastrar y soltar elementos, visualizar la parrilla de forma más amigable y realizar ajustes manuales.

5.  **Generación de Reportes:**
    *   Añadir funcionalidades para generar reportes sobre el uso de canciones, cumplimiento de pautas publicitarias y rating promedio por franja horaria.

## Guía de Troubleshooting

**Problema: El programa muestra "Error: No se pudo abrir [archivo].in"**

*   **Causa:** El programa no encuentra los archivos de entrada en la ubicación esperada.
*   **Solución:**
    1.  Asegúrese de que los archivos `canciones1.in`, `publicidad1.in` y `shows1.in` existen.
    2.  Verifique que se encuentran dentro de la carpeta `tests/files/` relativa a la ubicación desde donde se ejecuta el programa.
    3.  Si ejecuta el programa desde un directorio diferente, deberá ajustar las rutas en las funciones `leer_canciones()`, `leer_publicidad()` y `leer_shows()` en `main.c`.

**Problema: Los archivos `grilla_*.out` no se generan o están vacíos.**

*   **Causa 1:** No se seleccionó la opción "Generar grilla para toda la semana" en el menú.
*   **Solución 1:** Ejecute el programa y seleccione la opción 1.
*   **Causa 2:** Permisos de escritura insuficientes en el directorio donde se ejecuta el programa.
*   **Solución 2:** Asegúrese de tener permisos para crear y escribir archivos en la carpeta actual.
*   **Causa 3:** El bucle de generación de la programación termina prematuramente porque no puede encontrar contenido que se ajuste al tiempo restante.
*   **Solución 3:** Revise los archivos de entrada. Podría haber muy pocas canciones o canciones de muy larga duración que impiden llenar la parrilla completa.

**Problema: La programación de cada día es exactamente la misma.**

*   **Causa:** La semilla del generador de números aleatorios (`srand()`) no está cambiando.
*   **Solución:** El código actual (`srand(time(NULL) + dia_semana)`) intenta solucionar esto. Si el problema persiste, asegúrese de que la cabecera `<time.h>` esté incluida y que el sistema operativo esté devolviendo un valor de tiempo que cambie con cada ejecución.

**Problema: El programa se cuelga o entra en un bucle infinito.**

*   **Causa:** Esto podría ocurrir en la función `generar_programacion_dia` si las condiciones para seleccionar contenido son demasiado restrictivas y el programa no puede avanzar en el tiempo.
*   **Solución:** Revise la lógica de selección de contenido. Asegúrese de que siempre haya una opción "por defecto" (como una canción corta) que pueda ser programada si las demás opciones fallan, para garantizar que el `tiempo_actual` siempre progrese.
