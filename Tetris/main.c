#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "GBT/gbt.h"
#include "logica.h"
#include "deteccion_tecla.h"
#include "dibuja.h"
/*
    Apellido: Nuñez, Juan Ignacio
    DNI: 44547280
    Usuario: JuaniNunez88
    Entrega: Sí

    Apellido: Delia, Lautaro Tomas
    DNI: 43722468
    Usuario: Iautaro
    Entrega: Sí

    Apellido: Borja, Tomas Hernan
    DNI: 42353302
    Usuario: TomiiHB
    Entrega: Sí
*/
int main()
{

    gbt_iniciar();
    gbt_crear_ventana("Tetris", 320, 480, 1);

    juego_iniciar();

    while (1)
    {
        gbt_procesar_entrada();//Lee el teclado del sistema
        input_actualizar(); //Guarda el estado de teclas en variables propias
        juego_actualizar(); //mueve la pieza, detecta colisiones
        render_dibujar(); //dibuja TODO en pantalla
        gbt_esperar(16); //controla la velocidad (~60 FPS)
    }
    gbt_cerrar();
    return 0;
}
