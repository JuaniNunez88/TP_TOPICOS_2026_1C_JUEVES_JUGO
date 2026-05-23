#include "logica.h"
#include "deteccion_tecla.h"
#include "tablero.h"
#include "piezas.h"
#include "GBT/gbt.h"

#define COLUMNAS 10
#define FILAS    20
#define LINEAS_POR_NIVEL  10
#define INTERVALO_INICIAL 1000.0f
#define MS_POR_FRAME      16.0f


static float calcular_intervalo(int piezas_caidas)
{
    float intervalo = INTERVALO_INICIAL;
    int reducciones = piezas_caidas / 10;

    for (int i = 1; i < reducciones; i++)
        intervalo *= 0.97f;
    return intervalo;
}
int calcular_nivel(int piezas_caidas)
{
    return (piezas_caidas / 10) + 1;
}

static void spawnear_pieza(t_estado_juego *eg)
{
    if (eg->pieza_siguiente == NULL)
        eg->pieza_actual = elegir_pieza_aleatoria();
    else
        eg->pieza_actual = eg->pieza_siguiente;

    eg->pieza_siguiente = elegir_pieza_aleatoria();
    eg->pieza_x = 4;
    eg->pieza_y = 0;
}

void fijar_pieza(t_estado_juego *eg)
{
    for (int i = 0; i < TMAT; i++)
        for (int j = 0; j < TMAT; j++)
            if (eg->pieza_actual->tamano[i][j] == 1)
                tablero_set(eg->pieza_y + i, eg->pieza_x + j,
                            eg->pieza_actual->tipo + 1);
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
                    (pos_y >= 0 && pos_y < FILAS &&
                     tablero_get(pos_y, pos_x) != 0))
                    return 1;
            }
    return 0;
}

static void limpiar_y_puntuar(t_estado_juego *eg)
{
    int lineas = tablero_limpiar_lineas_completas();
    if (lineas > 0)
    {
        eg->lineas_limpiadas += lineas;

        eg->puntaje += ((lineas * 100) + ((lineas - 1) * 50)) * eg->nivel;
    }
}

static void confirmar_fijacion(t_estado_juego *eg)
{
    fijar_pieza(eg);
    limpiar_y_puntuar(eg);

    eg->piezas_caidas++;
    eg->nivel = calcular_nivel(eg->piezas_caidas);

    eg->intervalo_caida_ms = calcular_intervalo(eg->piezas_caidas);

    eg->esperando_fijar = 0;
    eg->timer_fijacion  = 0;

    spawnear_pieza(eg);
    if (choque_vert(eg, eg->pieza_y))
        eg->estado = ESTADO_GAMEOVER;
}

void juego_iniciar(t_estado_juego *eg)
{
    eg->estado               = ESTADO_MENU;
    eg->puntaje              = 0;
    eg->juego_terminado      = 0;
    eg->timer_caida          = 0;
    eg->pieza_siguiente      = NULL;
    eg->piezas_caidas        = 0;
    eg->intervalo_inicial_ms = INTERVALO_INICIAL;
    eg->intervalo_caida_ms   = INTERVALO_INICIAL;
    eg->timer_fijacion       = 0;
    eg->esperando_fijar      = 0;
    eg->nivel                = 1;
    eg->lineas_limpiadas     = 0;
    inicializar_tablero();
    spawnear_pieza(eg);
}

static void validar_rotacion(t_estado_juego *p)
{
    rotarPieza(p->pieza_actual); // Primero la rota
    if( !choque_horiz(p,p->pieza_x) && !choque_vert(p,p->pieza_y)) // si no caen fuera del tablero entonces sale
    {
        return;
    }
    int mover_lugares[] = {1,-1,2,-2}; // Para derecha y dsps izquierda
    for(int k = 0; k<4; k++)
    {
        int nueva_x = p->pieza_x + mover_lugares[k];
        if(!choque_horiz(p, nueva_x) && !choque_vert(p, p->pieza_y))
        {
            p->pieza_x = nueva_x;
            return;
        }
    }
    rotarPieza(p->pieza_actual); // Volver a estado original
    rotarPieza(p->pieza_actual);
    rotarPieza(p->pieza_actual);
}



/*void juego_actualizar(t_estado_juego *eg, int *val)
{
    rotarPieza(eg->pieza_actual);
    if (!choque_horiz(eg, eg->pieza_x) && !choque_vert(eg, eg->pieza_y))
        return;

    int mover_lugares[] = {1, -1, 2, -2};
    for (int k = 0; k < 4; k++)
    {
        int nueva_x = eg->pieza_x + mover_lugares[k];
        if (!choque_horiz(eg, nueva_x) && !choque_vert(eg, eg->pieza_y))
        {
            eg->pieza_x = nueva_x;
            return;
        }
    }
    rotarPieza(eg->pieza_actual);
    rotarPieza(eg->pieza_actual);
    rotarPieza(eg->pieza_actual);
}*/

void juego_actualizar(t_estado_juego *eg, int *val)
{
    if (input_salir()) { *val = 0; return; }

    switch (eg->estado)
    {
    case ESTADO_MENU:
        if (input_enter())
        {
            inicializar_tablero();
            eg->puntaje            = 0;
            eg->juego_terminado    = 0;
            eg->timer_caida        = 0;
            eg->pieza_siguiente    = NULL;
            eg->piezas_caidas      = 0;
            eg->intervalo_caida_ms = eg->intervalo_inicial_ms;
            eg->timer_fijacion     = 0;
            eg->esperando_fijar    = 0;
            eg->nivel              = 1;
            eg->lineas_limpiadas   = 0;
            spawnear_pieza(eg);
            eg->estado = ESTADO_JUGANDO;
        }
        break;

    case ESTADO_JUGANDO:
    {
        int movio = 0;
        int nueva_x = eg->pieza_x;
        if (input_derecha())   nueva_x++;
        if (input_izquierda()) nueva_x--;

        if (nueva_x != eg->pieza_x && !choque_horiz(eg, nueva_x))
        {
            movio = 1;
            eg->pieza_x = nueva_x;
        }


        if (input_arriba()) // No verifica por eso falla !!!
        {
            validar_rotacion(eg);
            movio = 1;
        }
            //rotarPieza(eg->pieza_actual);


        if (input_abajo())
        {
            int nueva_y = eg->pieza_y + 1;
            if (choque_vert(eg, nueva_y))
                confirmar_fijacion(eg);
            else
            {
                eg->esperando_fijar = 0;
                eg->timer_fijacion  = 0;
                eg->pieza_y         = nueva_y;
                eg->puntaje        += eg->nivel;
            }
        }

        int frames_caida    = (int)(eg->intervalo_caida_ms / MS_POR_FRAME);
        int frames_fijacion = (int)((eg->intervalo_caida_ms * 0.5f) / MS_POR_FRAME);
        if (frames_caida    < 1) frames_caida    = 1;
        if (frames_fijacion < 1) frames_fijacion = 1;

        eg->timer_caida++;
        if (eg->timer_caida >= frames_caida)
        {
            eg->timer_caida = 0;
            int nueva_y = eg->pieza_y + 1;
            if (choque_vert(eg, nueva_y))
            {
                if (!eg->esperando_fijar)
                {
                    eg->esperando_fijar = 1;
                    eg->timer_fijacion  = 0;
                }
            }
            else
            {
                eg->esperando_fijar = 0;
                eg->timer_fijacion  = 0;
                eg->pieza_y         = nueva_y;
            }
        }

        if (eg->esperando_fijar && movio)
        {
            eg->timer_fijacion = 0;
        }

        if (eg->esperando_fijar)
        {
            eg->timer_fijacion++;
            if (eg->timer_fijacion >= frames_fijacion)
                confirmar_fijacion(eg);
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
