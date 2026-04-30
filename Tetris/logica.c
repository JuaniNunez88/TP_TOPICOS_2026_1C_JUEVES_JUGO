#include "logica.h"
#include "tablero.h"
#include "deteccion_tecla.h"
#include "GBT/gbt.h"

int pieza_x;
int pieza_y;

int timer_caida = 0;

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

    // validar horizontal (Evita que salga del tablero y atravesar bloques)
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

            tablero_set(pieza_y, pieza_x, 1);

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
        if (nueva_y >= FILAS || tablero_get(nueva_y, pieza_x))
        {

        //Fija la pieza:guarda el bloque en el tablero
            tablero_set(pieza_y, pieza_x, 1);
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
    if( t == GBTK_ARRIBA)
    {

    }
}
