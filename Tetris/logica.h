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
    /* --- campos de dificultad --- */
    int   piezas_caidas;        /* cuantas piezas se fijaron en total        */
    float intervalo_caida_ms;   /* ms entre caidas automaticas (empieza 1000)*/
    int   timer_fijacion;       /* contador para el delay de fijacion        */
    int   esperando_fijar;      /* flag: pieza toco fondo, esperando fijar   */
    int   nivel;                /* nivel actual (sube cada 10 lineas)        */
    int   lineas_limpiadas;     /* lineas totales limpiadas                  */
} t_estado_juego;

void juego_iniciar   (t_estado_juego *eg);
void juego_actualizar(t_estado_juego *eg, int *val);
void fijar_pieza     (t_estado_juego *eg);
int  choque_vert     (t_estado_juego *eg, int futura_y);
int  choque_horiz    (t_estado_juego *eg, int futura_x);

#endif // JUEGO_H_INCLUDED
