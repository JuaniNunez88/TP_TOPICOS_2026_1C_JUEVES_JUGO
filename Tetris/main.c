#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GBT/gbt.h"
#include "logica.h"
#include "deteccion_tecla.h"
#include "dibuja.h"
#include "resolucion.h"
#include "fuente.h"
#include "config.h"
#include "menu.h"
#include "persona.h"

/*
    Apellido: Nuñez, Juan Ignacio     DNI: 44547280   Usuario: JuaniNunez88   Entrega: Sí
    Apellido: Delia, Lautaro Tomas    DNI: 43722468   Usuario: Iautaro         Entrega: Sí
    Apellido: Borja, Tomas Hernan     DNI: 42353302   Usuario: TomiiHB         Entrega: Sí
*/

static void recrear_ventana(const t_resolucion *res)
{
    gbt_destruir_ventana();
    gbt_crear_ventana("Tetris",
                      res->ancho,
                      res->alto,
                      res->escala);
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    t_config cfg;
    config_cargar(&cfg);

    t_stats stats;
    stats_cargar(&stats);

    t_resolucion res;
    if (!resolucion_desde_args(&res, argc, argv))
        resolucion_set(&res, cfg.resolucion);

    gbt_iniciar();
    gbt_crear_ventana("Tetris",
                      res.ancho,
                      res.alto,
                      res.escala);

    input_inicializar();

    t_menu         menu;
    t_estado_juego eg;
    int puntaje_ultimo = 0;
    int corriendo      = 1;
    int en_juego       = 0;

    menu_iniciar(&menu);

    while (corriendo)
    {
        gbt_procesar_entrada();
        input_actualizar();

        if (!en_juego)
        {
            int res_cambio = 0;
            int resultado  = menu_actualizar(&menu, &cfg, &stats,
                                             puntaje_ultimo, &res_cambio);

            if (res_cambio)
            {
                resolucion_set(&res, cfg.resolucion);
                recrear_ventana(&res);
                input_finalizar();
                input_inicializar();
                gbt_borrar_backbuffer(0);
                gbt_volcar_backbuffer();
            }

            menu_dibujar(&menu, &cfg, &stats, puntaje_ultimo);

          if (resultado == 1)
{
    eg.res = res;

    juego_iniciar(&eg);

    eg.intervalo_inicial_ms = 1600.0f - (float)(cfg.velocidad * 16);
    eg.intervalo_caida_ms   = eg.intervalo_inicial_ms;

    input_actualizar();

    en_juego = 1;
}
            else if (resultado == 2)
            {
                corriendo = 0;
            }
        }
        else
        {
            if (input_salir())
            {
                corriendo = 0;
            }
            else
            {
                int dummy = 1;
                juego_actualizar(&eg, &dummy);
                render_dibujar(&eg, &cfg, &stats);

                if (eg.estado == ESTADO_GAMEOVER)
                {
                    puntaje_ultimo = eg.puntaje;
                    stats_actualizar(&stats, puntaje_ultimo);
                    menu_ir_gameover(&menu);
                    en_juego = 0;
                }
            }
        }

        gbt_esperar(16);
    }

    config_guardar(&cfg);
    stats_guardar(&stats);
    input_finalizar();
    gbt_cerrar();
    return 0;
}
