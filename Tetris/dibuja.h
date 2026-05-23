#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "logica.h"
#include "config.h"
#include "persona.h"

void render_dibujar(t_estado_juego *eg, const t_config *cfg,
                    const t_stats *stats);

#endif // RENDER_H_INCLUDED
