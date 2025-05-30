#include "estrategia.h"
#include "pila.c"
#include "simulador.h"
#include "mapa.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static int posiciones_validas(Coordenada *validas, TipoCasilla **casillas, int alto, int ancho) {
    int cant_posiciones_validas = 0;
    
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            if (casillas[i][j] == VACIO) {
                validas[cant_posiciones_validas].x = i;
                validas[cant_posiciones_validas++].y = j;
            }
        }
    }

    return cant_posiciones_validas;
}

static void colocar_torre(Mapa *mapa, int x, int y, int nro_torre) {
    // actualizar torre
    mapa->torres[nro_torre].x = x;
    mapa->torres[nro_torre].y = y;

    // actualizar mapa
    mapa->casillas[x][y] = TORRE;
}

static int determinar_posicion_torre(int *casilla_elegida, int cant_validas) {
    int nueva_posicion = rand() % cant_validas;
    while(casilla_elegida[nueva_posicion])
        nueva_posicion = rand() % cant_validas;

    return nueva_posicion;
}

void disponer(Nivel* nivel, Mapa* mapa) {
    int cantidad_casillas = mapa->alto * mapa->ancho;
    Coordenada posiciones_validas_torre[cantidad_casillas];
    int casilla_elegida[cantidad_casillas];
    for(int i = 0; i < cantidad_casillas; casilla_elegida[i++] = 0);

    int cant_validas = posiciones_validas(posiciones_validas_torre, mapa->casillas, mapa->alto, mapa->ancho);

    for (int colocadas = 0; colocadas < mapa->cant_torres; colocadas++) {
        int nueva_torre = determinar_posicion_torre(casilla_elegida, cant_validas);
        casilla_elegida[nueva_torre] = 1;
        int nueva_torre_x = posiciones_validas_torre[nueva_torre].x;
        int nueva_torre_y = posiciones_validas_torre[nueva_torre].y;
        colocar_torre(mapa, nueva_torre_x, nueva_torre_y, colocadas);
    }
}

int encontrar_soluciones(Nivel* nivel, Mapa* mapa, Coordenada* posiciones_validas, int cant_validas, int torres_colocadas, int max_torres, int i_inicio) { 
    // Caso base: solución válida
    if (torres_colocadas == max_torres) {
        if (simular_nivel(nivel, mapa, NULL)) {
            printf("Configuración válida encontrada:\n");
            mostrar_mapa(mapa, nivel->enemigos);
            return 1; // Solución encontrada
        }
        return 0; // Solución no encontrada
    }

    // Explorar todas las opciones desde `i_inicio` para evitar redundancias
    for (int i = i_inicio; i < cant_validas; i++) {
        Coordenada pos = posiciones_validas[i];

        // Validar que la casilla esté vacía
        if (mapa->casillas[pos.x][pos.y] == VACIO) {
            // Aplicar la elección
            colocar_torre(mapa, pos.x, pos.y, torres_colocadas);

            // Recursión con siguiente torre
            if (encontrar_soluciones(nivel, mapa, posiciones_validas, cant_validas, torres_colocadas + 1, max_torres, i + 1)) {
                return 1; // Solución encontrada en una rama
            }

            // Retroceso
            mapa->casillas[pos.x][pos.y] = VACIO;
        }
    }

    return 0; // No se encontró solución en esta rama
}

void disponer_con_backtracking(Nivel* nivel, Mapa* mapa) {
    // Determinar las posiciones válidas
    int total_casillas = mapa->alto * mapa->ancho;
    Coordenada posiciones_validas_torre[total_casillas];
    int cant_validas = posiciones_validas(posiciones_validas_torre, mapa->casillas, mapa->alto, mapa->ancho);

    if (cant_validas <= 0) {
        printf("Error: No hay posiciones válidas para torres.\n");
        return;
    }

    // Iniciar backtracking con índice inicial `0`
    if (!encontrar_soluciones(nivel, mapa, posiciones_validas_torre, cant_validas, 0, mapa->cant_torres, 0)) {
        printf("No se encontró una configuración válida.\n");
    }
}



void disponer_custom(Nivel* nivel, Mapa* mapa) {
    int cantidad_casillas = mapa->alto * mapa->ancho;
    Coordenada posiciones_validas_torre[cantidad_casillas];

    int cant_validas = posiciones_validas(posiciones_validas_torre, mapa->casillas, mapa->alto, mapa->ancho);
    for (int colocadas = 0; colocadas < mapa->cant_torres; ) {
        mostrar_cords_mapa(mapa, nivel->enemigos);
        int cordenada_x, cordenada_y;
        printf("Coloca la torre %d en cualquier posicion alrededor del camino:\n", colocadas + 1);
        printf("Ingresa coordenada X: ");
        scanf("%d", &cordenada_y);
        printf("Ingresa coordenada Y: ");
        scanf("%d", &cordenada_x);
        if (cordenada_x < 0 || cordenada_x > mapa->ancho || cordenada_y < 0 || cordenada_y > mapa->alto) {
            printf("Coordenada fuera de los límites del mapa. Intenta de nuevo.\n\n");
            continue;
        }
        int es_valida = 0;
        for (int i = 0; i < cant_validas; i++) {
            if (posiciones_validas_torre[i].x == cordenada_x && posiciones_validas_torre[i].y == cordenada_y) {
                es_valida = 1;
                break;
            }
        }
        if (!es_valida) {
            printf("No se puede colocar una torre en esa casilla. Intenta de nuevo.\n\n");
            continue;
        }
        colocar_torre(mapa, cordenada_x, cordenada_y, colocadas);
        colocadas++;
        printf("Torre colocada en (%d, %d).\n\n", cordenada_x, cordenada_y);
    }
    return;
}

