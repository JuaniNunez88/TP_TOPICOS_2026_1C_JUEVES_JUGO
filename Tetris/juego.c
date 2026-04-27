#include "juego.h"
#include "deteccion_tecla.h"

int pieza_x;
int pieza_y;

int timer_caida = 0;

void juego_iniciar() {
    pieza_x = 4;
    pieza_y = 0;
}

void juego_actualizar() {

    // mover lateral
    if (input_izquierda()) pieza_x--;
    if (input_derecha()) pieza_x++;

    // bajar manual
    if (input_abajo()) pieza_y++;

    // caída automática
    timer_caida++;
    if (timer_caida > 30) { // velocidad inicial
        pieza_y++;
        timer_caida = 0;
    }
}
