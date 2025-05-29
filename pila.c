#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

Pila* pila_crear(int capacidad) {
    Pila* pila = malloc(sizeof(Pila));
    if (!pila) {
        return NULL; // Si falla, simplemente devuelve NULL
    }
    pila->datos = malloc(capacidad * sizeof(Estado));
    if (!pila->datos) {
        free(pila); // Libera la pila si la segunda asignación falla
        return NULL;
    }
    pila->tope = -1;
    pila->capacidad = capacidad;
    return pila;
}

void pila_destruir(Pila* pila) {
    if (pila) {
        if (pila->datos) {
            free(pila->datos);
        }
        free(pila);
    }
}

void pila_apilar(Pila* pila, Estado estado) {
    if (pila->tope + 1 >= pila->capacidad) {  // Expandir si es necesario
        int nueva_capacidad = pila->capacidad * 2;
        Estado* nuevo_datos = malloc(nueva_capacidad * sizeof(Estado));

        if (!nuevo_datos) {
            return; // No expande si no hay memoria disponible
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
        Estado estado_vacio = {0}; // Devuelve un estado vacío en lugar de terminar el programa
        return estado_vacio;
    }
    return pila->datos[pila->tope--];
}

Estado pila_tope(Pila* pila) {
    if (pila_esta_vacia(pila)) {
        Estado estado_vacio = {0}; // Devuelve un estado vacío si la pila está vacía
        return estado_vacio;
    }
    return pila->datos[pila->tope];
}

int pila_esta_vacia(Pila* pila) {
    return pila->tope == -1;
}
