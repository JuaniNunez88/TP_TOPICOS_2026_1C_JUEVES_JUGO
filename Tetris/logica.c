#include "logica.h"
#include "deteccion_tecla.h"
#include "tablero.h"
#include "piezas.h"
#include "GBT/gbt.h"

#define COLUMNAS 10
#define FILAS    20

static void spawnear_pieza(t_estado_juego *eg)
{
    eg->pieza_actual = elegir_pieza_aleatoria();
    eg->pieza_x      = 4;
    eg->pieza_y      = 0;
}

void fijar_pieza(t_estado_juego *eg)
{
    for (int i = 0; i < TMAT; i++)
        for (int j = 0; j < TMAT; j++)
            if (eg->pieza_actual->tamano[i][j] == 1)
                tablero_set(eg->pieza_y + i, eg->pieza_x + j, eg->pieza_actual->c);
}

int choque_vert(t_estado_juego *eg, int futura_y)
{
    for (int i = 0; i < TMAT; i++)
        for (int j = 0; j < TMAT; j++)
            if (eg->pieza_actual->tamano[i][j] == 1)
            {
                int pos_y = futura_y + i;
                int pos_x = eg->pieza_x + j;
                if (pos_y >= FILAS || tablero_get(pos_y, pos_x) != 0)
                    return 1;
            }
    return 0;
}

int choque_horiz(t_estado_juego *eg, int futura_x)
{
    for (int i = 0; i < TMAT; i++)
        for (int j = 0; j < TMAT; j++)
            if (eg->pieza_actual->tamano[i][j] == 1)
            {
                int pos_y = eg->pieza_y + i;
                int pos_x = futura_x + j;
                if (pos_x < 0 || pos_x >= COLUMNAS ||
                    (pos_y >= 0 && pos_y < FILAS && tablero_get(pos_y, pos_x) != 0))
                    return 1;
            }
    return 0;
}

static void limpiar_y_puntuar(t_estado_juego *eg)
{
    int lineas = tablero_limpiar_lineas_completas();
    if (lineas > 0)
        eg->puntaje += (lineas * 100) + ((lineas - 1) * 50);
}

void juego_iniciar(t_estado_juego *eg)
{
    eg->estado         = ESTADO_MENU;
    eg->puntaje        = 0;
    eg->juego_terminado = 0;
    eg->timer_caida    = 0;
    inicializar_tablero();
    spawnear_pieza(eg);
}

void juego_actualizar(t_estado_juego *eg, int *val)
{

    eGBT_Tecla t = gbt_obtener_tecla_presionada();
    if (t == GBTK_ESCAPE)
    {
        *val = 0;
        return;
    }

    switch (eg->estado)
    {
    // ------------------------
    case ESTADO_MENU:
        if (input_enter())
        {
            inicializar_tablero();
            eg->puntaje         = 0;
            eg->juego_terminado = 0;
            eg->timer_caida     = 0;
            spawnear_pieza(eg);
            eg->estado = ESTADO_JUGANDO;
        }
        break;

    // ------------------------
    case ESTADO_JUGANDO:
    {

        int nueva_x = eg->pieza_x;
        if (input_derecha())  nueva_x++;
        if (input_izquierda()) nueva_x--;

        if (!choque_horiz(eg, nueva_x))
            eg->pieza_x = nueva_x;


        if (input_arriba())
            rotarPieza(eg->pieza_actual);

        if (input_abajo())
        {
            int nueva_y = eg->pieza_y + 1;
            if (choque_vert(eg, nueva_y))
            {
                fijar_pieza(eg);
                limpiar_y_puntuar(eg);
                spawnear_pieza(eg);
                if (choque_vert(eg, eg->pieza_y))
                    eg->estado = ESTADO_GAMEOVER;
            }
            else
            {
                eg->pieza_y = nueva_y;
                eg->puntaje += 1;
            }
        }

        eg->timer_caida++;
        if (eg->timer_caida > 30)
        {
            eg->timer_caida = 0;
            int nueva_y = eg->pieza_y + 1;

            if (choque_vert(eg, nueva_y))
            {
                fijar_pieza(eg);
                limpiar_y_puntuar(eg);
                spawnear_pieza(eg);
                if (choque_vert(eg, eg->pieza_y))
                    eg->estado = ESTADO_GAMEOVER;
            }
            else
            {
                eg->pieza_y = nueva_y;
            }
        }

        if (input_pausa())
            eg->estado = ESTADO_PAUSA;

        break;
    }

    case ESTADO_PAUSA:
        if (input_pausa())
            eg->estado = ESTADO_JUGANDO;
        break;

    case ESTADO_GAMEOVER:
        if (input_enter())
            eg->estado = ESTADO_MENU;
        break;
    }
}
