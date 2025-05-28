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

void disponer_con_backtracking(Nivel* nivel, Mapa* mapa) {
	int totalCasillas = mapa->alto * mapa->ancho;
	Coordenada posValidas[totalCasillas];
	int cantValidas = posiciones_validas(posValidas, mapa->casillas, mapa->alto, mapa->ancho);

	// Crear la pila con capacidad para todas las casillas
	Pila* pila = pila_crear(totalCasillas);

	// Inicializar el primer nivel con índice -1 para que al incrementarlo se pruebe desde 0.
	Estado init;
	init.indice = -1;
	pila_apilar(pila, init);

	while (!pila_esta_vacia(pila)) {
    	// El nivel actual es igual a la cantidad de decisiones (torres) colocadas = (pila->tope + 1).
    	int nivelActual = pila->tope; // Índice de la última decisión en la pila.
   	 
    	// Desapila el estado del nivel actual y prueba el siguiente candidato.
    	Estado actual = pila_desapilar(pila);
    	actual.indice++;  // Probar la siguiente opción

    	// Asegurar que para niveles > 0 la elección sea mayor que la anterior (para evitar repeticiones)
    	if (nivelActual > 0) {
        	int indicePrevio = pila->datos[nivelActual - 1].indice;
        	if (actual.indice < indicePrevio + 1)
            	actual.indice = indicePrevio + 1;
    	}

    	if (actual.indice >= cantValidas) {
        	// No hay más candidatos en este nivel, así que retrocedemos.
        	if (nivelActual == 0)
            	break;  // Se han agotado todas las combinaciones.
       	 
        	// Eliminar la torre que había sido colocada en el nivel anterior.
        	int indiceRemovido = pila->datos[nivelActual - 1].indice;
        	Coordenada posRem = posValidas[indiceRemovido];
        	mapa->casillas[posRem.x][posRem.y] = VACIO;
        	// Continuamos (no empujamos ningún Estado nuevo) para probar el siguiente candidato en este nivel.
        	continue;
    	}
   	 
    	// Se encontró un candidato válido. Empujamos el estado actualizado para el nivel actual.
    	pila_apilar(pila, actual);
   	 
    	// Colocar provisionalmente la torre para este nivel.
    	Coordenada pos = posValidas[actual.indice];
    	colocar_torre(mapa, pos.x, pos.y, nivelActual);
   	 
    	// Si la cantidad de torres colocadas es la requerida, evaluamos la configuración.
    	if (pila->tope + 1 == mapa->cant_torres) {
        	if (simular_nivel(nivel, mapa, NULL)) {
            	printf("Configuracion valida encontrada:\n");
            	mostrar_mapa(mapa, nivel->enemigos);
            	break;
        	} else {
            	// La configuración fue rechazada: se retira la torre colocada y se sigue probando.
            	int indiceRemovido = pila->datos[pila->tope].indice;
            	Coordenada posRem = posValidas[indiceRemovido];
            	mapa->casillas[posRem.x][posRem.y] = VACIO;
            	pila_desapilar(pila);
        	}
    	} else {
        	// Aún faltan torres por colocar: iniciar el siguiente nivel con Estado inicial (-1)
        	Estado nuevo;
        	nuevo.indice = -1;
        	pila_apilar(pila, nuevo);
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


