#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "resolucion.h"

#define CONFIG_ARCHIVO "tetris_config.dat"

typedef enum {
    PALETA_CLASICA = 0,
    PALETA_NEON,
    PALETA_PASTEL,
    PALETA_TOTAL
} e_paleta;

typedef struct {
    int pieza[7];
    int fondo_panel;
    int borde;
    int titulo;
    int valor;
    int cursor;
    int fondo_banner;
} t_paleta;

typedef struct {
    e_resolucion resolucion;
    e_paleta     paleta;
    int          velocidad;
} t_config;


const t_paleta *config_get_paleta(const t_config *cfg);

const char *config_nombre_paleta    (e_paleta p);
const char *config_nombre_resolucion(e_resolucion r);

void config_cargar (t_config *cfg);
void config_guardar(const t_config *cfg);

#endif // CONFIG_H_INCLUDED
