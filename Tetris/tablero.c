#include "tablero.h"

static int matriz_tablero[FILAS][COLUMNAS];

void inicializar_tablero()
{
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            matriz_tablero[i][j] = 0;
}

int tablero_get(int fila, int col)
{
    return matriz_tablero[fila][col];
}

void tablero_set(int fila, int col, int valor)
{
    matriz_tablero[fila][col] = valor;
}

int tablero_limpiar_lineas_completas()
{
    int lineas_limpiadas = 0;

    for (int i = FILAS - 1; i >= 0; i--)
    {
        int linea_llena = 1;
        for (int j = 0; j < COLUMNAS; j++)
            if (tablero_get(i, j) == 0) { linea_llena = 0; break; }

        if (linea_llena)
        {
            lineas_limpiadas++;

            for (int k = i; k > 0; k--)
                for (int j = 0; j < COLUMNAS; j++)
                    tablero_set(k, j, tablero_get(k - 1, j));

            for (int j = 0; j < COLUMNAS; j++)
                tablero_set(0, j, 0);

            i++;
        }
    }

    return lineas_limpiadas;
}
