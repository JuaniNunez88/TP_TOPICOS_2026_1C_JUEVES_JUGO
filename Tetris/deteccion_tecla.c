#include "deteccion_tecla.h"
#include "GBT/gbt.h"

#define DELAY_ESTANDAR 0.20
#define DELAY_REPETIR  0.08

typedef struct
{
    eGBT_Tecla        tecla;
    tGBT_Temporizador *timer_delay;
    tGBT_Temporizador *timer_repeticion;
    int estado;
    int espera;
} tRepeTecla;

static tRepeTecla izq;
static tRepeTecla der;
static tRepeTecla abajo;

static int enter, pausa, salir, rotarIzq, rotarDer;
static int press_arriba, press_abajo, press_izq, press_der;

static void repet_inicializar(tRepeTecla *ri, eGBT_Tecla tecla)
{
    ri->tecla            = tecla;
    ri->timer_delay      = gbt_temporizador_crear(DELAY_ESTANDAR);
    ri->timer_repeticion = gbt_temporizador_crear(DELAY_REPETIR);
    ri->estado           = 0;
    ri->espera           = 0;
    gbt_temporizador_pausar(ri->timer_delay);
    gbt_temporizador_pausar(ri->timer_repeticion);
}

static void repet_destruir(tRepeTecla *rd)
{
    gbt_temporizador_destruir(rd->timer_delay);
    gbt_temporizador_destruir(rd->timer_repeticion);
}

static void repet_actualizar(tRepeTecla *ra)
{
    ra->estado = 0;
    if (gbt_tecla_presionada(ra->tecla))
    {
        ra->estado = 1;
        gbt_temporizador_reanudar(ra->timer_delay);
        gbt_temporizador_pausar(ra->timer_repeticion);
        ra->espera = 1;
    }
    else if (gbt_tecla_sostenida(ra->tecla))
    {
        if (ra->espera)
        {
            if (gbt_temporizador_consumir(ra->timer_delay))
            {
                ra->espera = 0;
                gbt_temporizador_pausar(ra->timer_delay);
                gbt_temporizador_reanudar(ra->timer_repeticion);
            }
        }
        else
        {
            if (gbt_temporizador_consumir(ra->timer_repeticion))
                ra->estado = 1;
        }
    }
    else
    {
        gbt_temporizador_pausar(ra->timer_delay);
        gbt_temporizador_pausar(ra->timer_repeticion);
        ra->espera = 0;
    }
}

void input_inicializar()
{
    repet_inicializar(&izq,   GBTK_IZQUIERDA);
    repet_inicializar(&der,   GBTK_DERECHA);
    repet_inicializar(&abajo, GBTK_ABAJO);
}

void input_finalizar()
{
    repet_destruir(&izq);
    repet_destruir(&der);
    repet_destruir(&abajo);
}

void input_actualizar()
{
    rotarDer = gbt_tecla_presionada(GBTK_c);
    salir  = gbt_tecla_presionada(GBTK_ESCAPE);
    enter  = gbt_tecla_presionada(GBTK_ENTER);
    pausa  = gbt_tecla_presionada(GBTK_p);
    rotarIzq = gbt_tecla_presionada(GBTK_z);

    press_arriba = gbt_tecla_presionada(GBTK_ARRIBA);
    press_abajo  = gbt_tecla_presionada(GBTK_ABAJO);
    press_izq    = gbt_tecla_presionada(GBTK_IZQUIERDA);
    press_der    = gbt_tecla_presionada(GBTK_DERECHA);

    repet_actualizar(&izq);
    repet_actualizar(&der);
    repet_actualizar(&abajo);
}

int input_izquierda()      { return izq.estado;   }
int input_derecha()        { return der.estado;   }
int input_abajo()          { return abajo.estado; }
int input_enter()          { return enter;        }
int input_pausa()          { return pausa;        }
int input_salir()          { return salir;        }
int input_arriba_press()   { return press_arriba; }
int input_abajo_press()    { return press_abajo;  }
int input_izquierda_press(){ return press_izq;    }
int input_derecha_press()  { return press_der;    }

int input_rotarIzq() { return rotarIzq; }
int input_rotarDer() { return rotarDer; }
