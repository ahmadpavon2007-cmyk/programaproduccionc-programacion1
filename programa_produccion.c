#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 5

// Prototipos de funciones
void mostrarMenu();
void ingresarProductos(char nombres[][50], int demandas[], float tiempos[], float recursos[], int *n);
void mostrarProductos(char nombres[][50], int demandas[], float tiempos[], float recursos[], int n);
void calcularProduccion(char nombres[][50], int demandas[], float tiempos[], float recursos[], int n);
int buscarProducto(char nombres[][50], int n, char buscador[]);
void editarProducto(char nombres[][50], int demandas[], float tiempos[], float recursos[], int n);
void eliminarProducto(char nombres[][50], int demandas[], float tiempos[], float recursos[], int *n);

int main() {
    char nombres[MAX_PRODUCTOS][50];
    int demandas[MAX_PRODUCTOS];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    int n = 0; // Cantidad actual de productos registrados
    int opcion;

    do {
        mostrarMenu();
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada invalida. Intente de nuevo.\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        while (getchar() != '\n'); // Limpiar buffer de salto de línea

        switch (opcion) {
            case 1:
                ingresarProductos(nombres, demandas, tiempos, recursos, &n);
                break;
            case 2:
                mostrarProductos(nombres, demandas, tiempos, recursos, n);
                break;
            case 3:
                calcularProduccion(nombres, demandas, tiempos, recursos, n);
                break;
            case 4: {
                char buscador[50];
                printf("Ingrese el nombre del producto a buscar: ");
                fgets(buscador, sizeof(buscador), stdin);
                buscador[strcspn(buscador, "\n")] = 0; // Remover salto de línea
                int idx = buscarProducto(nombres, n, buscador);
                if (idx != -1) {
                    printf("¡Producto encontrado en la posicion %d!\n", idx + 1);
                    printf("- Nombre: %s\n- Demanda: %d\n- Tiempo Unitario: %.2f\n- Recurso Unitario: %.2f\n",
                           nombres[idx], demandas[idx], tiempos[idx], recursos[idx]);
                } else {
                    printf("Producto no encontrado.\n");
                }
                break;
            }
            case 5:
                editarProducto(nombres, demandas, tiempos, recursos, n);
                break;
            case 6:
                eliminarProducto(nombres, demandas, tiempos, recursos, &n);
                break;
            case 7:
                printf("Saliendo del sistema de optimizacion...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
        printf("\n");
    } while (opcion != 7);

    return 0;
}

void mostrarMenu() {
    printf("==================================================\n");
    printf("   SISTEMA DE OPTIMIZACION DE FABRICA (SOCPE)     \n");
    printf("==================================================\n");
    printf("1. Ingresar / Registrar productos (Max 5)\n");
    printf("2. Mostrar listado de productos\n");
    printf("3. Calcular viabilidad de produccion y recursos\n");
    printf("4. Buscar producto\n");
    printf("5. Editar producto\n");
    printf("6. Eliminar producto\n");
    printf("7. Salir\n");
    printf("==================================================\n");
}

void ingresarProductos(char nombres[][50], int demandas[], float tiempos[], float recursos[], int *n) {
    if (*n >= MAX_PRODUCTOS) {
        printf("Capacidad maxima alcanzada (%d productos).\n", MAX_PRODUCTOS);
        return;
    }

    int cantidadAVerificar;
    printf("¿Cuantos productos desea registrar (Disponibles: %d)? ", MAX_PRODUCTOS - *n);
    if (scanf("%d", &cantidadAVerificar) != 1 || cantidadAVerificar <= 0 || (*n + cantidadAVerificar) > MAX_PRODUCTOS) {
        printf("Cantidad invalida o excede el limite permitido.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < cantidadAVerificar; i++) {
        int idx = *n;
        printf("\n--- Producto %d ---\n", idx + 1);
        printf("Nombre del producto: ");
        fgets(nombres[idx], 50, stdin);
        nombres[idx][strcspn(nombres[idx], "\n")] = 0;

        printf("Cantidad demandada: ");
        scanf("%d", &demandas[idx]);

        printf("Tiempo de fabricacion por unidad (horas): ");
        scanf("%f", &tiempos[idx]);

        printf("Recursos necesarios por unidad: ");
        scanf("%f", &recursos[idx]);
        while (getchar() != '\n');

        (*n)++;
    }
    printf("¡Productos ingresados exitosamente!\n");
}

void mostrarProductos(char nombres[][50], int demandas[], float tiempos[], float recursos[], int n) {
    if (n == 0) {
        printf("No hay productos registrados en el sistema.\n");
        return;
    }

    printf("\n---------------- LISTADO DE PRODUCTOS ----------------\n");
    printf("%-5s | %-15s | %-8s | %-8s | %-8s\n", "No.", "Nombre", "Demanda", "TiempoU", "RecursoU");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d | %-15s | %-8d | %-8.2f | %-8.2f\n", 
               i + 1, nombres[i], demandas[i], tiempos[i], recursos[i]);
    }
    printf("------------------------------------------------------\n");
}

int buscarProducto(char nombres[][50], int n, char buscador[]) {
    for (int i = 0; i < n; i++) {
        if (strcmp(nombres[i], buscador) == 0) {
            return i; // Retorna el índice exacto
        }
    }
    return -1; // No encontrado
}

void calcularProduccion(char nombres[][50], int demandas[], float tiempos[], float recursos[], int n) {
    if (n == 0) {
        printf("No hay productos para calcular.\n");
        return;
    }

    float tiempoTotalReq = 0;
    float recursoTotalReq = 0;

    for (int i = 0; i < n; i++) {
        tiempoTotalReq += (float)demandas[i] * tiempos[i];
        recursoTotalReq += (float)demandas[i] * recursos[i];
    }

    float tiempoDisponiblesPlanta, recursosDisponiblesPlanta;
    printf("Ingrese el tiempo total disponible en la planta (horas): ");
    scanf("%f", &tiempoDisponiblesPlanta);
    printf("Ingrese la cantidad total de recursos disponibles en la planta: ");
    scanf("%f", &recursosDisponiblesPlanta);
    while (getchar() != '\n');

    printf("\n============== REPORTE DE VIABILIDAD ==============\n");
    printf("Tiempo total requerido: %.2f horas\n", tiempoTotalReq);
    printf("Recursos totales requeridos: %.2f\n", recursoTotalReq);
    printf("--------------------------------------------------\n");

    if (tiempoTotalReq <= tiempoDisponiblesPlanta && recursoTotalReq <= recursosDisponiblesPlanta) {
        printf("ESTADO: LA FABRICA PUEDE CUMPLIR CON LA DEMANDA.\n");
    } else {
        printf("ESTADO: LA FABRICA NO PUEDE CUMPLIR CON LA DEMANDA.\n");
        if (tiempoTotalReq > tiempoDisponiblesPlanta) {
            printf("- Alerta: Exceso de tiempo requerido de %.2f horas.\n", tiempoTotalReq - tiempoDisponiblesPlanta);
        }
        if (recursoTotalReq > recursosDisponiblesPlanta) {
            printf("- Alerta: Exceso de recursos requeridos de %.2f unidades.\n", recursoTotalReq - recursosDisponiblesPlanta);
        }
    }
    printf("==================================================\n");
}

void editarProducto(char nombres[][50], int demandas[], float tiempos[], float recursos[], int n) {
    if (n == 0) {
        printf("No hay productos para editar.\n");
        return;
    }

    char buscador[50];
    printf("Ingrese el nombre del producto que desea editar: ");
    fgets(buscador, sizeof(buscador), stdin);
    buscador[strcspn(buscador, "\n")] = 0;

    int idx = buscarProducto(nombres, n, buscador);
    if (idx == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    printf("Producto encontrado. Ingrese los nuevos datos:\n");
    printf("Nuevo nombre: ");
    fgets(nombres[idx], 50, stdin);
    nombres[idx][strcspn(nombres[idx], "\n")] = 0;

    printf("Nueva demanda: ");
    scanf("%d", &demandas[idx]);

    printf("Nuevo tiempo unitario: ");
    scanf("%f", &tiempos[idx]);

    printf("Nuevo recurso unitario: ");
    scanf("%f", &recursos[idx]);
    while (getchar() != '\n');

    printf("¡Producto actualizado exitosamente!\n");
}

void eliminarProducto(char nombres[][50], int demandas[], float tiempos[], float recursos[], int *n) {
    if (*n == 0) {
        printf("No hay productos para eliminar.\n");
        return;
    }

    char buscador[50];
    printf("Ingrese el nombre del producto que desea eliminar: ");
    fgets(buscador, sizeof(buscador), stdin);
    buscador[strcspn(buscador, "\n")] = 0;

    int idx = buscarProducto(nombres, *n, buscador);
    if (idx == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    // Desplazamiento de elementos para eliminar físicamente del arreglo paralelo
    for (int i = idx; i < *n - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        demandas[i] = demandas[i + 1];
        tiempos[i] = tiempos[i + 1];
        recursos[i] = recursos[i + 1];
    }

    (*n)--;
    printf("¡Producto eliminado exitosamente del sistema!\n");
}
