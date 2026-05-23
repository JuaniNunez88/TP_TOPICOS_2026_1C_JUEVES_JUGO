#include "menu.h"
#include "fuente.h"
#include "deteccion_tecla.h"
#include "GBT/gbt.h"
#include <string.h>
#include <stdio.h>

static void rect(int x, int y, int w, int h, int color)
{
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            gbt_dibujar_pixel(x + j, y + i, color);
}

static void borde_rect(int x, int y, int w, int h, int color)
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

static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }
static int centrar(int chars, int area_x, int area_w)
{
    return area_x + (area_w - chars * 9) / 2;
}

static int res_ancho(const t_config *cfg)
{
    return (cfg->resolucion == RES_CGA) ? 320 : 640;
}
static int res_alto(const t_config *cfg)
{
    return (cfg->resolucion == RES_CGA) ? 200 : 480;
}
static int es_cga(const t_config *cfg)
{
    return (cfg->resolucion == RES_CGA);
}

void menu_iniciar(t_menu *m)
{
    m->estado             = MENU_PRINCIPAL;
    m->opcion             = OP_JUGAR;
    m->cfg_opcion         = CFG_RESOLUCION;
    m->nombre_para_jugar  = 0;
    memset(m->nombre_buf, 0, sizeof(m->nombre_buf));
    m->nombre_len = 0;
}

void menu_ir_gameover(t_menu *m)
{
    m->estado = MENU_GAMEOVER;
}

static int actualizar_nombre(t_menu *m, t_stats *stats)
{
    static const eGBT_Tecla letras[26] = {
        GBTK_a, GBTK_b, GBTK_c, GBTK_d, GBTK_e, GBTK_f, GBTK_g,
        GBTK_h, GBTK_i, GBTK_j, GBTK_k, GBTK_l, GBTK_m, GBTK_n,
        GBTK_o, GBTK_p, GBTK_q, GBTK_r, GBTK_s, GBTK_t, GBTK_u,
        GBTK_v, GBTK_w, GBTK_x, GBTK_y, GBTK_z
    };

    if (m->nombre_len < 16)
        for (int i = 0; i < 26; i++)
            if (gbt_tecla_presionada(letras[i]))
            {
                m->nombre_buf[m->nombre_len++] = 'A' + i;
                m->nombre_buf[m->nombre_len]   = '\0';
                break;
            }

    if (gbt_tecla_presionada(GBTK_RETROCESO) && m->nombre_len > 0)
        m->nombre_buf[--m->nombre_len] = '\0';

    if (input_enter() && m->nombre_len > 0)
    {
        strncpy(stats->nombre, m->nombre_buf, STATS_MAX_NOMBRE);
        stats->nombre[STATS_MAX_NOMBRE] = '\0';
        stats_guardar(stats);
        return 1;  // nombre confirmado
    }
    return 0;
}

int menu_actualizar(t_menu *m, t_config *cfg, t_stats *stats,
                    int puntaje_ultimo, int *res_cambio)
{
    *res_cambio = 0;

    switch (m->estado)
    {
    case MENU_PRINCIPAL:
        if (input_abajo_press())
            m->opcion = (m->opcion + 1) % OP_TOTAL;
        if (input_arriba_press())
            m->opcion = (m->opcion + OP_TOTAL - 1) % OP_TOTAL;

        if (input_enter())
        {
            if (m->opcion == OP_SALIR) return 2;

            if (m->opcion == OP_OPCIONES)
            {
                m->estado     = MENU_OPCIONES;
                m->cfg_opcion = CFG_RESOLUCION;
            }

            if (m->opcion == OP_NOMBRE)
            {
                memset(m->nombre_buf, 0, sizeof(m->nombre_buf));
                m->nombre_len        = 0;
                m->nombre_para_jugar = 0;
                m->estado = MENU_NOMBRE;
            }

            if (m->opcion == OP_JUGAR)
            {
                if (stats->nombre[0] != '\0')
                    return 1;
                memset(m->nombre_buf, 0, sizeof(m->nombre_buf));
                m->nombre_len        = 0;
                m->nombre_para_jugar = 1;
                m->estado = MENU_NOMBRE;
            }
        }
        break;

    case MENU_OPCIONES:
        if (input_abajo_press())
            m->cfg_opcion = (m->cfg_opcion + 1) % CFG_TOTAL;
        if (input_arriba_press())
            m->cfg_opcion = (m->cfg_opcion + CFG_TOTAL - 1) % CFG_TOTAL;

        if (input_enter() && m->cfg_opcion == CFG_VOLVER)
        {
            config_guardar(cfg);
            *res_cambio = 1;
            m->estado = MENU_PRINCIPAL;
        }

        if (input_izquierda_press() || input_derecha_press())
        {
            int dir = input_derecha_press() ? 1 : -1;
            if (m->cfg_opcion == CFG_RESOLUCION)
                cfg->resolucion = (cfg->resolucion == RES_VGA) ? RES_CGA : RES_VGA;
            if (m->cfg_opcion == CFG_PALETA)
                cfg->paleta = (cfg->paleta + PALETA_TOTAL + dir) % PALETA_TOTAL;
            if (m->cfg_opcion == CFG_VELOCIDAD)
            {
                cfg->velocidad += dir * 5;
                if (cfg->velocidad < 10) cfg->velocidad = 10;
                if (cfg->velocidad > 90) cfg->velocidad = 90;
            }
        }
        break;

    case MENU_NOMBRE:
        if (actualizar_nombre(m, stats))
        {
            if (m->nombre_para_jugar)
                return 1;
            else
                m->estado = MENU_PRINCIPAL;
        }
        break;

    case MENU_GAMEOVER:
        stats_actualizar(stats, puntaje_ultimo);
        if (input_enter())
        {
            m->estado = MENU_PRINCIPAL;
            m->opcion = OP_JUGAR;
        }
        break;
    }
    return 0;
}

static void dibujar_principal(const t_menu *m, const t_config *cfg,
                               const t_stats *stats)
{
    const t_paleta *p  = config_get_paleta(cfg);
    const t_fuente *fg = es_cga(cfg) ? fuente_get_8x8() : fuente_get_8x16();
    const t_fuente *fc = fuente_get_8x8();
    int W = res_ancho(cfg), H = res_alto(cfg);
    int cga = es_cga(cfg);

    int tw = cga ? 160 : 340;
    int th = cga ?  32 :  70;
    int tx = (W - tw) / 2;
    int ty = cga ? 14 : 60;
    rect      (tx, ty, tw, th, p->fondo_banner);
    borde_rect(tx, ty, tw, th, p->borde);
    fuente_dibujar_texto(fg, "TETRIS",
                         centrar(slen("TETRIS"), tx, tw),
                         ty + (cga ? 10 : 26), p->titulo);

    if (stats->nombre[0] != '\0')
    {
        char buf[40];
        int sy = cga ? 56 : 150;
        sprintf(buf, "JUGADOR: %s", stats->nombre);
        fuente_dibujar_texto(fc, buf, centrar(slen(buf), 0, W), sy, p->valor);
        if (stats->nombre_max[0] != '\0' && stats->puntaje_max > 0)
            sprintf(buf, "MAX(%s): %d", stats->nombre_max, stats->puntaje_max);
        else
            sprintf(buf, "MAX: %d", stats->puntaje_max);
        fuente_dibujar_texto(fc, buf, centrar(slen(buf), 0, W), sy + 12, p->cursor);
    }

    const char *ops[OP_TOTAL] = {"JUGAR", "NOMBRE", "OPCIONES", "SALIR"};
    int pw = cga ? 140 : 280;
    int ph = cga ?  76 : 194;
    int px = (W - pw) / 2;
    int py = cga ? 80 : 200;
    rect      (px, py, pw, ph, p->fondo_panel);
    borde_rect(px, py, pw, ph, p->borde);

    for (int i = 0; i < OP_TOTAL; i++)
    {
        int oy  = py + (cga ? 8 : 20) + i * (cga ? 16 : 44);
        int col = (i == m->opcion) ? p->cursor : p->titulo;
        if (i == m->opcion)
            fuente_dibujar_texto(fc, ">", px + 4, oy + 2, p->cursor);
        fuente_dibujar_texto(fg, ops[i],
                             centrar(slen(ops[i]), px, pw), oy, col);
    }

    fuente_dibujar_texto(fc, "FLECHAS: NAVEGAR   ENTER: SELECCIONAR",
                         centrar(37, 0, W), cga ? H - 12 : 420, p->borde);
}

static void dibujar_opciones(const t_menu *m, const t_config *cfg)
{
    const t_paleta *p  = config_get_paleta(cfg);
    const t_fuente *fg = es_cga(cfg) ? fuente_get_8x8() : fuente_get_8x16();
    const t_fuente *fc = fuente_get_8x8();
    int W = res_ancho(cfg), H = res_alto(cfg);
    int cga = es_cga(cfg);
    char buf[16];

    int pw = cga ? 200 : 400;
    int ph = cga ? 120 : 240;
    int px = (W - pw) / 2;
    int py = cga ?  20 : 110;
    rect      (px, py, pw, ph, p->fondo_panel);
    borde_rect(px, py, pw, ph, p->borde);

    fuente_dibujar_texto(fg, "OPCIONES",
                         centrar(slen("OPCIONES"), px, pw),
                         py + (cga ? 4 : 12), p->titulo);

    int sep_y = py + (cga ? 16 : 34);
    for (int j = 4; j < pw - 4; j++)
        gbt_dibujar_pixel(px + j, sep_y, p->borde);

    struct { const char *label; const char *valor; } items[CFG_TOTAL];
    items[CFG_RESOLUCION].label = "RESOLUCION:";
    items[CFG_RESOLUCION].valor = config_nombre_resolucion(cfg->resolucion);
    items[CFG_PALETA].label     = "PALETA:";
    items[CFG_PALETA].valor     = config_nombre_paleta(cfg->paleta);
    sprintf(buf, "%d", cfg->velocidad);
    items[CFG_VELOCIDAD].label  = "VELOCIDAD:";
    items[CFG_VELOCIDAD].valor  = buf;
    items[CFG_VOLVER].label     = "APLICAR Y VOLVER";
    items[CFG_VOLVER].valor     = "";

    int paso = cga ? 20 : 42;
    int iy   = sep_y + (cga ? 4 : 10);
    for (int i = 0; i < CFG_TOTAL; i++)
    {
        int col = (i == m->cfg_opcion) ? p->cursor : p->titulo;
        if (i == m->cfg_opcion)
            fuente_dibujar_texto(fc, ">", px + 4, iy + 2, p->cursor);
        fuente_dibujar_texto(fg, items[i].label, px + 14, iy, col);
        if (items[i].valor[0] != '\0')
        {
            int vlen = slen(items[i].valor);
            fuente_dibujar_texto(fg, items[i].valor,
                                 px + pw - vlen * 9 - 8, iy, p->valor);
        }
        iy += paso;
    }

    fuente_dibujar_texto(fc, "< > CAMBIAR   ENTER: APLICAR Y VOLVER",
                         centrar(37, 0, W), cga ? H - 12 : 390, p->borde);
}

static void dibujar_nombre(const t_menu *m, const t_config *cfg)
{
    const t_paleta *p  = config_get_paleta(cfg);
    const t_fuente *fg = es_cga(cfg) ? fuente_get_8x8() : fuente_get_8x16();
    const t_fuente *fc = fuente_get_8x8();
    int W = res_ancho(cfg), H = res_alto(cfg);
    int cga = es_cga(cfg);

    int pw = cga ? 200 : 400;
    int ph = cga ?  80 : 160;
    int px = (W - pw) / 2;
    int py = (H - ph) / 2;
    rect      (px, py, pw, ph, p->fondo_panel);
    borde_rect(px, py, pw, ph, p->borde);

    fuente_dibujar_texto(fg, "INGRESA TU NOMBRE",
                         centrar(slen("INGRESA TU NOMBRE"), px, pw),
                         py + (cga ? 6 : 16), p->titulo);

    int bx = px + 10, by = py + (cga ? 28 : 60);
    int bw = pw - 20, bh = cga ? 14 : 28;
    rect      (bx, by, bw, bh, 0);
    borde_rect(bx, by, bw, bh, p->cursor);

    char display[18];
    int  dlen = 0;
    const char *nb = m->nombre_buf;
    while (nb[dlen]) { display[dlen] = nb[dlen]; dlen++; }
    display[dlen++] = '_';
    display[dlen]   = '\0';
    fuente_dibujar_texto(fg, display, bx + 4, by + 3, p->valor);

    fuente_dibujar_texto(fc, "A-Z  RETROCESO: BORRAR  ENTER: OK",
                         centrar(33, 0, W), py + ph - 14, p->borde);
}

static void dibujar_gameover(const t_menu *m, const t_config *cfg,
                              const t_stats *stats, int puntaje_ultimo)
{
    const t_paleta *p  = config_get_paleta(cfg);
    const t_fuente *fg = es_cga(cfg) ? fuente_get_8x8() : fuente_get_8x16();
    const t_fuente *fc = fuente_get_8x8();
    int W = res_ancho(cfg), H = res_alto(cfg);
    int cga = es_cga(cfg);
    char buf[40];

    int pw = cga ? 200 : 340;
    int ph = cga ? 110 : 200;
    int px = (W - pw) / 2;
    int py = (H - ph) / 2;
    rect      (px, py, pw, ph, p->fondo_banner);
    borde_rect(px, py, pw, ph, p->borde);

    fuente_dibujar_texto(fg, "GAME OVER",
                         centrar(slen("GAME OVER"), px, pw),
                         py + (cga ? 4 : 16), p->titulo);

    int sep_y = py + (cga ? 16 : 40);
    for (int j = 4; j < pw - 4; j++)
        gbt_dibujar_pixel(px + j, sep_y, p->borde);

    int y    = sep_y + (cga ? 6 : 10);
    int paso = cga ? 12 : 18;

    sprintf(buf, "JUGADOR: %s", stats->nombre);
    fuente_dibujar_texto(fc, buf, centrar(slen(buf), px, pw), y, p->valor);
    y += paso;

    sprintf(buf, "PUNTAJE: %d", puntaje_ultimo);
    fuente_dibujar_texto(fc, buf, centrar(slen(buf), px, pw), y, p->valor);
    y += paso;

    int col_max = (puntaje_ultimo >= stats->puntaje_max) ? p->cursor : p->titulo;
    if (stats->nombre_max[0] != '\0')
        sprintf(buf, "MAX(%s): %d", stats->nombre_max, stats->puntaje_max);
    else
        sprintf(buf, "MAXIMO: %d", stats->puntaje_max);
    fuente_dibujar_texto(fc, buf, centrar(slen(buf), px, pw), y, col_max);
    y += paso;

    if (puntaje_ultimo >= stats->puntaje_max && puntaje_ultimo > 0)
    {
        fuente_dibujar_texto(fc, "NUEVO RECORD!",
                             centrar(slen("NUEVO RECORD!"), px, pw),
                             y, p->cursor);
    }

    fuente_dibujar_texto(fc, "ENTER: VOLVER AL MENU",
                         centrar(slen("ENTER: VOLVER AL MENU"), px, pw),
                         py + ph - 14, p->borde);
}

void menu_dibujar(const t_menu *m, const t_config *cfg,
                  const t_stats *stats, int puntaje_ultimo)
{
    gbt_borrar_backbuffer(0);

    switch (m->estado)
    {
        case MENU_PRINCIPAL: dibujar_principal(m, cfg, stats);                break;
        case MENU_OPCIONES:  dibujar_opciones(m, cfg);                        break;
        case MENU_NOMBRE:    dibujar_nombre(m, cfg);                          break;
        case MENU_GAMEOVER:  dibujar_gameover(m, cfg, stats, puntaje_ultimo); break;
    }

    gbt_volcar_backbuffer();
}
