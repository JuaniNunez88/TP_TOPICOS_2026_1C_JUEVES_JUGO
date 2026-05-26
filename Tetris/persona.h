#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

#define STATS_ARCHIVO    "tetris_stats.dat"
#define STATS_MAX_NOMBRE 16

typedef struct
{
    char nombre[STATS_MAX_NOMBRE + 1];
    int  puntaje_max;
    char nombre_max[STATS_MAX_NOMBRE + 1];
} t_stats;

void stats_cargar   (t_stats *s);
void stats_guardar  (const t_stats *s);
void stats_actualizar(t_stats *s, int puntaje);

#endif // STATS_H_INCLUDED
