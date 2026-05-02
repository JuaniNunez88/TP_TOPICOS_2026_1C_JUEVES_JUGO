#include "deteccion_tecla.h"
#include "GBT/gbt.h"

static int izq, der, abajo,enter, pausa;

void input_actualizar() {
    //arriba = tecla presionada, no sostenida
    enter = gbt_tecla_presionada(GBTK_ENTER);
    pausa = gbt_tecla_presionada(GBTK_ESCAPE);
    izq = gbt_tecla_sostenida(GBTK_IZQUIERDA);
    der = gbt_tecla_sostenida(GBTK_DERECHA);
    abajo = gbt_tecla_sostenida(GBTK_ABAJO);
}

int input_izquierda() { return izq; }
int input_derecha() { return der; }
int input_abajo() { return abajo; }
int input_enter() { return enter; }
int input_pausa() { return pausa; }
