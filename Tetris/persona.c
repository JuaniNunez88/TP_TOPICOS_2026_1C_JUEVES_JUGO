#include "persona.h"
#include <stdio.h>
#include <string.h>

void stats_cargar(t_stats *s)
{
    FILE *f = fopen(STATS_ARCHIVO, "rb");
    if (f == NULL)
    {
        memset(s->nombre,     0, sizeof(s->nombre));
        memset(s->nombre_max, 0, sizeof(s->nombre_max));
        s->puntaje_max = 0;
        return;
    }
    if (fread(s, sizeof(t_stats), 1, f) != 1)
    {
        memset(s->nombre,     0, sizeof(s->nombre));
        memset(s->nombre_max, 0, sizeof(s->nombre_max));
        s->puntaje_max = 0;
    }
    fclose(f);
}

void stats_guardar(const t_stats *s)
{
    FILE *f = fopen(STATS_ARCHIVO, "wb");
    if (f == NULL) return;
    fwrite(s, sizeof(t_stats), 1, f);
    fclose(f);
}

void stats_actualizar(t_stats *s, int puntaje)
{
    if (puntaje > s->puntaje_max)
    {
        s->puntaje_max = puntaje;
        strncpy(s->nombre_max, s->nombre, STATS_MAX_NOMBRE);
        s->nombre_max[STATS_MAX_NOMBRE] = '\0';
        stats_guardar(s);
    }
}
