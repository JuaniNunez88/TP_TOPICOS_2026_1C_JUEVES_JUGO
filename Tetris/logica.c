#include "logica.h"
#include "deteccion_tecla.h"
#include "tablero.h"
#include "piezas.h"
#include "GBT/gbt.h"

#define COLUMNAS 10
#define FILAS    20

/* Cada cuantas lineas limpiadas sube el nivel */
#define LINEAS_POR_NIVEL 10

/* Intervalo inicial de caida en ms */
#define INTERVALO_INICIAL 1000.0f

/* Cada frame el loop espera 16 ms (~62 fps).
   Se usa para convertir ms -> frames. */
#define MS_POR_FRAME 16.0f

/* ---------- funciones internas ---------- */

/*
 * Calcula el intervalo de caida segun cuantas piezas cayeron.
 * Baja 3% por cada grupo de 10 piezas fijadas.
 */
static float calcular_intervalo(int nivel)
{
    float intervalo = INTERVALO_INICIAL;

    for (int i = 1; i < nivel; i++)
        intervalo *= 0.80f;
    return intervalo;
}

/*
 * Devuelve el nivel actual segun lineas limpiadas totales.
 * Sube 1 nivel cada LINEAS_POR_NIVEL lineas.
 * Nivel minimo: 1.
 */
static int calcular_nivel(int lineas_limpiadas)
{
    return (lineas_limpiadas / LINEAS_POR_NIVEL) + 1;
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

/*
 * Limpia lineas completas, acumula lineas totales,
 * recalcula nivel e intervalo, y suma puntaje escalado por nivel.
 */
static void limpiar_y_puntuar(t_estado_juego *eg)
{
    int lineas = tablero_limpiar_lineas_completas();
    if (lineas > 0)
    {
        eg->lineas_limpiadas += lineas;
        eg->nivel             = calcular_nivel(eg->lineas_limpiadas);

        /* Puntaje base por lineas * nivel actual */
        eg->puntaje += ((lineas * 100) + ((lineas - 1) * 50)) * eg->nivel;
        eg->intervalo_caida_ms = calcular_intervalo(eg->nivel);
    }
}

/*
 * Se llama cada vez que una pieza se fija definitivamente.
 * Actualiza contador, recalcula intervalo y spawnea la siguiente.
 */
static void confirmar_fijacion(t_estado_juego *eg)
{
    fijar_pieza(eg);
    limpiar_y_puntuar(eg);

    eg->piezas_caidas++;
    eg->intervalo_caida_ms = calcular_intervalo(eg->nivel);

    eg->esperando_fijar = 0;
    eg->timer_fijacion  = 0;

    spawnear_pieza(eg);
    if (choque_vert(eg, eg->pieza_y))
        eg->estado = ESTADO_GAMEOVER;
}

/* ---------- API publica ---------- */

void juego_iniciar(t_estado_juego *eg)
{
    eg->estado             = ESTADO_MENU;
    eg->puntaje            = 0;
    eg->juego_terminado    = 0;
    eg->timer_caida        = 0;
    eg->pieza_siguiente    = NULL;
    eg->piezas_caidas      = 0;
    eg->intervalo_caida_ms = INTERVALO_INICIAL;
    eg->timer_fijacion     = 0;
    eg->esperando_fijar    = 0;
    eg->nivel              = 1;
    eg->lineas_limpiadas   = 0;
    inicializar_tablero();
    spawnear_pieza(eg);
}

void juego_actualizar(t_estado_juego *eg, int *val)
{
    if (input_salir())
    {
        *val = 0;
        return;
    }

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
            eg->intervalo_caida_ms = INTERVALO_INICIAL;
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
        /* Movimiento horizontal */
        int nueva_x = eg->pieza_x;
        if (input_derecha())   nueva_x++;
        if (input_izquierda()) nueva_x--;
        if (!choque_horiz(eg, nueva_x))
            eg->pieza_x = nueva_x;

        /* Rotacion */
        if (input_arriba())
            rotarPieza(eg->pieza_actual);

        /*
         * Caida manual (tecla abajo).
         * Si hay colision al bajar: inicia espera de fijacion (no fija directo).
         * Si no hay colision: baja y suma 1 punto extra escalado por nivel.
         */
        if (input_abajo())
        {
            int nueva_y = eg->pieza_y + 1;
            if (choque_vert(eg, nueva_y))
            {
                /* Forzar fijacion inmediata si el jugador empuja hacia abajo */
                confirmar_fijacion(eg);
            }
            else
            {
                eg->esperando_fijar = 0;
                eg->timer_fijacion  = 0;
                eg->pieza_y = nueva_y;
                eg->puntaje += eg->nivel; /* puntos por caida manual, escala con nivel */
            }
        }

        /*
         * Caida automatica.
         * Se convierte intervalo_caida_ms a frames (cada frame = 16 ms).
         * El delay de fijacion es el 50% del intervalo de caida.
         */
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
                /* Toca fondo: iniciar espera de fijacion si no estaba ya */
                if (!eg->esperando_fijar)
                {
                    eg->esperando_fijar = 1;
                    eg->timer_fijacion  = 0;
                }
            }
            else
            {
                /* Puede seguir bajando: cancela cualquier espera previa */
                eg->esperando_fijar = 0;
                eg->timer_fijacion  = 0;
                eg->pieza_y = nueva_y;
            }
        }

        /* Espera de fijacion: el 50% del intervalo de caida */
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
