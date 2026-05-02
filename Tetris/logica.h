#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#define TMAT 4
void juego_iniciar();
void juego_actualizar(int *val);
void fijar_pieza();
int choque_vert(int futura_y); // En desarrollo
int choque_horiz(int futura_x); // En desarrollo

extern int pieza_x;
extern int pieza_y;


#endif // JUEGO_H_INCLUDED
