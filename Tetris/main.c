#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GBT/gbt.h"
#include "logica.h"
#include "deteccion_tecla.h"
#include "dibuja.h"
#include "resolucion.h"
#include "fuente.h"

/*
    Apellido: Nuñez, Juan Ignacio     DNI: 44547280   Usuario: JuaniNunez88   Entrega: Sí
    Apellido: Delia, Lautaro Tomas    DNI: 43722468   Usuario: Iautaro         Entrega: Sí
    Apellido: Borja, Tomas Hernan     DNI: 42353302   Usuario: TomiiHB         Entrega: Sí
*/


int main(int argc, char *argv[])
{
    t_resolucion res;
    resolucion_desde_args(&res, argc, argv);

    gbt_iniciar();
    gbt_crear_ventana("Tetris",
                      resolucion_ancho_ventana(&res),
                      resolucion_alto_ventana(&res),
                      1);

    srand((unsigned int)time(NULL));

    t_estado_juego eg;
    eg.res = res;
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
    }
    input_finalizar();
    gbt_cerrar();
    return 0;
}
