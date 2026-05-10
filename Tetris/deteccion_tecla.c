/*
#include "deteccion_tecla.h"
#include "GBT/gbt.h"

static int izq, der, abajo, enter, pausa, arriba, salir;

void input_actualizar()
{
    enter  = gbt_tecla_presionada(GBTK_ENTER);
    pausa  = gbt_tecla_presionada(GBTK_p);
    //izq    = gbt_tecla_sostenida(GBTK_IZQUIERDA);
    //der    = gbt_tecla_sostenida(GBTK_DERECHA);
    abajo  = gbt_tecla_sostenida(GBTK_ABAJO);

    izq    = gbt_tecla_presionada(GBTK_IZQUIERDA);
    der    = gbt_tecla_presionada(GBTK_DERECHA);

    arriba = gbt_tecla_presionada(GBTK_ARRIBA);
    salir = gbt_tecla_presionada(GBTK_ESCAPE);
}

int input_izquierda() { return izq;    }
int input_derecha()   { return der;    }
int input_abajo()     { return abajo;  }
int input_arriba()    { return arriba; }
int input_enter()     { return enter;  }
int input_pausa()     { return pausa;  }

int input_salir()     { return salir;  }


*/
#include "deteccion_tecla.h"
#include "GBT/gbt.h"

#define DELAY_ESTANDAR 0.20 // Tiempo que el programa espera post primer movimiento. Sirve para q un toque minimo no haga repeticion
#define DELAY_REPETIR 0.08 // cada cuanto tiempo se mueve YA durante repeticion

//static int izq, der, abajo, enter, pausa, arriba, salir;
static int enter, pausa, arriba, salir;

typedef struct
{
    eGBT_Tecla tecla;
    tGBT_Temporizador *timer_delay; // Punteros a direccion de inicio del temporizador pq es lo que me devuelve la GBT
    tGBT_Temporizador *timer_repeticion; // Punteros a direccion de inicio de temporizador
    int estado;
    int espera;
}tRepeTecla;

static tRepeTecla izq;
static tRepeTecla der;
static tRepeTecla abajo;
static int enter, pausa, arriba, salir;

static void repet_inicializar(tRepeTecla *ri, eGBT_Tecla tecla)
{
    // Crea 2 temporizadores, los pausa y pone estado inicial en 0.
    ri->tecla =tecla;
    ri->timer_delay = gbt_temporizador_crear(DELAY_ESTANDAR);
    ri->timer_repeticion = gbt_temporizador_crear(DELAY_REPETIR);
    ri->estado = 0;
    ri->espera = 0;
    gbt_temporizador_pausar( ri->timer_delay);
    gbt_temporizador_pausar(ri->timer_repeticion);

}

static void repet_destruir(tRepeTecla *rd)
{
    // Libera memoria de temporizadores
    gbt_temporizador_destruir( rd->timer_delay);
    gbt_temporizador_destruir(rd->timer_repeticion);

}

static void repet_actualizar(tRepeTecla *ra)
{
    //Contiene la logica y setea estado.
    ra->estado = 0;

    if(gbt_tecla_presionada(ra->tecla) ) // Se detecta PRESION de tecla (1)
    {
        ra->estado = 1; // Se detecta presion de tecla
        gbt_temporizador_reanudar( ra->timer_delay);
        gbt_temporizador_pausar( ra->timer_repeticion);
        ra->espera = 1;
    }
    else if(gbt_tecla_sostenida(ra->tecla)) // tecla SOSTENIDA (2)
    {
        if(ra->espera) // Si esto esta en 1 es pq ya antes se utilizo gbt_tecla_presionada()
        {
            if(gbt_temporizador_consumir(ra->timer_delay)) // Si el tiempo desde la ult llamada paso los 0.20 entonces retorna un uno
            {
                // Entramos a repetir los movimientos
                ra->espera = 0;
                gbt_temporizador_pausar( ra->timer_delay);
                gbt_temporizador_reanudar( ra->timer_repeticion); // a partir de ahora vamos a empezar a repetir
            }
        }
        else
        {
            if(gbt_temporizador_consumir(ra->timer_repeticion) )
            {
                ra->estado = 1; // Se mueve cada 0,08 (ms)
            }
        }
    }
    else // (3)
    {
        // Se solto la tecla entonces se reinicia todo o ni se presiono
        gbt_temporizador_pausar( ra->timer_delay);
        gbt_temporizador_pausar( ra->timer_repeticion);
        ra->espera = 0; // se resetea para proxima presion
    }
}

void input_inicializar(){
    // Llama a autorepeat_inicializar para las 3 teclas "complicadas"
    repet_inicializar(&izq, GBTK_IZQUIERDA);
    repet_inicializar(&der, GBTK_DERECHA);
    repet_inicializar(&abajo, GBTK_ABAJO);
}

void input_finalizar(){
    // Libera todo al cerrar el juego
    repet_destruir(&izq);
    repet_destruir(&der);
    repet_destruir(&abajo);
}

void input_actualizar(){
    // Actualiza las teclas simples y las 3 "complicadas"
    arriba = gbt_tecla_presionada(GBTK_ARRIBA);
    salir = gbt_tecla_presionada(GBTK_ESCAPE);
    enter  = gbt_tecla_presionada(GBTK_ENTER);
    pausa  = gbt_tecla_presionada(GBTK_p);

    //Teclas que se pueden repetir
    repet_actualizar(&izq);
    repet_actualizar(&der);
    repet_actualizar(&abajo);
}

int input_izquierda() { return izq.estado ;    }
int input_derecha()   { return der.estado;    }
int input_abajo()     { return abajo.estado;  }
int input_arriba()    { return arriba; }
int input_enter()     { return enter;  }
int input_pausa()     { return pausa;  }
int input_salir()     { return salir;  }
