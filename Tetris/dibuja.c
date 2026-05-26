#include "dibuja.h"
#include "tablero.h"
#include "fuente.h"
#include "config.h"
#include "GBT/gbt.h"
#include <stdio.h>

#define TAM_M 4

static int get_tam(const t_resolucion *res)
{
    return (res->tipo == RES_CGA) ? 10 : 20;
}
static int get_tablero_x(const t_resolucion *res)
{
    return (res->tipo == RES_CGA) ? 60 : 220;
}
static int get_tablero_y(const t_resolucion *res)
{
    return (res->tipo == RES_CGA) ? 0 : 40;
}
static int get_panel_x(const t_resolucion *res, int tablero_x, int tam)
{
    return tablero_x + COLUMNAS * tam + (res->tipo == RES_CGA ? 6 : 10);
}
static int get_panel_w(const t_resolucion *res, int panel_x)
{
    return res->ancho - panel_x - 2;
}

static void dibujar_bloque(int x, int y, int tam, int color)
{
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            gbt_dibujar_pixel(x + j, y + i, color);
}

static void dibujar_rect(int x, int y, int w, int h, int color)
{
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            gbt_dibujar_pixel(x + j, y + i, color);
}

static void dibujar_borde(int x, int y, int w, int h, int color)
{
    for (int j = 0; j < w; j++)
    {
        gbt_dibujar_pixel(x + j, y,         color);
        gbt_dibujar_pixel(x + j, y + h - 1, color);
    }
    for (int i = 0; i < h; i++)
    {
        gbt_dibujar_pixel(x,         y + i, color);
        gbt_dibujar_pixel(x + w - 1, y + i, color);
    }
}

static void dibujar_cuadricula(int tablero_x, int tablero_y,
                               int tablero_w, int tablero_h,
                               int tam, int color)
{
    for (int c = 0; c <= COLUMNAS; c++)
        for (int f = 0; f < tablero_h; f++)
            gbt_dibujar_pixel(tablero_x + c * tam, tablero_y + f, color);
    for (int f = 0; f <= FILAS; f++)
        for (int c = 0; c < tablero_w; c++)
            gbt_dibujar_pixel(tablero_x + c, tablero_y + f * tam, color);
}

static int slen(const char *s)
{
    int n = 0;
    while (s[n]) n++;
    return n;
}
static int centrar(int chars, int area_x, int area_w)
{
    return area_x + (area_w - chars * 9) / 2;
}

static void dibujar_tablero(const t_paleta *p, const t_resolucion *res)
{
    int tam       = get_tam(res);
    int tablero_x = get_tablero_x(res);
    int tablero_y = get_tablero_y(res);
    int tablero_w = COLUMNAS * tam;
    int tablero_h = FILAS    * tam;

    dibujar_rect(tablero_x, tablero_y, tablero_w, tablero_h, 0);

    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
        {
            int val = tablero_get(i, j);
            if (val != 0)
            {
                int color = p->pieza[val - 1];
                dibujar_bloque(tablero_x + j * tam,
                               tablero_y + i * tam, tam, color);
            }
        }

    dibujar_borde(tablero_x - 1, tablero_y - 1,
                  tablero_w + 2, tablero_h + 2, p->borde);
}

static void dibujar_pieza(t_estado_juego *eg, const t_paleta *p,
                          const t_resolucion *res)
{
    int tam       = get_tam(res);
    int tablero_x = get_tablero_x(res);
    int tablero_y = get_tablero_y(res);
    int color     = p->pieza[eg->pieza_actual->tipo];

    for (int i = 0; i < TAM_M; i++)
    {
        for (int j = 0; j < TAM_M; j++)
        {
            if (eg->pieza_actual->tamano[i][j] == 1)
            {
                if (eg->pieza_y + i < 0)
                    continue;

                dibujar_bloque(
                    tablero_x + (eg->pieza_x + j) * tam,
                    tablero_y + (eg->pieza_y + i) * tam,
                    tam,
                    color
                );
            }
        }
    }
}

static void dibujar_pieza_siguiente(t_estado_juego *eg, const t_paleta *p,
                                    const t_resolucion *res,
                                    int panel_x, int panel_w, int area_y)
{
    if (eg->pieza_siguiente == NULL) return;

    int tam    = get_tam(res);
    int area_w = TAM_M * tam;
    int area_h = TAM_M * tam;
    int ax     = panel_x + (panel_w - area_w) / 2;
    int ay     = area_y;

    dibujar_rect (ax, ay, area_w, area_h, 0);
    dibujar_borde(ax - 1, ay - 1, area_w + 2, area_h + 2, p->borde);

    int min_c = TAM_M, max_c = -1, min_f = TAM_M, max_f = -1;
    for (int i = 0; i < TAM_M; i++)
        for (int j = 0; j < TAM_M; j++)
            if (eg->pieza_siguiente->tamano[i][j] == 1)
            {
                if (j < min_c) min_c = j;
                if (j > max_c) max_c = j;
                if (i < min_f) min_f = i;
                if (i > max_f) max_f = i;
            }

    int pw = (max_c - min_c + 1) * tam;
    int ph = (max_f - min_f + 1) * tam;
    int ox = (area_w - pw) / 2 - min_c * tam;
    int oy = (area_h - ph) / 2 - min_f * tam;

    int color = p->pieza[eg->pieza_siguiente->tipo];
    for (int i = 0; i < TAM_M; i++)
        for (int j = 0; j < TAM_M; j++)
            if (eg->pieza_siguiente->tamano[i][j] == 1)
                dibujar_bloque(ax + ox + j * tam,
                               ay + oy + i * tam, tam, color);
}

static void dibujar_panel(t_estado_juego *eg, const t_paleta *p,
                          const t_stats *stats, const t_resolucion *res)
{
    char buf[32];
    int  tam     = get_tam(res);
    int  tx      = get_tablero_x(res);
    int  ty      = get_tablero_y(res);
    int  panel_x = get_panel_x(res, tx, tam);
    int  panel_w = get_panel_w(res, panel_x);
    int  panel_y = ty + 2;
    int  panel_h = res->alto - panel_y * 2;
    int  es_cga  = (res->tipo == RES_CGA);

    const t_fuente *fg = es_cga ? fuente_get_8x8() : fuente_get_8x16();
    const t_fuente *fc = fuente_get_8x8();

    dibujar_rect (panel_x - 2, panel_y, panel_w, panel_h, p->fondo_panel);
    dibujar_borde(panel_x - 2, panel_y, panel_w, panel_h, p->borde);

    int titulo_h = es_cga ?  8 : 16;
    int valor_h  = 8;
    int margen   = es_cga ?  3 :  6;
    int y        = panel_y + 4;

    if (stats->nombre[0] != '\0')
    {
        fuente_dibujar_texto(fc, stats->nombre,
                             centrar(slen(stats->nombre), panel_x - 2, panel_w),
                             y, p->cursor);
        y += valor_h + margen;
    }

    fuente_dibujar_texto(fg, "SCORE",
                         centrar(slen("SCORE"), panel_x - 2, panel_w),
                         y, p->titulo);
    y += titulo_h + 2;
    sprintf(buf, "%d", eg->puntaje);
    fuente_dibujar_texto(fc, buf,
                         centrar(slen(buf), panel_x - 2, panel_w),
                         y, p->valor);
    y += valor_h + margen;

    if (stats->puntaje_max > 0)
    {
        if (stats->nombre_max[0] != '\0')
            sprintf(buf, "MAX(%s):%d", stats->nombre_max, stats->puntaje_max);
        else
            sprintf(buf, "MAX:%d", stats->puntaje_max);
        if (slen(buf) > 14) sprintf(buf, "MAX:%d", stats->puntaje_max);
        fuente_dibujar_texto(fc, buf,
                             centrar(slen(buf), panel_x - 2, panel_w),
                             y, p->borde);
        y += valor_h + margen;
    }

    fuente_dibujar_texto(fg, "NIVEL",
                         centrar(slen("NIVEL"), panel_x - 2, panel_w),
                         y, p->titulo);
    y += titulo_h + 2;
    sprintf(buf, "%d", eg->nivel);
    fuente_dibujar_texto(fc, buf,
                         centrar(slen(buf), panel_x - 2, panel_w),
                         y, p->valor);
    y += valor_h + margen;

    fuente_dibujar_texto(fg, "CAIDA",
                         centrar(slen("CAIDA"), panel_x - 2, panel_w),
                         y, p->titulo);
    y += titulo_h + 2;
    sprintf(buf, "%dMS", (int)eg->intervalo_caida_ms);
    fuente_dibujar_texto(fc, buf,
                         centrar(slen(buf), panel_x - 2, panel_w),
                         y, p->valor);
    y += valor_h + margen;

    fuente_dibujar_texto(fg, "LINEAS",
                         centrar(slen("LINEAS"), panel_x - 2, panel_w),
                         y, p->titulo);
    y += titulo_h + 2;
    sprintf(buf, "%d", eg->lineas_limpiadas);
    fuente_dibujar_texto(fc, buf,
                         centrar(slen(buf), panel_x - 2, panel_w),
                         y, p->valor);
    y += valor_h + margen;

    fuente_dibujar_texto(fg, "NEXT",
                         centrar(slen("NEXT"), panel_x - 2, panel_w),
                         y, p->titulo);
    y += titulo_h + 2;

    dibujar_pieza_siguiente(eg, p, res, panel_x - 2, panel_w, y);
}

static void render_pausa(t_estado_juego *eg, const t_paleta *p,
                         const t_stats *stats)
{
    const t_resolucion *res = &eg->res;
    int tam       = get_tam(res);
    int tablero_x = get_tablero_x(res);
    int tablero_y = get_tablero_y(res);
    int tablero_w = COLUMNAS * tam;
    int tablero_h = FILAS    * tam;

    dibujar_tablero(p, res);
    dibujar_pieza(eg, p, res);
    dibujar_cuadricula( get_tablero_x(res), get_tablero_y(res), COLUMNAS*get_tam(res), FILAS*get_tam(res), get_tam(res), p->fondo_panel);
    dibujar_panel(eg, p, stats, res);

    int bw = (res->tipo == RES_CGA) ? 100 : 200;
    int bh = (res->tipo == RES_CGA) ?  32 :  56;
    int bx = tablero_x + (tablero_w - bw) / 2;
    int by = tablero_y + (tablero_h - bh) / 2;

    dibujar_rect (bx, by, bw, bh, p->fondo_banner);
    dibujar_borde(bx, by, bw, bh, p->borde);

    const t_fuente *fp = (res->tipo == RES_CGA)
                         ? fuente_get_8x8()
                         : fuente_get_8x16();

    int pausa_y = (res->tipo == RES_CGA) ? by + 4  : by + 8;
    int cont_y  = (res->tipo == RES_CGA) ? by + 18 : by + 30;

    fuente_dibujar_texto(fp, "PAUSA",
                         centrar(slen("PAUSA"), bx, bw),
                         pausa_y,
                         p->titulo);

    fuente_dibujar_texto(fuente_get_8x8(), "P:CONTINUAR",
                         centrar(slen("P:CONTINUAR"), bx, bw),
                         cont_y,
                         p->valor);
}

void render_dibujar(t_estado_juego *eg, const t_config *cfg,
                    const t_stats *stats)
{
    const t_paleta    *p   = config_get_paleta(cfg);
    const t_resolucion *res = &eg->res;

    gbt_borrar_backbuffer(0);

    switch (eg->estado)
    {
    case ESTADO_JUGANDO:
        dibujar_tablero(p, res);
        dibujar_pieza(eg, p, res);
        dibujar_cuadricula( get_tablero_x(res), get_tablero_y(res), COLUMNAS*get_tam(res), FILAS*get_tam(res), get_tam(res), p->fondo_panel);
        dibujar_panel(eg, p, stats, res);
        break;
    case ESTADO_PAUSA:
        render_pausa(eg, p, stats);
        break;
    case ESTADO_GAMEOVER:
        break;
    default:
        break;
    }

    gbt_volcar_backbuffer();
}
