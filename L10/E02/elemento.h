#ifndef ELEMENTO_H
#define ELEMENTO_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { avanti,
               indietro,
               transizione } tipo;

typedef enum { frontale,
               spalle } direzione;

typedef struct Elemento {
    char *Nome;
    tipo Tipo;
    direzione Ingresso;
    direzione Uscita;
    bool Precedenza;
    bool Finale;
    float Valore;
    uint8_t Difficolta;
} * elemento;

elemento parseElemento(char *string);
void freeElemento(elemento e);
elemento creaElemento();
extern const uint8_t MAX_STRING;

#endif // ELEMENTO_H
