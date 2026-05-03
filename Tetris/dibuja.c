#include "dibuja.h"
#include "tablero.h"
#include "GBT/gbt.h"

#define TAM   10  // píxeles por celda
#define TAM_M  4  // tamaño matriz de pieza

static void dibujar_bloque(int x, int y, int color)
{
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            gbt_dibujar_pixel(x + j, y + i, color);
}

static void dibujar_pieza(t_estado_juego *eg)
{
    for (int i = 0; i < TAM_M; i++)
        for (int j = 0; j < TAM_M; j++)
            if (eg->pieza_actual->tamano[i][j] == 1)
                dibujar_bloque((eg->pieza_x + j) * TAM,
                               (eg->pieza_y + i) * TAM,
                               eg->pieza_actual->c);
}

static void dibujar_tablero()
{
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
        {
            int val = tablero_get(i, j);
            if (val != 0)
                dibujar_bloque(j * TAM, i * TAM, val);
        }
}

static void render_menu()
{
    for (int i = 0; i < 40; i++)
        for (int j = 0; j < 200; j++)
            gbt_dibujar_pixel(60 + j, 80 + i, 4);

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 140; j++)
            gbt_dibujar_pixel(90 + j, 160 + i, 3);
}

static void render_juego(t_estado_juego *eg)
{
    dibujar_tablero();
    dibujar_pieza(eg);
}

static void render_pausa(t_estado_juego *eg)
{
    render_juego(eg); // tablero detrás
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 160; j++)
            gbt_dibujar_pixel(80 + j, 100 + i, 4);
}

static void render_gameover()
{
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 160; j++)
            gbt_dibujar_pixel(80 + j, 100 + i, 4);
}

void render_dibujar(t_estado_juego *eg)
{
    gbt_borrar_backbuffer(0);

    switch (eg->estado)
    {
        case ESTADO_MENU:     render_menu();        break;
        case ESTADO_JUGANDO:  render_juego(eg);     break;
        case ESTADO_PAUSA:    render_pausa(eg);     break;
        case ESTADO_GAMEOVER: render_gameover();    break;
    }

    gbt_volcar_backbuffer();
}
