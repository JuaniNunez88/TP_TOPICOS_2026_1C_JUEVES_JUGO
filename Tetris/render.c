#include "render.h"
#include "tablero.h"
#include "juego.h"
#include "GBT/gbt.h"

#define TAM 20

// --------------------
// FUNCIONES INTERNAS
// --------------------

static void dibujar_rectangulo_lleno(int x, int y, int ancho, int alto, int color) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            gbt_dibujar_pixel(x + j, y + i, color);
        }
    }
}

static void dibujar_rectangulo(int x, int y, int ancho, int alto, int color) {

    // arriba y abajo
    for (int i = 0; i < ancho; i++) {
        gbt_dibujar_pixel(x + i, y, color);
        gbt_dibujar_pixel(x + i, y + alto - 1, color);
    }

    // lados
    for (int i = 0; i < alto; i++) {
        gbt_dibujar_pixel(x, y + i, color);
        gbt_dibujar_pixel(x + ancho - 1, y + i, color);
    }
}

// --------------------
// RENDER PRINCIPAL
// --------------------

void render_dibujar() {

    gbt_borrar_backbuffer(0);

    // dibujar grilla
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            dibujar_rectangulo(j*TAM, i*TAM, TAM, TAM, 1);
        }
    }

    // dibujar pieza (cuadrado)
    dibujar_rectangulo_lleno(pieza_x*TAM, pieza_y*TAM, TAM, TAM, 5);

    gbt_volcar_backbuffer();
}
