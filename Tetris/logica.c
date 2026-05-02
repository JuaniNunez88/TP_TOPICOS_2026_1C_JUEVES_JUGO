#include "logica.h"
#include "tablero.h"
#include "deteccion_tecla.h"
#include "GBT/gbt.h"
#include "piezas.h"

//#define TMAT 4

int pieza_x;
int pieza_y;

int timer_caida = 0;

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



void juego_iniciar()
{
    pieza_x = 4;
    pieza_y = 0;
    timer_caida = 0;
}

void juego_actualizar(int *val)
{
    // MOVIMIENTO HORIZONTAL
    int nueva_x = pieza_x;

    if (input_derecha()) nueva_x++;
    if (input_izquierda()) nueva_x--;



    // validar horizontal
    if (nueva_x >= 0 && nueva_x < COLUMNAS &&
            !tablero_get(pieza_y, nueva_x))
    {
        pieza_x = nueva_x;
    }

    // Baja la pieza con la flecha hacia abajo no por gravedad
    if (input_abajo())
    {
        int nueva_y = pieza_y + 1;

        if (nueva_y >= FILAS || tablero_get(nueva_y, pieza_x))
        {

            //tablero_set(pieza_y, pieza_x, 1); // guarda un solo bloque en esta coordenada
            fijar_pieza();
            pieza_x = 4;
            pieza_y = 0;

        }
        else
        {
            pieza_y = nueva_y;
        }
    }

    timer_caida++;

    if (timer_caida > 30)
    {

        int nueva_y = pieza_y + 1;
    //COLISION VERTICAL

        if ( nueva_y >= FILAS || tablero_get(nueva_y, pieza_x) )
        {

        //Fija la pieza:guarda el bloque en el tablero
            //tablero_set(pieza_y, pieza_x, 1);
            fijar_pieza();
            //Aparece nueva pieza arriba
            pieza_x = 4;
            pieza_y = 0;

        }
        else
        {
            pieza_y = nueva_y;
        }

        timer_caida = 0;
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
