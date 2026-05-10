#ifndef RESOLUCION_H_INCLUDED
#define RESOLUCION_H_INCLUDED

typedef enum {
    RES_CGA,
    RES_VGA
} e_resolucion;

typedef struct {
    e_resolucion tipo;
    int ancho;
    int alto;
    int escala;
} t_resolucion;

void resolucion_iniciar(t_resolucion *r);

int resolucion_desde_args(t_resolucion *r, int argc, char *argv[]);

void resolucion_set(t_resolucion *r, e_resolucion tipo);

void resolucion_set_escala(t_resolucion *r, int escala);

int resolucion_ancho_ventana(const t_resolucion *r);

int resolucion_alto_ventana(const t_resolucion *r);

#endif // RESOLUCION_H_INCLUDED
