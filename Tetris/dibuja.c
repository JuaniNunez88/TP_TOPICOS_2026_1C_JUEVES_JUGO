#include "dibuja.h"
#include "tablero.h"
#include "logica.h"
#include "GBT/gbt.h"

#define TAM 10
//Dibuja el bloque cayendo
static void dibujar_rectangulo_lleno(int x, int y, int ancho, int alto, int color) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            gbt_dibujar_pixel(x + j, y + i, color);
        }
    }
}

void render_dibujar() {

    gbt_borrar_backbuffer(0);

    //DIBUJAR TABLERO (bloques ya fijados)
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero_get(i, j)) {
                dibujar_rectangulo_lleno(j*TAM, i*TAM, TAM, TAM, 2);
            }
        }
    }

    // DIBUJAR PIEZA ACTUAL (la que cae)
    dibujar_rectangulo_lleno(pieza_x*TAM, pieza_y*TAM, TAM, TAM, 4);

    //Muestra todo por pantalla
    gbt_volcar_backbuffer();
}

