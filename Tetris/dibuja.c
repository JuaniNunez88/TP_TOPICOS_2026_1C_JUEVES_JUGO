#include "dibuja.h"
#include "tablero.h"
#include "logica.h"
#include "GBT/gbt.h"

#define TAM 10

// ------------------------
// DIBUJAR RECTÁNGULO
// ------------------------
static void dibujar_rectangulo_lleno(int x, int y, int ancho, int alto, int color) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            gbt_dibujar_pixel(x + j, y + i, color);
        }
    }
}

// ------------------------
// MENU
// ------------------------
void render_menu() {
    // prueba visible
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            gbt_dibujar_pixel(50 + j, 50 + i, 4);
        }
    }

    dibujar_rectangulo_lleno(100, 80, 120, 20, 5);
    dibujar_rectangulo_lleno(100, 120, 120, 10, 3);
}

// ------------------------
// JUEGO
// ------------------------
void render_juego() {

    // tablero
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero_get(i, j)) {
                dibujar_rectangulo_lleno(j*TAM, i*TAM, TAM, TAM, 2);
            }
        }
    }

    // pieza
    t_pieza p = juego_get_pieza();
    dibujar_rectangulo_lleno(p.x*TAM, p.y*TAM, TAM, TAM, 4);
}

// ------------------------
// PAUSA
// ------------------------
void render_pausa() {
    render_juego();
    dibujar_rectangulo_lleno(80, 100, 160, 20, 4);
}

// ------------------------
// GAME OVER
// ------------------------
void render_gameover() {
    dibujar_rectangulo_lleno(80, 100, 160, 20, 4);
}

// ------------------------
// RENDER PRINCIPAL
// ------------------------
void render_dibujar() {

    gbt_borrar_backbuffer(0);

    switch (juego_get_estado()) {

        case ESTADO_MENU:
            render_menu();
        break;

        case ESTADO_JUGANDO:
            render_juego();
        break;

        case ESTADO_PAUSA:
            render_pausa();
        break;

        case ESTADO_GAMEOVER:
            render_gameover();
        break;
    }

    gbt_volcar_backbuffer();
}
