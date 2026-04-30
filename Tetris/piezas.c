#include "piezas.h"
#include <stdlib.h>
// DECLARACION DE PIEZAS
/*
typedef struct
{
    int tamano[4][4];
    int c; // color
}t_pieza; */



static t_pieza piezaI = { // La I
    {
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0} }, 11};

static t_pieza piezaJ = { // La J
            {
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0} },1};

static t_pieza piezaL = { // La L
            {
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0} },6};

static t_pieza piezaO = { // La O
            {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0} },14};

static t_pieza piezaS = { // La S
            {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0} },10};

static t_pieza piezaT = { // La T
            {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0} },5};

static t_pieza piezaZ = { // La Z
            {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0} },4};

t_pieza* obtener_matriz_pieza()
{
    /*
    int numero_pieza = rand()%7 +1; // Elementos de 1 a 7
    printf("Numero pieza:%d\n", numero_pieza);
    switch( numero_pieza )
    {
        case 1: return &piezaI;
        case 2: return &piezaJ;
        case 3: return &piezaL;
        case 4: return &piezaO;
        case 5: return &piezaS;
        case 6: return &piezaT;
        case 7: return &piezaT;
    }*/
    return &piezaI;
}

void rotarPieza(t_pieza *pieza_rotar)
{
    int temp;
    int n=4;
    for(int i= 0; i < n; i++)
    {
        for(int j=i; j<n; j++)
        {
            temp = pieza_rotar->tamano[i][j];
            pieza_rotar->tamano[i][j] = pieza_rotar->tamano[j][i]; // Transpongo la matriz(filas a columans)
            pieza_rotar->tamano[j][i] = temp;
        }
    }
    for( int i=0; i<n; i++)
    {
        for(int j=0; j<n/2;j++)
        {
            temp = pieza_rotar->tamano[i][j];
            pieza_rotar->tamano[i][j] = pieza_rotar->tamano[i][n-1-j]; // invierto la matriz. Toda la fila da vuelta horizontal
            pieza_rotar->tamano[i][n-1-j] = temp;
        }
    }


}




