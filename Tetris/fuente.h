#ifndef FUENTE_H_INCLUDED
#define FUENTE_H_INCLUDED

#define FUENTE_MAX_ALTO 16
#define FUENTE_CHAR_MIN 32
#define FUENTE_CHAR_MAX 90
#define FUENTE_TOTAL    (FUENTE_CHAR_MAX - FUENTE_CHAR_MIN + 1)

typedef struct {
    int ancho;
    int alto;
    const unsigned char letra[FUENTE_TOTAL][FUENTE_MAX_ALTO];
} t_fuente;

const t_fuente *fuente_get_8x8();
const t_fuente *fuente_get_8x16();

void fuente_dibujar_char (const t_fuente *f, char c, int x, int y, int color);

void fuente_dibujar_texto(const t_fuente *f, const char *texto, int x, int y, int color);

#endif // FUENTE_H_INCLUDED
