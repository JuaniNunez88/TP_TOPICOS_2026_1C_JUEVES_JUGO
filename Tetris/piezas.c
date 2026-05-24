#include "piezas.h"
#include <stdlib.h>

static t_piezaL piezaI = {
    {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, 11, 0, true};
static t_piezaL piezaJ = {
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},  1, 1, true};
static t_piezaL piezaL = {
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},  6, 2, true};
static t_piezaL piezaO = {
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, 14, 3, false};
static t_piezaL piezaS = {
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, 10, 4, true};
static t_piezaL piezaT = {
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},  5, 5, true};
static t_piezaL piezaZ = {
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},  4, 6, true};

t_piezaL *elegir_pieza_aleatoria()
{
    switch (rand() % 7)
    {
        case 0: return &piezaI;
        case 1: return &piezaJ;
        case 2: return &piezaL;
        case 3: return &piezaO;
        case 4: return &piezaS;
        case 5: return &piezaT;
        case 6: return &piezaZ;
    }
    return &piezaL;
}

void rotarPieza(t_piezaL *pieza_rotar)
{
    int temp;
    if (!pieza_rotar->rotar) return;

    // Transponer
    for (int i = 0; i < TMAT; i++)
        for (int j = i + 1; j < TMAT; j++)
        {
            temp = pieza_rotar->tamano[i][j];
            pieza_rotar->tamano[i][j] = pieza_rotar->tamano[j][i];
            pieza_rotar->tamano[j][i] = temp;
        }
    // Invertir columnas
    for (int i = 0; i < TMAT; i++)
        for (int j = 0; j < TMAT / 2; j++)
        {
            temp = pieza_rotar->tamano[i][j];
            pieza_rotar->tamano[i][j] = pieza_rotar->tamano[i][TMAT - 1 - j];
            pieza_rotar->tamano[i][TMAT - 1 - j] = temp;
        }
}


void rotarPiezaIzq( t_piezaL *pieza_rotar) // rotacion izq
{
    int temp;
    if(!pieza_rotar->rotar) return;

    for(int i = 0; i < TMAT; i++)
    {
        for(int j=i+1; j<TMAT; j++)
            {
                temp = pieza_rotar->tamano[i][j];
                pieza_rotar->tamano[i][j] = pieza_rotar->tamano[j][i];
                pieza_rotar->tamano[j][i] = temp;
            }
    }
    for(int j=0; j<TMAT; j++)
        {
            for(int i=0; i<TMAT/2; i++)
            {
                temp = pieza_rotar->tamano[i][j];
                pieza_rotar->tamano[i][j] = pieza_rotar->tamano[TMAT-1-i][j];
                pieza_rotar->tamano[TMAT-1-i][j] = temp;
            }
        }

}
