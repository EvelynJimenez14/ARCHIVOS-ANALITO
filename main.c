#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define TEXT_FILE "datos_salud.txt"
#define BIN_FILE "datos_salud.bin"

typedef struct {
    char nombre[MAX_NAME];
    float glucosa;
    float colesterol;
    float trigliceridos;
} Analito;

// Prototipos de funciones
void registrarPaciente();
void mostrarRegistros();
void buscarRegistro();
void analizarPromedios();
void generarArchivoBinario();
void mostrarArchivoBinario();

int main() {
    int opcion;
    do {
        printf("\n--- Gestion de Analitos de la Salud ---\n");
        printf("1. Registrar datos de un paciente\n");
        printf("2. Mostrar todos los registros\n");
        printf("3. Buscar un registro por nombre\n");
        printf("4. Analizar promedios de analitos\n");
        printf("5. Generar archivo binario\n");
        printf("6. Mostrar datos del archivo binario\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrarPaciente();
                break;
            case 2:
                mostrarRegistros();
                break;
            case 3:
                buscarRegistro();
                break;
            case 4:
                analizarPromedios();
                break;
            case 5:
                generarArchivoBinario();
                break;
            case 6:
                mostrarArchivoBinario();
                break;
            case 7:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 7);

    return 0;
}

// Funcion para registrar datos de un paciente
void registrarPaciente() {
    FILE *archivo = fopen(TEXT_FILE, "a");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    Analito nuevo;
    printf("Ingrese el nombre del paciente: ");
    scanf("%s", nuevo.nombre);
    printf("Ingrese nivel de glucosa: ");
    scanf("%f", &nuevo.glucosa);
    printf("Ingrese nivel de colesterol: ");
    scanf("%f", &nuevo.colesterol);
    printf("Ingrese nivel de trigliceridos: ");
    scanf("%f", &nuevo.trigliceridos);

    fprintf(archivo, "%s %.2f %.2f %.2f\n", nuevo.nombre, nuevo.glucosa, nuevo.colesterol, nuevo.trigliceridos);
    fclose(archivo);
    printf("Datos registrados con exito.\n");
}

// Funcion para mostrar todos los registros
void mostrarRegistros() {
    FILE *archivo = fopen(TEXT_FILE, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    Analito paciente;
    printf("\n--- Registros de Analitos ---\n");
    while (fscanf(archivo, "%s %f %f %f", paciente.nombre, &paciente.glucosa, &paciente.colesterol, &paciente.trigliceridos) != EOF) {
        printf("Nombre: %s, Glucosa: %.2f, Colesterol: %.2f, Trigliceridos: %.2f\n", 
                paciente.nombre, paciente.glucosa, paciente.colesterol, paciente.trigliceridos);
    }

    fclose(archivo);
}

// Funcion para buscar un registro por nombre
void buscarRegistro() {
    FILE *archivo = fopen(TEXT_FILE, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char nombreBuscado[MAX_NAME];
    printf("Ingrese el nombre del paciente a buscar: ");
    scanf("%s", nombreBuscado);

    Analito paciente;
    int encontrado = 0;
    while (fscanf(archivo, "%s %f %f %f", paciente.nombre, &paciente.glucosa, &paciente.colesterol, &paciente.trigliceridos) != EOF) {
        if (strcmp(paciente.nombre, nombreBuscado) == 0) {
            printf("\n--- Registro Encontrado ---\n");
            printf("Nombre: %s, Glucosa: %.2f, Colesterol: %.2f, Trigliceridos: %.2f\n", 
                    paciente.nombre, paciente.glucosa, paciente.colesterol, paciente.trigliceridos);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontro un registro con el nombre '%s'.\n", nombreBuscado);
    }

    fclose(archivo);
}

// Funcion para analizar promedios de analitos
void analizarPromedios() {
    FILE *archivo = fopen(TEXT_FILE, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    int contador = 0;
    float sumaGlucosa = 0, sumaColesterol = 0, sumaTrigliceridos = 0;
    Analito paciente;

    while (fscanf(archivo, "%s %f %f %f", paciente.nombre, &paciente.glucosa, &paciente.colesterol, &paciente.trigliceridos) != EOF) {
        sumaGlucosa += paciente.glucosa;
        sumaColesterol += paciente.colesterol;
        sumaTrigliceridos += paciente.trigliceridos;
        contador++;
    }

    if (contador == 0) {
        printf("No hay datos para analizar.\n");
    } else {
        printf("\n--- Analisis de Promedios ---\n");
        printf("Glucosa promedio: %.2f\n", sumaGlucosa / contador);
        printf("Colesterol promedio: %.2f\n", sumaColesterol / contador);
        printf("Trigliceridos promedio: %.2f\n", sumaTrigliceridos / contador);
    }

    fclose(archivo);
}

// Funcion para generar archivo binario
void generarArchivoBinario() {
    FILE *archivoTexto = fopen(TEXT_FILE, "r");
    FILE *archivoBin = fopen(BIN_FILE, "wb");
    if (archivoTexto == NULL || archivoBin == NULL) {
        perror("Error al abrir los archivos");
        return;
    }

    Analito paciente;
    while (fscanf(archivoTexto, "%s %f %f %f", paciente.nombre, &paciente.glucosa, &paciente.colesterol, &paciente.trigliceridos) != EOF) {
        fwrite(&paciente, sizeof(Analito), 1, archivoBin);
    }

    fclose(archivoTexto);
    fclose(archivoBin);
    printf("Archivo binario generado con exito.\n");
}

// Funcion para mostrar los datos del archivo binario
void mostrarArchivoBinario() {
    FILE *archivoBin = fopen(BIN_FILE, "rb");
    if (archivoBin == NULL) {
        perror("Error al abrir el archivo binario");
        return;
    }

    Analito paciente;
    printf("\n--- Datos del Archivo Binario ---\n");
    while (fread(&paciente, sizeof(Analito), 1, archivoBin)) {
        printf("Nombre: %s, Glucosa: %.2f, Colesterol: %.2f, Trigliceridos: %.2f\n", 
                paciente.nombre, paciente.glucosa, paciente.colesterol, paciente.trigliceridos);
    }

    fclose(archivoBin);
}

