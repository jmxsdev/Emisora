#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Estructuras para los diferentes tipos de contenido
typedef struct {
    char Nom[51];
    int Min;
    int Seg;
    int Punt;
    int duracion_segundos;
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
    fgetc(archivo); // consumir el newline anterior
    for (int i = 0; i < num_canciones; i++) {
        // Leer hasta el final de la línea para el nombre
        char linea[200];
        fgets(linea, sizeof(linea), archivo);
        
        // Parsear la línea manualmente
        char *token = strrchr(linea, ' ');
        canciones[i].Punt = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        canciones[i].Seg = atoi(token);
        *token = '\0';
        
        token = strrchr(linea, ' ');
        canciones[i].Min = atoi(token);
        *token = '\0';
        
        // El resto es el nombre
        strncpy(canciones[i].Nom, linea, 50);
        canciones[i].Nom[50] = '\0';
        
        // Calcular duración total en segundos
        canciones[i].duracion_segundos = canciones[i].Min * 60 + canciones[i].Seg;
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
    fgetc(archivo); // consumir el newline anterior
    for (int i = 0; i < num_shows; i++) {
        char linea[200];
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
    
    // Contadores y flags para la nueva lógica
    int contador_publicidad[MAX_PUBLICIDAD] = {0};
    int segmentos_programados[MAX_SHOWS] = {0};
    int show_usado_hoy[MAX_SHOWS] = {0}; // <-- REQ 1: Para no repetir shows en el día
    int ultimo_anuncio_idx = -1; // <-- REQ 2: Para no repetir publicidad seguida

    // Semilla diferente para cada día
    srand(time(NULL) + dia_semana);
    
    while (tiempo_actual < SEGUNDOS_DIA && num_elementos < MAX_ELEMENTOS - 1) {
        int hora_actual = tiempo_actual / 3600;
        int es_horario_estelar = (hora_actual >= 7 && hora_actual < 9) ||
                               (hora_actual >= 12 && hora_actual < 14) ||
                               (hora_actual >= 18 && hora_actual < 19);
        
        int programado = 0; // Flag para saber si ya se programó algo en esta iteración

        // 1. Priorizar shows en horarios estelares
        if (es_horario_estelar && (rand() % 100) < 60) {
            int show_idx = -1;
            for (int i = 0; i < num_shows; i++) {
                // REQ 1: No repetir un show que ya se emitió en el día.
                if (show_usado_hoy[i] == 0 && segmentos_programados[i] < shows[i].segmentos) {
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
                
                tiempo_actual += shows[show_idx].duracion_segundos + 1;
                segmentos_programados[show_idx]++;
                show_usado_hoy[show_idx] = 1; // Marcar show como usado para todo el día
                num_elementos++;
                programado = 1;
            }
        }

        if (programado) continue;

        // 2. Intentar programar una canción (respetando reglas) o publicidad
        int cancion_idx = -1;
        if (num_canciones > 0) { // Solo intentar si hay canciones cargadas
            int intentos = 0;
            while (intentos < num_canciones * 2) { // Intentar encontrar una canción válida
                int idx = rand() % num_canciones;
                if (puede_repetir_cancion(idx, tiempo_actual, ultima_reproduccion) &&
                    tiempo_actual + canciones[idx].duracion_segundos <= SEGUNDOS_DIA) {
                    
                    if (cancion_idx == -1 || (es_horario_estelar && canciones[idx].Punt > canciones[cancion_idx].Punt)) {
                       cancion_idx = idx;
                       if (!es_horario_estelar) break; // Fuera de horario estelar, la primera válida es suficiente
                    }
                }
                intentos++;
            }
        }

        // Si se encontró una canción válida, programarla
        if (cancion_idx != -1) {
            programacion_dia[num_elementos].tipo = 'C';
            strcpy(programacion_dia[num_elementos].nombre, canciones[cancion_idx].Nom);
            programacion_dia[num_elementos].duracion_segundos = canciones[cancion_idx].duracion_segundos;
            programacion_dia[num_elementos].hora_inicio = tiempo_actual;
            
            tiempo_actual += canciones[cancion_idx].duracion_segundos + 1;
            ultima_reproduccion[cancion_idx] = tiempo_actual; // Guardar el tiempo de fin
            num_elementos++;
            programado = 1;
        } else {
            // REQ 2: Si no hay canciones disponibles (por la regla de 4h), rellenar con publicidad.
            int pub_idx = -1;
            int intentos_pub = 0;
            // Buscar una publicidad que no sea la última emitida
            while (intentos_pub < num_publicidades * 2 && num_publicidades > 1) {
                int idx = rand() % num_publicidades;
                if (idx != ultimo_anuncio_idx && contador_publicidad[idx] < publicidades[idx].veces) {
                    pub_idx = idx;
                    break;
                }
                intentos_pub++;
            }

            // Si no se encontró una aleatoria, buscar la primera disponible que no sea la última
            if (pub_idx == -1) {
                for (int i = 0; i < num_publicidades; i++) {
                    if (i != ultimo_anuncio_idx && contador_publicidad[i] < publicidades[i].veces) {
                        pub_idx = i;
                        break;
                    }
                }
            }
            
            // Si incluso así no se encuentra (solo queda una opción y es la última), se usa esa.
            if (pub_idx == -1) {
                 for (int i = 0; i < num_publicidades; i++) {
                    if (contador_publicidad[i] < publicidades[i].veces) {
                        pub_idx = i;
                        break;
                    }
                }
            }

            if (pub_idx != -1 && tiempo_actual + publicidades[pub_idx].duracion_segundos <= SEGUNDOS_DIA) {
                programacion_dia[num_elementos].tipo = 'P';
                strcpy(programacion_dia[num_elementos].nombre, publicidades[pub_idx].empresa);
                programacion_dia[num_elementos].duracion_segundos = publicidades[pub_idx].duracion_segundos;
                programacion_dia[num_elementos].hora_inicio = tiempo_actual;
                
                tiempo_actual += publicidades[pub_idx].duracion_segundos + 1;
                contador_publicidad[pub_idx]++;
                ultimo_anuncio_idx = pub_idx; // Guardar la última publicidad emitida
                num_elementos++;
                programado = 1;
            }
        }

        // Si no se pudo programar nada (ni show, ni canción, ni publicidad), salir para evitar bucle infinito.
        if (!programado) {
            break;
        }
    }
}

// --- Funciones de Persistencia ---

// Nombres de los días para los archivos
const char *nombres_dias[] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};

int verificar_grilla_existe(const char* dia_nombre) {
    char nombre_archivo[50];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "grilla_%s.out", dia_nombre);
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo) {
        fclose(archivo);
        return 1; // Existe
    }
    return 0; // No existe
}

int cargar_grilla_desde_archivo(const char* dia_nombre) {
    char nombre_archivo[50];
    snprintf(nombre_archivo, sizeof(nombre_archivo), "grilla_%s.out", dia_nombre);
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        return 0; // No se pudo abrir
    }

    num_elementos = 0;
    char linea[200]; // Buffer para una línea

    // Leer línea por línea para mayor robustez
    while (fgets(linea, sizeof(linea), archivo) != NULL && num_elementos < MAX_ELEMENTOS) {
        int horas, minutos, segundos;
        char tipo;
        char nombre_buffer[101] = ""; // Inicializar para seguridad

        // Parsear la línea desde el buffer
        if (sscanf(linea, "%d %d %d %c %100[^\n]", &horas, &minutos, &segundos, &tipo, nombre_buffer) == 5) {
            programacion_dia[num_elementos].hora_inicio = horas * 3600 + minutos * 60 + segundos;
            programacion_dia[num_elementos].tipo = tipo;
            strncpy(programacion_dia[num_elementos].nombre, nombre_buffer, 100);
            programacion_dia[num_elementos].nombre[100] = '\0';

            // Calcular la duración basándose en el inicio del siguiente elemento
            if (num_elementos > 0) {
                programacion_dia[num_elementos - 1].duracion_segundos = 
                    programacion_dia[num_elementos].hora_inicio - programacion_dia[num_elementos - 1].hora_inicio - 1;
            }
            num_elementos++;
        }
    }
    
    // Calcular la duración del último elemento de la parrilla
    if (num_elementos > 0) {
        programacion_dia[num_elementos - 1].duracion_segundos = SEGUNDOS_DIA - programacion_dia[num_elementos - 1].hora_inicio;
    }

    fclose(archivo);
    return num_elementos;
}

void guardar_grilla_en_archivo(int dia_semana) {
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
        int segundos_inicio = total_segundos % 60;
        
        fprintf(archivo, "%02d %02d %02d %c %s\n", 
                horas, minutos, segundos_inicio,
                programacion_dia[i].tipo,
                programacion_dia[i].nombre);
    }
    
    fclose(archivo);
    printf("Grilla para %s guardada exitosamente.\n", nombres_dias[dia_semana]);
}

// --- Funciones de Consulta y Menú ---

void consultar_momento_especifico(int hora, int minuto, int segundo) {
    int tiempo_busqueda = hora * 3600 + minuto * 60 + segundo;
    
    printf("Programación para las %02d:%02d:%02d:\n", hora, minuto, segundo);
    
    for (int i = 0; i < num_elementos; i++) {
        if (programacion_dia[i].hora_inicio <= tiempo_busqueda &&
            programacion_dia[i].hora_inicio + programacion_dia[i].duracion_segundos > tiempo_busqueda) {
            
            int total_segundos = programacion_dia[i].hora_inicio;
            int horas_inicio = total_segundos / 3600;
            int minutos_inicio = (total_segundos % 3600) / 60;
            int segundos_inicio = total_segundos % 60;
            
            printf("En curso: %c %s (Inició: %02d:%02d:%02d, Duración: %d segundos)\n",
                   programacion_dia[i].tipo,
                   programacion_dia[i].nombre,
                   horas_inicio, minutos_inicio, segundos_inicio,
                   programacion_dia[i].duracion_segundos);
            return;
        }
    }
    
    printf("No hay programación en este momento.\n");
}

void mostrar_programacion_completa() {
    printf("\n--- Programación Completa del Día ---\n");
    for (int i = 0; i < num_elementos; i++) {
        int total_segundos = programacion_dia[i].hora_inicio;
        int horas = total_segundos / 3600;
        int minutos = (total_segundos % 3600) / 60;
        int segundos_inicio = total_segundos % 60;
        
        printf("%02d:%02d:%02d %c %s\n", 
               horas, minutos, segundos_inicio,
               programacion_dia[i].tipo,
               programacion_dia[i].nombre);
    }
    printf("--- Fin de la Programación ---\n");
}

void mostrar_menu() {
    printf("\n=== SISTEMA DE PROGRAMACIÓN ÉXITOS FM ===\n");
    printf("1. Generar grilla para toda la semana\n");
    printf("2. Consultar programación de un día\n");
    printf("3. Consultar programación en un momento específico\n");
    printf("4. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    // Leer archivos de entrada de contenido
    leer_canciones();
    leer_publicidad();
    leer_shows();
    
    printf("Datos de contenido cargados:\n");
    printf("- Canciones: %d\n- Publicidades: %d\n- Shows: %d\n", num_canciones, num_publicidades, num_shows);
    
    int opcion;
    
    do {
        mostrar_menu();
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1: { // Generar grilla para toda la semana
                printf("Generando grillas para toda la semana...\n");
                for (int dia = 0; dia < 7; dia++) {
                    generar_programacion_dia(dia);
                    guardar_grilla_en_archivo(dia);
                }
                printf("¡Grillas para toda la semana generadas exitosamente!\n");
                break;
            }
            case 2: { // Consultar programación de un día
                int dia_idx;
                printf("Ingrese el día a consultar (0=lunes, ..., 6=domingo): ");
                scanf("%d", &dia_idx);

                if (dia_idx < 0 || dia_idx > 6) {
                    printf("Día inválido.\n");
                    break;
                }

                const char* dia_nombre = nombres_dias[dia_idx];
                if (verificar_grilla_existe(dia_nombre)) {
                    printf("Cargando grilla existente para %s...\n", dia_nombre);
                    if (cargar_grilla_desde_archivo(dia_nombre) > 0) {
                        mostrar_programacion_completa();
                    } else {
                        printf("Error: La grilla para %s está vacía o corrupta.\n", dia_nombre);
                    }
                } else {
                    char respuesta;
                    printf("No hay programación generada para %s. ¿Desea generarla ahora? (s/n): ", dia_nombre);
                    scanf(" %c", &respuesta);
                    if (respuesta == 's' || respuesta == 'S') {
                        generar_programacion_dia(dia_idx);
                        guardar_grilla_en_archivo(dia_idx);
                        mostrar_programacion_completa();
                    }
                }
                break;
            }
            case 3: { // Consultar programación en un momento específico
                int dia_idx, hora, minuto, segundo;
                printf("Ingrese el día (0=lunes, ..., 6=domingo): ");
                scanf("%d", &dia_idx);
                printf("Ingrese hora (0-23): ");
                scanf("%d", &hora);
                printf("Ingrese minuto (0-59): ");
                scanf("%d", &minuto);
                printf("Ingrese segundo (0-59): ");
                scanf("%d", &segundo);

                if (dia_idx < 0 || dia_idx > 6) {
                    printf("Día inválido.\n");
                    break;
                }

                const char* dia_nombre = nombres_dias[dia_idx];
                if (verificar_grilla_existe(dia_nombre)) {
                    if (cargar_grilla_desde_archivo(dia_nombre) > 0) {
                        consultar_momento_especifico(hora, minuto, segundo);
                    } else {
                        printf("Error: La grilla para %s está vacía o corrupta.\n", dia_nombre);
                    }
                } else {
                    char respuesta;
                    printf("No hay programación generada para %s. ¿Desea generarla ahora? (s/n): ", dia_nombre);
                    scanf(" %c", &respuesta);
                    if (respuesta == 's' || respuesta == 'S') {
                        generar_programacion_dia(dia_idx);
                        guardar_grilla_en_archivo(dia_idx);
                        consultar_momento_especifico(hora, minuto, segundo);
                    }
                }
                break;
            }
            case 4:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opción inválida.\n");
                break;
        }
    } while (opcion != 4);
    
    return 0;
}