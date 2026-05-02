#include "logica.h"
#include "deteccion_tecla.h"
#include "tablero.h"

#define COLUMNAS 10
#define FILAS 20

// pieza actual
static t_pieza pieza;
static e_estado estado;

// timer para la caída automática
static int timer_caida = 0;

// ------------------------
// INICIALIZAR JUEGO
// ------------------------
void juego_iniciar()
{
    estado = ESTADO_MENU;

    pieza.x = 4;
    pieza.y = 0;

    timer_caida = 0;

    inicializar_tablero();
}


t_pieza juego_get_pieza()
{
    return pieza;
}

e_estado juego_get_estado()
{
    return estado;
}

// ------------------------
// ACTUALIZAR JUEGO
// ------------------------
void juego_actualizar()
{
    switch (estado)
    {
    case ESTADO_MENU:

        if (input_enter())
        {
            estado = ESTADO_JUGANDO;
            inicializar_tablero();

            pieza.x = 4;
            pieza.y = 0;
        }

        break;

    case ESTADO_JUGANDO:
    {
        // ------------------------
        // MOVIMIENTO HORIZONTAL
        // ------------------------
        int nueva_x = pieza.x;

        if (input_derecha()) nueva_x++;
        if (input_izquierda()) nueva_x--;

        if (nueva_x >= 0 && nueva_x < COLUMNAS &&
                !tablero_get(pieza.y, nueva_x))
        {
            pieza.x = nueva_x;
        }

        // ------------------------
        // BAJADA MANUAL
        // ------------------------
        if (input_abajo())
        {
            int nueva_y = pieza.y + 1;

            if (nueva_y >= FILAS || tablero_get(nueva_y, pieza.x))
            {
                tablero_set(pieza.y, pieza.x, 1);

                // nueva pieza
                pieza.x = 4;
                pieza.y = 0;

                // GAME OVER
                if (tablero_get(pieza.y, pieza.x))
                {
                    estado = ESTADO_GAMEOVER;
                }
            }
            else
            {
                pieza.y = nueva_y;
            }
        }

        // ------------------------
        // CAÍDA AUTOMÁTICA
        // ------------------------
        timer_caida++;

        if (timer_caida > 30)
        {
            int nueva_y = pieza.y + 1;

            if (nueva_y >= FILAS || tablero_get(nueva_y, pieza.x))
            {
                tablero_set(pieza.y, pieza.x, 1);

                // nueva pieza
                pieza.x = 4;
                pieza.y = 0;

                // GAME OVER
                // verificar toda la fila superior
                for (int j = 0; j < COLUMNAS; j++)
                {
                    if (tablero_get(0, j))
                    {
                        estado = ESTADO_GAMEOVER;
                    }
                }
            }
            else
            {
                pieza.y = nueva_y;
            }

            timer_caida = 0;
        }

        // ------------------------
        // PAUSA
        // ------------------------
        if (input_pausa())
        {
            estado = ESTADO_PAUSA;
        }

    }
    break;

    case ESTADO_PAUSA:

        if (input_pausa())
        {
            estado = ESTADO_JUGANDO;
        }

        break;

    case ESTADO_GAMEOVER:

        if (input_enter())
        {
            estado = ESTADO_MENU;
        }

        break;
    }
}
