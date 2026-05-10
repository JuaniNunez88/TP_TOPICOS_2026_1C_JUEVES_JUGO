#ifndef PIEZAS_H_INCLUDED
#define PIEZAS_H_INCLUDED

#define TMAT 4

typedef struct
{
    int tamano[TMAT][TMAT];
    int c;
} t_piezaL;

t_piezaL* elegir_pieza_aleatoria();
void rotarPieza(t_piezaL *pieza_rotar);

#endif // PIEZAS_H_INCLUDED
