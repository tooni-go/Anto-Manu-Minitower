#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

Pila* pila_crear(int capacidad) {
    Pila* pila = malloc(sizeof(Pila));
    if (!pila) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la pila.\n");
        exit(EXIT_FAILURE);
    }
    pila->datos = malloc(capacidad * sizeof(Estado));
    if (!pila->datos) {
        free(pila);
        fprintf(stderr, "Error: No se pudo asignar memoria para los datos de la pila.\n");
        exit(EXIT_FAILURE);
    }
    pila->tope = -1;
    pila->capacidad = capacidad;
    return pila;
}

void pila_destruir(Pila* pila) {
    if (pila) {
        free(pila->datos);
        free(pila);
    }
}

void pila_apilar(Pila* pila, Estado estado) {
    if (pila->tope + 1 >= pila->capacidad) {  // Expandir si es necesario
        int nueva_capacidad = pila->capacidad * 2;
        Estado* nuevo_datos = malloc(nueva_capacidad * sizeof(Estado));
        if (!nuevo_datos) {
            fprintf(stderr, "Error: No se pudo expandir la pila.\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < pila->capacidad; i++) {
            nuevo_datos[i] = pila->datos[i];
        }
        free(pila->datos);
        pila->datos = nuevo_datos;
        pila->capacidad = nueva_capacidad;
    }
    pila->tope++;
    pila->datos[pila->tope] = estado;
}

Estado pila_desapilar(Pila* pila) {
    if (pila_esta_vacia(pila)) {
        fprintf(stderr, "Error: Intento de desapilar una pila vacía.\n");
        exit(EXIT_FAILURE);
    }
    Estado estado = pila->datos[pila->tope];
    pila->tope--;
    return estado;
}

Estado pila_tope(Pila* pila) {
    if (pila_esta_vacia(pila)) {
        fprintf(stderr, "Error: Intento de acceder a la cima de una pila vacía.\n");
        exit(EXIT_FAILURE);
    }
    return pila->datos[pila->tope];
}

int pila_esta_vacia(Pila* pila) {
    return pila->tope == -1;
}

