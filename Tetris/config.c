#include "config.h"
#include <stdio.h>

// Indices GBT: 0=negro 1=azul 2=verde 3=cyan 4=rojo
//              5=magenta 6=marron 7=gris claro 8=gris oscuro
//              9=azul brillante 10=verde brillante 11=cyan brillante
//              12=rojo brillante 13=magenta brillante 14=amarillo 15=blanco

static const t_paleta PALETA_CLASICA_DATA =
{
    .pieza        = {11, 1, 6, 14, 10, 5, 4},
    //                I  J  L   O   S  T  Z
    .fondo_panel  = 1,
    .borde        = 8,
    .titulo       = 15,
    .valor        = 14,
    .cursor       = 11,
    .fondo_banner = 1,
};

static const t_paleta PALETA_NEON_DATA =
{
    .pieza        = {11, 9, 13, 14, 10, 12, 4},
    //                I  J   L   O   S   T  Z
    .fondo_panel  = 8,
    .borde        = 11,
    .titulo       = 14,
    .valor        = 13,
    .cursor       = 10,
    .fondo_banner = 4,
};


static const t_paleta PALETA_PASTEL_DATA =
{
    .pieza        = {3, 9, 7, 15, 2, 5, 6},
    //               I  J  L   O  S  T  Z
    .fondo_panel  = 7,
    .borde        = 8,
    .titulo       = 15,
    .valor        = 3,
    .cursor       = 13,
    .fondo_banner = 8,
};

const t_paleta *config_get_paleta(const t_config *cfg)
{
    switch (cfg->paleta)
    {
    case PALETA_NEON:
        return &PALETA_NEON_DATA;
    case PALETA_PASTEL:
        return &PALETA_PASTEL_DATA;
    default:
        return &PALETA_CLASICA_DATA;
    }
}

const char *config_nombre_paleta(e_paleta p)
{
    switch (p)
    {
    case PALETA_CLASICA:
        return "CLASICA";
    case PALETA_NEON:
        return "NEON";
    case PALETA_PASTEL:
        return "PASTEL";
    default:
        return "?";
    }
}

const char *config_nombre_resolucion(e_resolucion r)
{
    return (r == RES_CGA) ? "CGA" : "VGA";
}

static void config_defaults(t_config *cfg)
{
    cfg->resolucion = RES_VGA;
    cfg->paleta     = PALETA_CLASICA;
    cfg->velocidad  = 60;
}

void config_cargar(t_config *cfg)
{
    FILE *f = fopen(CONFIG_ARCHIVO, "rb");
    if (f == NULL)
    {
        config_defaults(cfg);
        config_guardar(cfg);
        return;
    }
    if (fread(cfg, sizeof(t_config), 1, f) != 1)
        config_defaults(cfg);
    fclose(f);
}

void config_guardar(const t_config *cfg)
{
    FILE *f = fopen(CONFIG_ARCHIVO, "wb");
    if (f == NULL) return;
    fwrite(cfg, sizeof(t_config), 1, f);
    fclose(f);
}
