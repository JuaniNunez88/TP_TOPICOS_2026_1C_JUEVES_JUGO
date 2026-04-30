/* SILENCIADO 01.39
#include "dibuja.h"
#include "tablero.h"
#include "logica.h"
#include "GBT/gbt.h"
#include "piezas.h"

#define TAM 10




//Dibuja el bloque cayendo
static void dibujar_rectangulo_lleno(int x, int y, int ancho, int alto, int color, t_pieza *pieza_act) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            //gbt_dibujar_pixel(x + j, y + i, color);
            if( pieza_act->tamano[i][j] == 1 ){// buscar 1 de matriz    // AGREGADO
                printf("Dibujando bloque en coordenada X=%d, Y=%d\n",x+j,y+i);
                gbt_dibujar_pixel( x+j , y+i ,pieza_act->c);  // AGREGADO
             }                                              // AGREGADO
            gbt_dibujar_pixel( x+j, y+i , pieza_act->c);
        }
    }
}

void render_dibujar() {

    gbt_borrar_backbuffer(0);

    //DIBUJAR TABLERO (bloques ya fijados)
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero_get(i, j)) {
                t_pieza *piezaElegida = obtener_matriz_pieza;
                dibujar_rectangulo_lleno(j*TAM, i*TAM, TAM, TAM, 2,&piezaElegida); //Color en que se convierte al quedar fijo
            }
        }
    }

    // DIBUJAR PIEZA ACTUAL (la que cae)
    //dibujar_rectangulo_lleno(pieza_x*TAM, pieza_y*TAM, TAM, TAM, 4); // Color cuando aparece
    pieza_nueva();

    //Muestra todo por pantalla
    gbt_volcar_backbuffer();
}

void pieza_nueva()
{
    // DIBUJAR PIEZA ACTUAL (la que cae)
    //dibujar_rectangulo_lleno(pieza_x*TAM, pieza_y*TAM, TAM, TAM, 4); // Color cuando aparece
    t_pieza *piezaElegida = obtener_matriz_pieza;
    dibujar_rectangulo_lleno(pieza_x*TAM, pieza_y*TAM, 4, 4, 4, &piezaElegida);

}

*/
#include "dibuja.h"
#include "tablero.h"
#include "logica.h"
#include "GBT/gbt.h"
#include "piezas.h"

#define TAM 10

// 1. Dibuja un cuadrado relleno de TAMxTAM píxeles
static void dibujar_rectangulo_lleno(int x, int y, int color) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            // Dibuja pixel por pixel hasta armar el bloque de 10x10
            gbt_dibujar_pixel(x + j, y + i, color);
        }
    }
}

// 2. FUNCIÓN PARA DIBUJAR LA PIEZA QUE CAE
void pieza_nueva() {
    // Obtenemos la pieza (CON los paréntesis)
    t_pieza *piezaElegida = obtener_matriz_pieza();

    // Recorremos la matriz 4x4 de la pieza
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            // Si hay un 1, mandamos a dibujar un bloque relleno de 10x10
            if (piezaElegida->tamano[i][j] == 1) {

                // Calculamos dónde va el bloque.
                // Multiplicamos por TAM para pasarlo a píxeles de pantalla.
                int pos_x_real = (pieza_x + j) * TAM;
                int pos_y_real = (pieza_y + i) * TAM;

                // Dibujamos el bloque usando el color de la pieza
                dibujar_rectangulo_lleno(pos_x_real, pos_y_real, piezaElegida->c);
            }
        }
    }
}

// 3. LA FUNCIÓN PRINCIPAL DE RENDER
void render_dibujar() {
    gbt_borrar_backbuffer(0);

    // DIBUJAR TABLERO (bloques ya fijados)
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            // Si hay un bloque fijado en el tablero
            if (tablero_get(i, j)) {
                // Dibujamos un rectángulo en su posición (color 2 fijo)
                dibujar_rectangulo_lleno(j * TAM, i * TAM, 2);
            }
        }
    }

    // DIBUJAR PIEZA ACTUAL (la que cae)
    // Acá llamamos a tu función que se encarga de recorrer la matriz 4x4 y dibujarla
    pieza_nueva();

    // Muestra todo por pantalla
    gbt_volcar_backbuffer();
}
