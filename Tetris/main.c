#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "GBT/gbt.h"
#include "juego.h"
#include "deteccion_tecla.h"
#include "render.h"
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
int main() {

    gbt_iniciar();
    gbt_crear_ventana("Tetris", 320, 480, 1);

    juego_iniciar();

    while (1) {
        input_actualizar();
        juego_actualizar();
        render_dibujar();
        gbt_esperar(100);
    }
    gbt_cerrar();
    return 0;
}
