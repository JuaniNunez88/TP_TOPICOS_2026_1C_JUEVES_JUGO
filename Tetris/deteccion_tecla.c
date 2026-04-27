#include "deteccion_tecla.h"
#include "GBT/gbt.h"
int izq, der, abajo;

void input_actualizar()
{
    izq = gbt_tecla_presionada(GBTK_IZQUIERDA);
    abajo = gbt_tecla_presionada(GBTK_ABAJO);
    der = gbt_tecla_presionada(GBTK_DERECHA);
}

int input_abajo()
{
    return abajo;
}
int input_izquierda()
{
    return izq;
}
int input_derecha()
{
    return der;
}
