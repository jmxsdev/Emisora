#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Estructuras para los diferentes tipos de contenido
typedef struct {
    char nombre[51];
    int minutos;
    int segundos;
    int duracion_segundos;
    int puntuacion;
} Cancion;

typedef struct {
    char empresa[31];
    int segundos;
    int veces;
    int duracion_segundos;
} Publicidad;

typedef struct {
    char nombre[101];
    int minutos;
    int segundos;
    int segmentos;
    int preferencia;
    int duracion_segundos;
} Show;

typedef struct {
    char tipo; // 'S', 'P', 'C'
    char nombre[101];
    int duracion_segundos;
    int hora_inicio; // en segundos desde 00:00:00
} ElementoProgramacion;

// Constantes
#define MAX_CANCIONES 1000
#define MAX_PUBLICIDAD 100
#define MAX_SHOWS 15
#define MAX_ELEMENTOS 5000
#define SEGUNDOS_DIA 86400
#define HORA_INICIO 300 // 00:05:00 en segundos

// Variables globales (memoria estática)
Cancion canciones[MAX_CANCIONES];
Publicidad publicidades[MAX_PUBLICIDAD];
Show shows[MAX_SHOWS];
ElementoProgramacion programacion_dia[MAX_ELEMENTOS];

int num_canciones = 0;
int num_publicidades = 0;
int num_shows = 0;
int num_elementos = 0;

// Funciones para leer archivos de entrada
void leer_canciones() {
    FILE *archivo = fopen("tests/files/canciones1.in", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir canciones.in\n");
        exit(1);
    }

    fscanf(archivo, "%d", &num_canciones);
    for (int i = 0; i < num_canciones; i++) {
        // Leer hasta el final de la línea para el nombre
        char linea[200];
        fgetc(archivo); // consumir el newline anterior
        fgets(linea, sizeof(linea), archivo);
        
        // Parsear la línea manualmente
        char *token = strrchr(linea, ' ');
        canciones[i].puntuacion = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        canciones[i].segundos = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        canciones[i].minutos = atoi(token);
        *token = '\0';
        
        // El resto es el nombre
        strncpy(canciones[i].nombre, linea, 50);
        canciones[i].nombre[50] = '\0';
        
        // Calcular duración total en segundos
        canciones[i].duracion_segundos = canciones[i].minutos * 60 + canciones[i].segundos;
    }
    fclose(archivo);
}

void leer_publicidad() {
    FILE *archivo = fopen("tests/files/publicidad1.in", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir publicidad.in\n");
        exit(1);
    }

    num_publicidades = 0;
    char linea[100];
    while (fgets(linea, sizeof(linea), archivo) != NULL && num_publicidades < MAX_PUBLICIDAD) {
        char *token = strrchr(linea, ' ');
        publicidades[num_publicidades].veces = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        publicidades[num_publicidades].segundos = atoi(token);
        *token = '\0';
        
        // El resto es el nombre de la empresa
        strncpy(publicidades[num_publicidades].empresa, linea, 30);
        publicidades[num_publicidades].empresa[30] = '\0';
        
        publicidades[num_publicidades].duracion_segundos = publicidades[num_publicidades].segundos;
        num_publicidades++;
    }
    fclose(archivo);
}

void leer_shows() {
    FILE *archivo = fopen("tests/files/shows1.in", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir shows.in\n");
        exit(1);
    }

    fscanf(archivo, "%d", &num_shows);
    for (int i = 0; i < num_shows; i++) {
        char linea[200];
        fgetc(archivo); // consumir el newline anterior
        fgets(linea, sizeof(linea), archivo);
        
        // Parsear la línea manualmente
        char *token = strrchr(linea, ' ');
        shows[i].preferencia = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        shows[i].segmentos = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        shows[i].segundos = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        shows[i].minutos = atoi(token);
        *token = '\0';
        
        // El resto es el nombre
        strncpy(shows[i].nombre, linea, 100);
        shows[i].nombre[100] = '\0';
        
        // Calcular duración total en segundos por segmento
        shows[i].duracion_segundos = shows[i].minutos * 60 + shows[i].segundos;
    }
    fclose(archivo);
}

// Función para verificar si una canción puede ser reproducida en un momento dado
int puede_repetir_cancion(int cancion_idx, int tiempo_actual, int ultima_reproduccion[MAX_CANCIONES]) {
    if (ultima_reproduccion[cancion_idx] == -1) return 1;
    return (tiempo_actual - ultima_reproduccion[cancion_idx]) >= (4 * 3600); // 4 horas en segundos
}

// Función para generar la programación de un día
void generar_programacion_dia(int dia_semana) {
    num_elementos = 0;
    int tiempo_actual = HORA_INICIO;
    int ultima_reproduccion[MAX_CANCIONES];
    
    // Inicializar array de últimas reproducciones
    for (int i = 0; i < num_canciones; i++) {
        ultima_reproduccion[i] = -1;
    }
    
    // Array para contar cuántas veces se ha reproducido cada publicidad
    int contador_publicidad[MAX_PUBLICIDAD] = {0};
    
    // Array para contar cuántos segmentos de cada show se han programado
    int segmentos_programados[MAX_SHOWS] = {0};
    
    // Semilla diferente para cada día
    srand(time(NULL) + dia_semana);
    
    while (tiempo_actual < SEGUNDOS_DIA && num_elementos < MAX_ELEMENTOS - 1) {
        // Verificar horarios estelares (07:00-09:00, 12:00-14:00, 18:00-19:00)
        int hora_actual = tiempo_actual / 3600;
        int es_horario_estelar = 0;
        
        if ((hora_actual >= 7 && hora_actual < 9) ||
            (hora_actual >= 12 && hora_actual < 14) ||
            (hora_actual >= 18 && hora_actual < 19)) {
            es_horario_estelar = 1;
        }
        
        // Decidir qué tipo de contenido programar
        int tipo_contenido = rand() % 100;
        
        // Priorizar shows en horarios estelares
        if (es_horario_estelar && tipo_contenido < 60) {
            // Intentar programar un show
            int show_idx = -1;
            for (int i = 0; i < num_shows; i++) {
                if (segmentos_programados[i] < shows[i].segmentos) {
                    if (show_idx == -1 || shows[i].preferencia > shows[show_idx].preferencia) {
                        show_idx = i;
                    }
                }
            }
            
            if (show_idx != -1 && tiempo_actual + shows[show_idx].duracion_segundos <= SEGUNDOS_DIA) {
                programacion_dia[num_elementos].tipo = 'S';
                strcpy(programacion_dia[num_elementos].nombre, shows[show_idx].nombre);
                programacion_dia[num_elementos].duracion_segundos = shows[show_idx].duracion_segundos;
                programacion_dia[num_elementos].hora_inicio = tiempo_actual;
                
                tiempo_actual += shows[show_idx].duracion_segundos + 1; // +1 segundo de separación
                segmentos_programados[show_idx]++;
                num_elementos++;
                continue;
            }
        }
        
        // Programar publicidad si hay que cumplir cuotas
        if (tipo_contenido < 30) {
            int pub_idx = -1;
            for (int i = 0; i < num_publicidades; i++) {
                if (contador_publicidad[i] < publicidades[i].veces) {
                    pub_idx = i;
                    break;
                }
            }
            
            if (pub_idx != -1 && tiempo_actual + publicidades[pub_idx].duracion_segundos <= SEGUNDOS_DIA) {
                programacion_dia[num_elementos].tipo = 'P';
                strcpy(programacion_dia[num_elementos].nombre, publicidades[pub_idx].empresa);
                programacion_dia[num_elementos].duracion_segundos = publicidades[pub_idx].duracion_segundos;
                programacion_dia[num_elementos].hora_inicio = tiempo_actual;
                
                tiempo_actual += publicidades[pub_idx].duracion_segundos + 1;
                contador_publicidad[pub_idx]++;
                num_elementos++;
                continue;
            }
        }
        
        // Programar una canción (caso por defecto)
        int cancion_idx = -1;
        int intentos = 0;
        
        while (intentos < num_canciones * 2 && cancion_idx == -1) {
            int idx = rand() % num_canciones;
            if (puede_repetir_cancion(idx, tiempo_actual, ultima_reproduccion) &&
                tiempo_actual + canciones[idx].duracion_segundos <= SEGUNDOS_DIA) {
                
                // Priorizar canciones con mejor puntuación en horarios estelares
                if (!es_horario_estelar || canciones[idx].puntuacion >= 50 ||
                    (cancion_idx != -1 && canciones[idx].puntuacion > canciones[cancion_idx].puntuacion)) {
                    cancion_idx = idx;
                }
            }
            intentos++;
        }
        
        if (cancion_idx == -1) {
            // Si no encontramos canción válida, buscar cualquier canción que quepa
            for (int i = 0; i < num_canciones; i++) {
                if (tiempo_actual + canciones[i].duracion_segundos <= SEGUNDOS_DIA) {
                    cancion_idx = i;
                    break;
                }
            }
        }
        
        if (cancion_idx != -1) {
            programacion_dia[num_elementos].tipo = 'C';
            strcpy(programacion_dia[num_elementos].nombre, canciones[cancion_idx].nombre);
            programacion_dia[num_elementos].duracion_segundos = canciones[cancion_idx].duracion_segundos;
            programacion_dia[num_elementos].hora_inicio = tiempo_actual;
            
            tiempo_actual += canciones[cancion_idx].duracion_segundos + 1;
            ultima_reproduccion[cancion_idx] = tiempo_actual;
            num_elementos++;
        } else {
            // Si no podemos programar nada más, salir
            break;
        }
    }
}

// Función para guardar la grilla de un día
void guardar_grilla_dia(int dia_semana) {
    char *nombres_dias[] = {
        "lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"
    };
    
    char nombre_archivo[50];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "grilla_%s.out", nombres_dias[dia_semana]);
    
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo crear %s\n", nombre_archivo);
        return;
    }
    
    for (int i = 0; i < num_elementos; i++) {
        int total_segundos = programacion_dia[i].hora_inicio;
        int horas = total_segundos / 3600;
        int minutos = (total_segundos % 3600) / 60;
        int segundos = total_segundos % 60;
        
        fprintf(archivo, "%02d %02d %02d %c %s\n", 
                horas, minutos, segundos,
                programacion_dia[i].tipo,
                programacion_dia[i].nombre);
    }
    
    fclose(archivo);
    printf("Grilla para %s generada exitosamente\n", nombres_dias[dia_semana]);
}

// Función para mostrar la programación en un momento específico
void consultar_momento_especifico(int dia, int hora, int minuto, int segundo) {
    int tiempo_busqueda = hora * 3600 + minuto * 60 + segundo;
    
    printf("Programación para las %02d:%02d:%02d:\n", hora, minuto, segundo);
    
    for (int i = 0; i < num_elementos; i++) {
        if (programacion_dia[i].hora_inicio <= tiempo_busqueda &&
            programacion_dia[i].hora_inicio + programacion_dia[i].duracion_segundos > tiempo_busqueda) {
            
            int total_segundos = programacion_dia[i].hora_inicio;
            int horas = total_segundos / 3600;
            int minutos = (total_segundos % 3600) / 60;
            int segundos_inicio = total_segundos % 60;
            
            printf("En curso: %c %s (Inició: %02d:%02d:%02d, Duración: %d segundos)\n",
                   programacion_dia[i].tipo,
                   programacion_dia[i].nombre,
                   horas, minutos, segundos_inicio,
                   programacion_dia[i].duracion_segundos);
            return;
        }
    }
    
    printf("No hay programación en este momento\n");
}

// Función para mostrar el menú principal
void mostrar_menu() {
    printf("\n=== SISTEMA DE PROGRAMACIÓN ÉXITOS FM ===\n");
    printf("1. Generar grilla para toda la semana\n");
    printf("2. Consultar programación para un momento específico\n");
    printf("3. Mostrar programación del día actual\n");
    printf("4. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    // Leer archivos de entrada
    leer_canciones();
    leer_publicidad();
    leer_shows();
    
    printf("Datos cargados:\n");
    printf("- Canciones: %d\n", num_canciones);
    printf("- Publicidades: %d\n", num_publicidades);
    printf("- Shows: %d\n", num_shows);
    
    int opcion;
    int dia_actual = 0; // 0 = lunes, 6 = domingo
    
    do {
        mostrar_menu();
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                // Generar grilla para toda la semana
                for (int dia = 0; dia < 7; dia++) {
                    generar_programacion_dia(dia);
                    guardar_grilla_dia(dia);
                }
                printf("Grillas para toda la semana generadas exitosamente\n");
                break;
                
            case 2:
                // Consultar programación para un momento específico
                {
                    int dia, hora, minuto, segundo;
                    printf("Ingrese el día (0=lunes, 1=martes, ..., 6=domingo): ");
                    scanf("%d", &dia);
                    printf("Ingrese hora (0-23): ");
                    scanf("%d", &hora);
                    printf("Ingrese minuto (0-59): ");
                    scanf("%d", &minuto);
                    printf("Ingrese segundo (0-59): ");
                    scanf("%d", &segundo);
                    
                    if (dia >= 0 && dia < 7) {
                        generar_programacion_dia(dia);
                        consultar_momento_especifico(dia, hora, minuto, segundo);
                    } else {
                        printf("Día inválido\n");
                    }
                }
                break;
                
            case 3:
                // Mostrar programación del día actual
                {
                    printf("Ingrese el día (0=lunes, 1=martes, ..., 6=domingo): ");
                    scanf("%d", &dia_actual);
                    
                    if (dia_actual >= 0 && dia_actual < 7) {
                        generar_programacion_dia(dia_actual);
                        printf("\nProgramación completa del día:\n");
                        
                        for (int i = 0; i < num_elementos; i++) {
                            int total_segundos = programacion_dia[i].hora_inicio;
                            int horas = total_segundos / 3600;
                            int minutos = (total_segundos % 3600) / 60;
                            int segundos = total_segundos % 60;
                            
                            printf("%02d:%02d:%02d %c %s\n", 
                                   horas, minutos, segundos,
                                   programacion_dia[i].tipo,
                                   programacion_dia[i].nombre);
                        }
                    } else {
                        printf("Día inválido\n");
                    }
                }
                break;
                
            case 4:
                printf("Saliendo del sistema...\n");
                break;
                
            default:
                printf("Opción inválida\n");
                break;
        }
    } while (opcion != 4);
    
    return 0;
}