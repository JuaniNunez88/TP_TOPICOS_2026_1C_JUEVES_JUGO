#include "deteccion_tecla.h"
#include "GBT/gbt.h"

static int izq, der, abajo,arriba;

void input_actualizar() {
    //arriba = tecla presionada, no sostenida
    //agregados: la p puede ser pausa
    izq = gbt_tecla_sostenida(GBTK_IZQUIERDA);
    der = gbt_tecla_sostenida(GBTK_DERECHA);
    abajo = gbt_tecla_sostenida(GBTK_ABAJO);
    arriba = gbt_tecla_presionada(GBTK_ARRIBA);
}

int input_izquierda() { return izq; }
int input_derecha() { return der; }
int input_abajo() { return abajo; }
int input_arriba() {return arriba; }
