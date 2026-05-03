#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#define FILAS    20
#define COLUMNAS 10

void inicializar_tablero();
int  tablero_get(int fila, int col);
void tablero_set(int fila, int col, int valor);
int  tablero_limpiar_lineas_completas();

#endif // TABLERO_H_INCLUDED
