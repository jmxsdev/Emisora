# Éxitos FM - Planificador de Programación Automático

## Descripción del Proyecto

Este proyecto es una solución de software "inteligente" diseñada para la emisora radial Éxitos FM. Su objetivo principal es automatizar la creación de la parrilla de programación diaria y semanal, optimizando el rating al asignar estratégicamente canciones, shows y publicidad en los horarios más convenientes.

El software ahora cuenta con **persistencia de datos**: las grillas generadas se guardan en archivos de salida (`grilla_*.out`) y se leen en futuras ejecuciones, evitando la necesidad de regenerar la programación cada vez que se realiza una consulta.

## Arquitectura del Proyecto

El sistema está desarrollado en C y sigue una arquitectura modular simple, ahora con una capa de persistencia.

### Componentes Principales:

1.  **Módulo de Carga de Contenido:**
    *   Funciones: `leer_canciones()`, `leer_publicidad()`, `leer_shows()`.
    *   Responsabilidad: Leer y parsear los archivos de entrada (`canciones.in`, etc.) para cargar el contenido disponible (canciones, shows) en memoria al iniciar el programa.

2.  **Motor de Planificación (`generar_programacion_dia()`):**
    *   Es el núcleo que genera la programación para un día completo. Utiliza un algoritmo basado en reglas (horarios estelares, cuotas de publicidad) y aleatoriedad (`rand()`) para crear una parrilla balanceada y única.

3.  **Módulo de Persistencia:**
    *   Funciones: `verificar_grilla_existe()`, `cargar_grilla_desde_archivo()`, `guardar_grilla_en_archivo()`.
    *   Responsabilidad: Manejar el ciclo de vida de los archivos de grilla (`.out`). Guarda una programación generada en disco y la lee cuando se solicita, implementando la persistencia de datos.

4.  **Interfaz de Usuario (CLI en `main()`):**
    *   Un menú interactivo que orquesta el flujo de trabajo:
        *   **Opción 1 (Generar Semana):** Itera 7 días, llamando al motor de planificación y al módulo de persistencia para guardar cada día.
        *   **Opciones 2 y 3 (Consultas):** Ya no regeneran la grilla. Primero usan `verificar_grilla_existe()` para comprobar si el archivo `.out` del día solicitado existe. 
            *   Si existe, lo carga con `cargar_grilla_desde_archivo()` y muestra la información.
            *   Si no existe, pregunta al usuario si desea generar y guardar la grilla en ese momento.

### Estructuras de Datos:

El programa utiliza `structs` para modelar las entidades principales: `Cancion`, `Publicidad`, `Show` y `ElementoProgramacion`.

## Diagrama de Flujo del Proyecto

A continuación se presenta el diagrama de flujo actualizado que describe la nueva operación del software con persistencia.

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
    K --> L[Mostrar Programación];
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
    S --> T[Consultar Momento Específico];
    T --> C;
    R -- No --> U{Generar ahora? (s/n)};
    U -- Sí --> V[generar_programacion_dia];
    V --> W[guardar_grilla_en_archivo];
    W --> T;
    U -- No --> C;

    C --> X[Opción 4: Salir];
    X --> Y[Fin];
```

## Posibles Mejoras

1.  **Algoritmo de Planificación Avanzado:** Implementar un algoritmo más sofisticado (e.g., genético) para optimizar la parrilla.
2.  **Flexibilidad de Archivos de Entrada:** Permitir especificar las rutas de los archivos como argumentos de línea de comandos.
3.  **Base de Datos:** Usar SQLite para una gestión de contenido más robusta.
4.  **Interfaz Gráfica de Usuario (GUI):** Desarrollar una interfaz visual para una operación más amigable.
5.  **Generación de Reportes:** Añadir funcionalidades para generar reportes de uso y rating.

## Guía de Troubleshooting

**Problema: El programa muestra "Error: No se pudo abrir [archivo].in"**
*   **Causa:** El programa no encuentra los archivos de contenido en `tests/files/`.
*   **Solución:** Asegúrese de que los archivos `.in` existan en la ruta correcta relativa a la ejecución del programa.

**Problema: El programa dice que la grilla está "vacía o corrupta" al leerla.**
*   **Causa:** El archivo `grilla_*.out` está vacío, tiene un formato incorrecto o el programa no pudo parsearlo. Esto puede ocurrir si el archivo fue modificado manualmente.
*   **Solución:** Borre el archivo `.out` problemático y permita que el programa lo genere de nuevo con la opción 1 o aceptando la sugerencia en las opciones de consulta.

**Problema: Los archivos `grilla_*.out` no se generan.**
*   **Causa:** Permisos de escritura insuficientes en el directorio.
*   **Solución:** Asegúrese de tener permisos para crear y escribir archivos en la carpeta actual.
