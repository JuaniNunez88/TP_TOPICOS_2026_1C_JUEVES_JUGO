#include "deteccion_tecla.h"
#include "GBT/gbt.h"

static int izq, der, abajo;

void input_actualizar() {
    //arriba = tecla presionada, no sostenida
    //agregados: la p puede ser pausa
    izq = gbt_tecla_sostenida(GBTK_IZQUIERDA);
    der = gbt_tecla_sostenida(GBTK_DERECHA);
    abajo = gbt_tecla_sostenida(GBTK_ABAJO);
}

int input_izquierda() { return izq; }
int input_derecha() { return der; }
int input_abajo() { return abajo; }
