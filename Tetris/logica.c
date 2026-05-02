#include "logica.h"
#include "deteccion_tecla.h"
#include "GBT/gbt.h"
#include "piezas.h"

//#define TMAT 4
#include "tablero.h"

#define COLUMNAS 10
#define FILAS 20

// pieza actual
static t_pieza pieza;
static e_estado estado;

void fijar_pieza()
{
    t_pieza *pieza_act = obtener_matriz_pieza();
    for(int i=0;i<TMAT;i++)
    {
        for(int j=0;j<TMAT;j++)
        {
            if(pieza_act->tamano[i][j] == 1)
            {// Pone un 1 en cada coordenada donde la matriz tiene 1 (Para fijar tetromino y no un pixel)
                tablero_set(pieza_y+i, pieza_x+j, pieza_act->c);

            }
        }
    }
}

int choque_vert(int futura_y) {
    t_pieza *pieza_actual = obtener_matriz_pieza();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {


            if (pieza_actual->tamano[i][j] == 1) {

                int pos_y = futura_y + i;
                int pos_x = pieza_x + j;

                // VerificA si bloque en particular toca el fondo o a otro bloque
                if (pos_y >= FILAS || tablero_get(pos_y, pos_x) != 0) {
                    return 1; // colision
                }
            }
        }
    }
    return 0; // No colision
}

int choque_horiz(int futura_x)
{
    t_pieza *pieza_actual = obtener_matriz_pieza();
    for( int i=0;i<TMAT;i++)
    {
        for( int j=0; j<TMAT;j++)
        {
            if(pieza_actual->tamano[i][j] == 1)
            {
                int pos_y = pieza_y + i;
                int pos_x = futura_x + j;


                if(pos_x < 0 || pos_x <COLUMNAS || (pos_y >=0 && pos_y<FILAS && tablero_get(pos_y, pos_x) !=0 ))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}



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

void juego_actualizar(int *val)

t_pieza juego_get_pieza()
{
    return pieza;
}

e_estado juego_get_estado()
{
    return estado;
}



    // validar horizontal
    if (nueva_x >= 0 && nueva_x < COLUMNAS &&
            !tablero_get(pieza_y, nueva_x))
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

            //tablero_set(pieza_y, pieza_x, 1); // guarda un solo bloque en esta coordenada
            fijar_pieza();
            pieza_x = 4;
            pieza_y = 0;
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

    timer_caida++;
    break;

    case ESTADO_PAUSA:

        int nueva_y = pieza_y + 1;
    //COLISION VERTICAL

        if ( nueva_y >= FILAS || tablero_get(nueva_y, pieza_x) )
        if (input_pausa())
        {
            estado = ESTADO_JUGANDO;
        }

        //Fija la pieza:guarda el bloque en el tablero
            //tablero_set(pieza_y, pieza_x, 1);
            fijar_pieza();
            //Aparece nueva pieza arriba
            pieza_x = 4;
            pieza_y = 0;
        break;

    case ESTADO_GAMEOVER:

        if (input_enter())
        {
            estado = ESTADO_MENU;
        }

        break;
    }
    eGBT_Tecla t = gbt_obtener_tecla_presionada();
    if( t == GBTK_ESCAPE){
        *val = 0;
    }

    if( input_arriba() )
    {
        t_pieza *pieza_act = obtener_matriz_pieza();
        rotarPieza( pieza_act );

        //rotarPieza( pieza_act );
    }

}
