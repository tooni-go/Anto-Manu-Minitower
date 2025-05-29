#include "estrategia.h"
#include "pila.c"
#include "simulador.h"
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

void disponer_con_backtracking(Nivel* nivel_original, Mapa* mapa_original) {
    int totalCasillas = mapa_original->alto * mapa_original->ancho;
    Coordenada posValidas[totalCasillas];
    int cantValidas = posiciones_validas(posValidas, mapa_original->casillas, mapa_original->alto, mapa_original->ancho);

    // Crear la pila con capacidad para todas las torres permitidas
    Pila* pila = pila_crear(mapa_original->cant_torres);
    int torres_colocadas = 0; // Contador de torres colocadas

    // Copiar el estado inicial del nivel y el mapa
    Nivel nivel_copia = *nivel_original;
    Mapa mapa_copia = *mapa_original;

    // Inicializar la pila con un estado vacío
    Estado inicial = {.indice = -1};
    pila_apilar(pila, inicial);

    while (!pila_esta_vacia(pila)) {
        Estado actual = pila_desapilar(pila);

        // Intentar avanzar al siguiente índice
        actual.indice++;
        if (actual.indice < cantValidas) {
            Coordenada pos = posValidas[actual.indice];
            colocar_torre(&mapa_copia, pos.x, pos.y, torres_colocadas);
            pila_apilar(pila, actual);
            torres_colocadas++;

            // Evaluar si colocamos todas las torres
            if (torres_colocadas == mapa_original->cant_torres) {
                // Restaurar el estado inicial antes de simular
                *nivel_original = nivel_copia;
                *mapa_original = mapa_copia;

                if (simular_nivel(nivel_original, mapa_original, NULL)) {
                    printf("Configuración válida encontrada:\n");
                    mostrar_mapa(mapa_original, nivel_original->enemigos);
                }

                // Quitar la última torre colocada para seguir explorando
                Coordenada ultima_pos = posValidas[actual.indice];
                mapa_copia.casillas[ultima_pos.x][ultima_pos.y] = VACIO;
                torres_colocadas--;
            } else {
                // Avanzar al siguiente nivel
                Estado siguiente = {.indice = -1};
                pila_apilar(pila, siguiente);
            }
        } else if (torres_colocadas > 0) {
            // Retroceso: quitar la última torre colocada
            Coordenada ultima_pos = posValidas[pila->datos[pila->tope].indice];
            mapa_copia.casillas[ultima_pos.x][ultima_pos.y] = VACIO;
            torres_colocadas--;
        }
    }

    pila_destruir(pila);
}

void disponer_custom(Nivel* nivel, Mapa* mapa) {
    int cantidad_casillas = mapa->alto * mapa->ancho;
    Coordenada posiciones_validas_torre[cantidad_casillas];
    int casilla_elegida[cantidad_casillas];
    for(int i = 0; i < cantidad_casillas; casilla_elegida[i++] = 0);

    int cant_validas = posiciones_validas(posiciones_validas_torre, mapa->casillas, mapa->alto, mapa->ancho);
    
    for (int colocadas = 0; colocadas < mapa->cant_torres; ) {
        int cordenada_x, cordenada_y;
        printf("Coloca la torre %d:\n", colocadas + 1);
        printf("Las cordenadas comienzan desde 0\n\n");
        printf("Ingresa coordenada X: ");
        scanf("%d", &cordenada_x);
        printf("Ingresa coordenada Y: ");
        scanf("%d", &cordenada_y);
        if (cordenada_x < 0 || cordenada_x >= mapa->ancho || cordenada_y < 0 || cordenada_y >= mapa->alto) {
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
}


