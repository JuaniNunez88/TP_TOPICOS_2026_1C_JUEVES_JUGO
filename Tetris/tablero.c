#include "tablero.h"
#include "stdio.h"

static int matriz_tablero [FILAS][COLUMNAS];

int i,j;

void inicializar_tablero()
{
    for (i=0; i<FILAS; i++)
        for(j=0; j<COLUMNAS; j++)
            matriz_tablero[i][j]=0;

    printf("--- MATRIZ ---\n");
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", matriz_tablero[i][j]);
        }
        printf("\n");
    }
}
