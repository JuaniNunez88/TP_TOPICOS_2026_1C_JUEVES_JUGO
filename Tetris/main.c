#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GBT/gbt.h"
#include "logica.h"
#include "deteccion_tecla.h"
#include "dibuja.h"

/*
    Apellido: Nuñez, Juan Ignacio     DNI: 44547280   Usuario: JuaniNunez88
    Apellido: Delia, Lautaro Tomas    DNI: 43722468   Usuario: Iautaro
    Apellido: Borja, Tomas Hernan     DNI: 42353302   Usuario: TomiiHB
*/

int main()
{
    gbt_iniciar();
    gbt_crear_ventana("Tetris", 320, 480, 1);
    srand((unsigned int)time(NULL));

    t_estado_juego eg;
    juego_iniciar(&eg);

    input_inicializar(); // ******
    int val = 1;
    while (val)
    {
        gbt_procesar_entrada();
        input_actualizar();
        juego_actualizar(&eg, &val);
        render_dibujar(&eg);
        gbt_esperar(16);

        printf("Puntaje: %d\n", eg.puntaje);
    }
    input_finalizar();
    gbt_cerrar();
    return 0;
}
