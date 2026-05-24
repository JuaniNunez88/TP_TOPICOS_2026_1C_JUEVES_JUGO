#include "resolucion.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CGA_ANCHO 320
#define CGA_ALTO  200
#define VGA_ANCHO 640
#define VGA_ALTO  480

#define ESCALA_MIN 1
#define ESCALA_MAX 4

static void aplicar_tipo(t_resolucion *r)
{
    if (r->tipo == RES_CGA)
    {
        r->ancho = CGA_ANCHO;
        r->alto  = CGA_ALTO;
    }
    else
    {
        r->ancho = VGA_ANCHO;
        r->alto  = VGA_ALTO;
    }
}

int resolucion_escala_default(e_resolucion tipo)
{
    return 1;//(tipo == RES_CGA) ? 2 : 1;
}

void resolucion_iniciar(t_resolucion *r)
{
    r->tipo   = RES_VGA;
    r->escala = resolucion_escala_default(RES_VGA);
    aplicar_tipo(r);
}

int resolucion_desde_args(t_resolucion *r, int argc, char *argv[])
{
    resolucion_iniciar(r);
    if (argc < 2)
        return 0;

    if (strcmp(argv[1], "CGA") == 0)
        r->tipo = RES_CGA;
    else if (strcmp(argv[1], "VGA") == 0)
        r->tipo = RES_VGA;
    else
    {
        printf("Resolucion desconocida: %s. Usando VGA.\n", argv[1]);
        return 0;
    }

    r->escala = resolucion_escala_default(r->tipo);
    aplicar_tipo(r);

    if (argc >= 3)
    {
        int escala = atoi(argv[2]);
        if (escala >= ESCALA_MIN && escala <= ESCALA_MAX)
            r->escala = escala;
        else
            printf("Escala %d fuera de rango [%d-%d]. Usando %d.\n",
                   escala, ESCALA_MIN, ESCALA_MAX, r->escala);
    }
    return 1;
}

void resolucion_set(t_resolucion *r, e_resolucion tipo)
{
    r->tipo   = tipo;
    r->escala = resolucion_escala_default(tipo);
    aplicar_tipo(r);
}

void resolucion_set_escala(t_resolucion *r, int escala)
{
    if (escala >= ESCALA_MIN && escala <= ESCALA_MAX)
        r->escala = escala;
}

int resolucion_ancho_ventana(const t_resolucion *r)
{
    return r->ancho * r->escala;
}

int resolucion_alto_ventana(const t_resolucion *r)
{
    return r->alto * r->escala;
}
