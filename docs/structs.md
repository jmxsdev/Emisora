# Estructuras de Datos del Proyecto

Este documento detalla las estructuras de datos (`structs`) utilizadas en el sistema de planificación de Éxitos FM para modelar las entidades principales: canciones, publicidades, shows y los elementos de la programación final.

---

## Estructura `Cancion`

Representa una canción individual disponible en la emisora.

```c
typedef struct {
    char Nom[51];
    int Min;
    int Seg;
    int duracion_segundos;
    int Punt;
} Cancion;
```

### Campos

| Campo | Tipo | Tamaño | Descripción |
| :--- | :--- | :--- | :--- |
| `Nom` | `char[]` | 51 | Nombre de la canción. Limitado a 50 caracteres más el terminador nulo. |
| `Min` | `int` | | Parte de los minutos de la duración total de la canción. |
| `Seg` | `int` | | Parte de los segundos de la duración total de la canción. |
| `duracion_segundos` | `int` | | Duración total de la canción calculada en segundos. Se usa para facilitar los cálculos de tiempo. |
| `Punt` | `int` | | Nivel de popularidad de la canción, en una escala de 1 a 100. |

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

| Campo | Tipo | Tamaño | Descripción |
| :--- | :--- | :--- | :--- |
| `empresa` | `char[]` | 31 | Nombre de la empresa patrocinante. Limitado a 30 caracteres más el terminador nulo. |
| `segundos` | `int` | | Duración de la cuña en segundos. |
| `veces` | `int` | | Número de veces que la cuña debe ser reproducida durante el día. |
| `duracion_segundos` | `int` | | Duración total en segundos. Es redundante con `segundos` pero mantiene consistencia con las otras estructuras. |

---

## Estructura `Show`

Representa un show o programa radial, que puede estar compuesto por varios segmentos.

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

| Campo | Tipo | Tamaño | Descripción |
| :--- | :--- | :--- | :--- |
| `nombre` | `char[]` | 101 | Nombre del show. Limitado a 100 caracteres más el terminador nulo. |
| `minutos` | `int` | | Duración en minutos de **un solo segmento** del show. |
| `segundos` | `int` | | Duración en segundos de **un solo segmento** del show. |
| `segmentos` | `int` | | Cantidad total de segmentos que componen el show a lo largo del día. |
| `preferencia` | `int` | | Nivel de rating o popularidad del show, en una escala de 1 a 10. |
| `duracion_segundos` | `int` | | Duración total en segundos de **un solo segmento**. |

---

## Estructura `ElementoProgramacion`

Es una estructura genérica utilizada para poblar la parrilla de programación final. Cada instancia de esta estructura representa un bloque de contenido (una canción, un show o una publicidad) que se emitirá en un momento específico.

```c
typedef struct {
    char tipo; // 'S', 'P', 'C'
    char nombre[101];
    int duracion_segundos;
    int hora_inicio; // en segundos desde 00:00:00
} ElementoProgramacion;
```

### Campos

| Campo | Tipo | Tamaño | Descripción |
| :--- | :--- | :--- | :--- |
| `tipo` | `char` | | Un carácter que identifica el tipo de contenido: 'S' para Show, 'P' para Publicidad, 'C' para Canción. |
| `nombre` | `char[]` | 101 | Nombre del elemento a transmitir. Se usa el tamaño más grande (el del nombre del show) para ser flexible. |
| `duracion_segundos` | `int` | | Duración total del elemento en segundos. |
| `hora_inicio` | `int` | | El momento exacto de inicio de la transmisión, medido en segundos desde la medianoche (00:00:00). |
