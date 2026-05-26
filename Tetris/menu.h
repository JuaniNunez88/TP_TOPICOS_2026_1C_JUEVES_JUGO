#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "config.h"
#include "persona.h"

typedef enum
{
    MENU_PRINCIPAL = 0,
    MENU_OPCIONES,
    MENU_NOMBRE,
    MENU_GAMEOVER
} e_estado_menu;

typedef enum
{
    OP_JUGAR = 0,
    OP_NOMBRE,
    OP_OPCIONES,
    OP_SALIR,
    OP_TOTAL
} e_opcion_principal;

typedef enum
{
    CFG_RESOLUCION = 0,
    CFG_PALETA,
    CFG_VELOCIDAD,
    CFG_VOLVER,
    CFG_TOTAL
} e_opcion_cfg;

typedef struct
{
    e_estado_menu      estado;
    e_resolucion resolucion_temp;
    e_opcion_principal opcion;
    e_opcion_cfg       cfg_opcion;
    char               nombre_buf[17];
    int                nombre_len;
    int                nombre_para_jugar;
} t_menu;

void menu_iniciar     (t_menu *m);
void menu_ir_gameover (t_menu *m);

int  menu_actualizar(t_menu *m, t_config *cfg, t_stats *stats,
                     int puntaje_ultimo, int *res_cambio);

void menu_dibujar(const t_menu *m, const t_config *cfg,
                  const t_stats *stats, int puntaje_ultimo);

#endif // MENU_H_INCLUDED
