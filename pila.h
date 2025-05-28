#ifndef PILA_H
#define PILA_H

#include "mapa.h"
#include "nivel.h"

typedef struct {
    Coordenada coord; // (No se utiliza en este ejemplo, pero se dejó para compatibilidad)
    int indice;       // Indice en el arreglo de posiciones válidas
} Estado;

typedef struct {
    Estado *datos;    // Arreglo dinámico de estados
    int tope;         // Índice del último estado agregado
    int capacidad;    // Capacidad de la pila
} Pila;

Pila* pila_crear(int capacidad);
void pila_destruir(Pila*);
void pila_apilar(Pila*, Estado);
Estado pila_desapilar(Pila*);
Estado pila_tope(Pila*);
int pila_esta_vacia(Pila*);

#endif


