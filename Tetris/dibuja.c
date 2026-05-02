
#include "dibuja.h"
#include "tablero.h"
#include "logica.h"
#include "GBT/gbt.h"
#include "piezas.h"

#define TAM 10
#define TAM_M 4

// Dibuja un cuadrado relleno de TAMxTAM píxeles
static void dibujar_rectangulo_lleno(int x, int y, int color) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            // Dibuja pixel por pixel hasta armar el bloque de 10x10
            gbt_dibujar_pixel(x + j, y + i, color);
        }
    }
}

// FUNCIÓN PARA DIBUJAR LA PIEZA QUE CAE
void pieza_nueva() {
    // Obtenemos la pieza
    t_pieza *piezaElegida = obtener_matriz_pieza();

    // Matriz cuadrada (definifido en TAM_M)
    for (int i = 0; i < TAM_M; i++) {
        for (int j = 0; j < TAM_M; j++) {

            // dibuja bloque relleno de 10x10
            if (piezaElegida->tamano[i][j] == 1) {

                // multiplica por TAM para pasarlo a píxeles de pantalla.
                int pos_x_real = (pieza_x + j) * TAM;
                int pos_y_real = (pieza_y + i) * TAM;

                // Dibujar bloque
                dibujar_rectangulo_lleno(pos_x_real, pos_y_real, piezaElegida->c);
            }
        }
    }
}


void render_dibujar() {
    gbt_borrar_backbuffer(0); // Limpia buffer

    // DIBUJAR TABLERO (bloques ya fijados)
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
            // Si hay un bloque fijado en el tablero
            if (tablero_get(i, j)) {
                // Dibuja un rectángulo en su posición (se establece el color verde fijo(2))
                dibujar_rectangulo_lleno(j * TAM, i * TAM, 2);
            }
        }
    }


    pieza_nueva(); // recorre matriz y dibuja

    // Muestra todo por pantalla: copia de los pixeles del backbuffer en la ventana
    gbt_volcar_backbuffer();
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
