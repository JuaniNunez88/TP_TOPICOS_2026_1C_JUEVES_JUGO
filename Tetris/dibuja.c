#include "dibuja.h"
#include "tablero.h"
#include "fuente.h"
#include "GBT/gbt.h"
#include <stdio.h>

#define TAM         20
#define TAM_M        4

#define TABLERO_X  220
#define TABLERO_Y   40

#define PANEL_X    450
#define PANEL_Y     40

#define COLOR_BORDE       8
#define COLOR_TITULO     15
#define COLOR_VALOR      14
#define COLOR_FONDO_PANEL 1

static void dibujar_bloque(int x, int y, int color)
{
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            gbt_dibujar_pixel(x + j, y + i, color);
}

static void dibujar_rect(int x, int y, int ancho, int alto, int color)
{
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            gbt_dibujar_pixel(x + j, y + i, color);
}

static void dibujar_borde(int x, int y, int ancho, int alto, int color)
{
    for (int j = 0; j < ancho; j++)
    {
        gbt_dibujar_pixel(x + j, y,            color);
        gbt_dibujar_pixel(x + j, y + alto - 1, color);
    }
    for (int i = 0; i < alto; i++)
    {
        gbt_dibujar_pixel(x,             y + i, color);
        gbt_dibujar_pixel(x + ancho - 1, y + i, color);
    }
}

static void dibujar_tablero()
{
    dibujar_rect(TABLERO_X, TABLERO_Y, COLUMNAS * TAM, FILAS * TAM, 0);

    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
        {
            int val = tablero_get(i, j);
            if (val != 0)
                dibujar_bloque(TABLERO_X + j * TAM, TABLERO_Y + i * TAM, val);
        }

    dibujar_borde(TABLERO_X - 1, TABLERO_Y - 1,
                  COLUMNAS * TAM + 2, FILAS * TAM + 2, COLOR_BORDE);
}

static void dibujar_pieza(t_estado_juego *eg)
{
    for (int i = 0; i < TAM_M; i++)
        for (int j = 0; j < TAM_M; j++)
            if (eg->pieza_actual->tamano[i][j] == 1)
                dibujar_bloque(TABLERO_X + (eg->pieza_x + j) * TAM,
                               TABLERO_Y + (eg->pieza_y + i) * TAM,
                               eg->pieza_actual->c);
}

static void dibujar_pieza_siguiente(t_estado_juego *eg)
{
    if (eg->pieza_siguiente == NULL)
        return;

    /*
     * La preview de pieza siguiente se ubica debajo de "NEXT".
     * NEXT esta en Y+220, el valor en Y+238, la preview en Y+256.
     */
    int prev_x = PANEL_X;
    int prev_y = PANEL_Y + 256;
    dibujar_rect(prev_x, prev_y, 4 * TAM, 4 * TAM, 0);
    dibujar_borde(prev_x - 1, prev_y - 1, 4 * TAM + 2, 4 * TAM + 2, COLOR_BORDE);

    for (int i = 0; i < TAM_M; i++)
        for (int j = 0; j < TAM_M; j++)
            if (eg->pieza_siguiente->tamano[i][j] == 1)
                dibujar_bloque(prev_x + j * TAM,
                               prev_y + i * TAM,
                               eg->pieza_siguiente->c);
}

/*
 * Layout del panel (cada seccion ocupa 16px titulo + 14px valor + 10px margen = ~40px):
 *
 *  Y +  0  : titulo "SCORE"   (8x16)
 *  Y + 20  : valor  puntaje   (8x8)
 *  Y + 40  : titulo "NIVEL"   (8x16)
 *  Y + 60  : valor  nivel     (8x8)
 *  Y + 80  : titulo "CAIDA"   (8x16)
 *  Y +100  : valor  ms        (8x8)
 *  Y +120  : titulo "LINEAS"  (8x16)
 *  Y +140  : valor  lineas    (8x8)
 *  Y +160  : titulo "NEXT"    (8x16)
 *  Y +180  : (espacio)
 *  Y +196  : preview pieza siguiente (4*20 = 80px alto)
 *
 * Total panel: 340px de alto, encaja en la ventana VGA (480px).
 */
static void dibujar_panel(t_estado_juego *eg)
{
    char buf[16];
    const t_fuente *fg = fuente_get_8x16();
    const t_fuente *fc = fuente_get_8x8();

    dibujar_rect (PANEL_X - 4, PANEL_Y - 4, 180, 370, COLOR_FONDO_PANEL);
    dibujar_borde(PANEL_X - 4, PANEL_Y - 4, 180, 370, COLOR_BORDE);

    /* SCORE */
    fuente_dibujar_texto(fg, "SCORE", PANEL_X, PANEL_Y,      COLOR_TITULO);
    sprintf(buf, "%d", eg->puntaje);
    fuente_dibujar_texto(fc, buf,     PANEL_X, PANEL_Y + 20, COLOR_VALOR);

    /* NIVEL */
    fuente_dibujar_texto(fg, "NIVEL", PANEL_X, PANEL_Y + 40, COLOR_TITULO);
    sprintf(buf, "%d", eg->nivel);
    fuente_dibujar_texto(fc, buf,     PANEL_X, PANEL_Y + 60, COLOR_VALOR);

    /* CAIDA */
    fuente_dibujar_texto(fg, "CAIDA", PANEL_X, PANEL_Y + 80,  COLOR_TITULO);
    sprintf(buf, "%d MS", (int)eg->intervalo_caida_ms);
    fuente_dibujar_texto(fc, buf,     PANEL_X, PANEL_Y + 100, COLOR_VALOR);

    /* LINEAS */
    fuente_dibujar_texto(fg, "LINEAS", PANEL_X, PANEL_Y + 120, COLOR_TITULO);
    sprintf(buf, "%d", eg->lineas_limpiadas);
    fuente_dibujar_texto(fc, buf,      PANEL_X, PANEL_Y + 140, COLOR_VALOR);

    /* NEXT */
    fuente_dibujar_texto(fg, "NEXT", PANEL_X, PANEL_Y + 160, COLOR_TITULO);
    dibujar_pieza_siguiente(eg);
}

static void render_menu()
{
    const t_fuente *fg = fuente_get_8x16();

    dibujar_rect (180, 140, 280, 50, 4);
    dibujar_borde(180, 140, 280, 50, COLOR_BORDE);
    fuente_dibujar_texto(fg, "TETRIS", 270, 155, COLOR_TITULO);
}

static void render_juego(t_estado_juego *eg)
{
    dibujar_tablero();
    dibujar_pieza(eg);
    dibujar_panel(eg);
}

static void render_pausa(t_estado_juego *eg)
{
    render_juego(eg);
    dibujar_rect (240, 210, 160, 50, 1);
    dibujar_borde(240, 210, 160, 50, COLOR_BORDE);
    fuente_dibujar_texto(fuente_get_8x16(), "PAUSA",       278, 220, COLOR_TITULO);
    fuente_dibujar_texto(fuente_get_8x8(),  "P: CONTINUAR", 248, 244, COLOR_VALOR);
}

static void render_gameover(t_estado_juego *eg)
{
    char buf[16];

    dibujar_rect (160, 180, 320, 110, 4);
    dibujar_borde(160, 180, 320, 110, COLOR_BORDE);

    fuente_dibujar_texto(fuente_get_8x16(), "GAME OVER", 210, 192, COLOR_TITULO);

    fuente_dibujar_texto(fuente_get_8x8(), "SCORE:", 210, 226, COLOR_VALOR);
    sprintf(buf, "%d", eg->puntaje);
    fuente_dibujar_texto(fuente_get_8x8(), buf, 270, 226, COLOR_TITULO);

    fuente_dibujar_texto(fuente_get_8x8(), "ENTER: VOLVER AL MENU", 200, 260, COLOR_BORDE);
}

void render_dibujar(t_estado_juego *eg)
{
    gbt_borrar_backbuffer(0);

    switch (eg->estado)
    {
        case ESTADO_MENU:     render_menu();       break;
        case ESTADO_JUGANDO:  render_juego(eg);    break;
        case ESTADO_PAUSA:    render_pausa(eg);    break;
        case ESTADO_GAMEOVER: render_gameover(eg); break;
    }

    gbt_volcar_backbuffer();
}
