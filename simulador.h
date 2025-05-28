#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "mapa.h"
#include "estrategia.h"
#include "archivo.h"
#include "turno.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int simular_nivel(Nivel *nivel, Mapa *mapa, DisposicionTorres colocar_torres);

#endif