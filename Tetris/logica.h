#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

void juego_iniciar();
void juego_actualizar();

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
