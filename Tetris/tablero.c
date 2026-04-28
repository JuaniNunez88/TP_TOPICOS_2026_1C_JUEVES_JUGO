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
