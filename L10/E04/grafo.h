#ifndef GRAFO_H
#define GRAFO_H

#include "SymbleTable.h"
#include "arco.h"
#include "matriceAdiacenze.h"
#include "shortlistadapted.h"

struct Grafo;
typedef struct Grafo *grafo;

grafo parseGrafo(char *filename);
void creaListaAdiacenze(grafo g);
grafo creaGrafo(unsigned int NumeroVertici, unsigned int NumeroArchi);

#endif // ! GRAFO_H
