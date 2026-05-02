#include "tablero.h"
#include "stdio.h"

static int matriz_tablero [FILAS][COLUMNAS];

int i,j;

void inicializar_tablero()
{
    for (i=0; i<FILAS; i++)
        for(j=0; j<COLUMNAS; j++)
            matriz_tablero[i][j]=0;
}
//Consulta si hay bloques
int tablero_get(int fila, int col) {
    return matriz_tablero[fila][col];
}
//Guarda bloques
void tablero_set(int fila, int col, int valor) {
    matriz_tablero[fila][col] = valor;
}

int tablero_limpiar_lineas_completas()
{
    int i, j,k , linea_llena = 0;
    int lineas_limpiadas = 0;

    for(i = FILAS - 1; i >= 0; i--)
    {
        linea_llena = 1;
        for(j = 0; j < COLUMNAS; j++)
        {
            if(tablero_get(i,j) == 0)
            {
                linea_llena = 0;
                break; //evitar el break
            }
        }

        if(linea_llena)
        {
            lineas_limpiadas++;

            for(k = i; k > 0; k--)
            {
                for(j = 0; j < COLUMNAS; j++)
                {
                    tablero_set(k,j,tablero_get(k-1,j));
                }
            }

            for(int j = 0; j < COLUMNAS; j++)
            {
                tablero_set(0, j, 0);
            }
            i++; //para revisar la fila de nuevo, porque movimos datos de arriba a bajo.
        }
    }
    return lineas_limpiadas;
}
