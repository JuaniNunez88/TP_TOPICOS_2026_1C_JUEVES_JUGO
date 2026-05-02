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
extern int puntaje;
extern int juego_terminado;
typedef struct {
    int x;
    int y;
} t_pieza;

t_pieza juego_get_pieza();


typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAMEOVER
} e_estado;

e_estado juego_get_estado();

#endif // JUEGO_H_INCLUDED
