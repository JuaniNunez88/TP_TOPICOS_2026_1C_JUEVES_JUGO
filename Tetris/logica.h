#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "piezas.h"
#include "resolucion.h"

typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAMEOVER
} e_estado;

typedef struct {
    int       pieza_x;
    int       pieza_y;
    int       puntaje;
    int       juego_terminado;
    e_estado  estado;
    int       timer_caida;
    t_piezaL *pieza_actual;
    t_piezaL *pieza_siguiente;
    t_resolucion res;

    int   piezas_caidas;
    float intervalo_caida_ms;
    int   timer_fijacion;
    int   esperando_fijar;
    int   nivel;
    int   lineas_limpiadas;
} t_estado_juego;

void juego_iniciar   (t_estado_juego *eg);
void juego_actualizar(t_estado_juego *eg, int *val);
void fijar_pieza     (t_estado_juego *eg);
int  choque_vert     (t_estado_juego *eg, int futura_y);
int  choque_horiz    (t_estado_juego *eg, int futura_x);

#endif // JUEGO_H_INCLUDED
