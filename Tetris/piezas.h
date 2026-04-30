#ifndef PIEZAS_H_INCLUDED
#define PIEZAS_H_INCLUDED

typedef struct
{
    int tamano[4][4];
    int c; // color
}t_pieza;

//void seleccionPieza(t_pieza *tp ,int val);
//extern t_pieza piezaJ; // AGREGADO
//extern t_pieza piezaO;
t_pieza* obtener_matriz_pieza();
void rotarPieza(t_pieza *pieza_rotar);

#endif // PIEZAS_H_INCLUDED*/
