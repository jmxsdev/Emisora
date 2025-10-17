# Estructuras de Datos del Proyecto

Este documento detalla las estructuras de datos (`structs`) utilizadas en el sistema de planificación de Éxitos FM para modelar las entidades principales y las estructuras auxiliares para la planificación.

---

## Estructura `Cancion`

Representa una canción individual disponible en la emisora.

```c
typedef struct {
    char Nom[51];
    int Min;
    int Seg;
    int Punt;
    int duracion_segundos;
} Cancion;
```

### Campos

| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `Nom` | `char[]` | Nombre de la canción (hasta 50 caracteres). |
| `Min` | `int` | Minutos de duración. |
| `Seg` | `int` | Segundos de duración. |
| `Punt` | `int` | Popularidad de la canción (1-100). |
| `duracion_segundos` | `int` | Duración total calculada en segundos para facilitar cálculos. |

---

## Estructura `Publicidad`

Representa una cuña publicitaria de un patrocinador.

```c
typedef struct {
    char empresa[31];
    int segundos;
    int veces;
    int duracion_segundos;
} Publicidad;
```

### Campos

| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `empresa` | `char[]` | Nombre de la empresa (hasta 30 caracteres). |
| `segundos` | `int` | Duración de la cuña en segundos. |
| `veces` | `int` | Número de veces que la cuña debe reproducirse al día. |
| `duracion_segundos` | `int` | Duración total en segundos (redundante con `segundos` por consistencia). |

---

## Estructura `Show`

Representa un programa radial, que puede estar compuesto por varios segmentos.

```c
typedef struct {
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos;
    int preferencia;
    int duracion_segundos;
} Show;
```

### Campos

| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `nombre` | `char[]` | Nombre del show (hasta 100 caracteres). |
| `minutos` | `int` | Duración en minutos de **un solo segmento**. |
| `segundos` | `int` | Duración en segundos de **un solo segmento**. |
| `segmentos` | `int` | Cantidad total de segmentos que componen el show. |
| `preferencia` | `int` | Nivel de rating o popularidad del show (1-10). |
| `duracion_segundos` | `int` | Duración en segundos de **un solo segmento**. |

---

## Estructura `ElementoProgramacion`

Estructura genérica para la parrilla de programación. Cada instancia representa un bloque de contenido (canción, show o publicidad) en un momento específico.

```c
typedef struct {
    char tipo; // 'S', 'P', 'C'
    char nombre[101];
    int duracion_segundos;
    int hora_inicio; // en segundos desde 00:00:00
} ElementoProgramacion;
```

### Campos

| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `tipo` | `char` | Identificador del tipo de contenido: 'S' (Show), 'P' (Publicidad), 'C' (Canción). |
| `nombre` | `char[]` | Nombre del elemento a transmitir. |
| `duracion_segundos` | `int` | Duración total del elemento en segundos. |
| `hora_inicio` | `int` | Momento de inicio de la transmisión, en segundos desde la medianoche. |

---

## Estructura `BloqueEstelar`

Estructura auxiliar utilizada por el motor de planificación para agrupar shows en los horarios de mayor audiencia (mañana, mediodía, noche).

```c
typedef struct {
    int show_indices[MAX_SHOWS];
    int num_shows;
    int duracion_total;
} BloqueEstelar;
```

### Campos

| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `show_indices` | `int[]` | Array de índices que apuntan a los shows (en el array global `shows[]`) asignados a este bloque. |
| `num_shows` | `int` | Contador del número de shows actualmente asignados al bloque. |
| `duracion_total` | `int` | Suma de las duraciones de todos los shows asignados, para controlar que no exceda el tiempo del bloque. |