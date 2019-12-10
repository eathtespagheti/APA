#include "scacchiera.h"
#include "smartfunctions.h"
#include <string.h>

// Trova tutte le tessere non ancora utilizzate e le inserisce in un array di puntatori a tessere
arrayTessera tessereDisponibili(arrayTessera *a, scacchiera *s, arrayCelle *celleDisponibili) {
    tessera *inUso[a->NumeroElementi];
    celleDisponibili->NumeroElementi = 0;
    celleDisponibili->Array          = (cella **)calloc(s->Righe * s->Colonne, sizeof(cella *));

    // Controllo le tessere in uso
    unsigned int posInUso = 0;
    for (unsigned int i = 0; i < s->Righe; i++) {             // Per ogni riga
        for (unsigned int j = 0; j < s->Colonne; j++) {       // Per ogni colonna
            if (s->Matrice[i][j].Tessera != NULL) {           // Se la cella non è vuota
                inUso[posInUso++] = s->Matrice[i][j].Tessera; // La segno tra le tessere in uso
            } else {
                celleDisponibili->Array[celleDisponibili->NumeroElementi++] = &s->Matrice[i][j]; // Segno la cella come vuota
            }
        }
    }

    // Ridimensiono le celle disponibili
    cella **temp = (cella **)calloc(celleDisponibili->NumeroElementi, sizeof(cella *));
    memcpy(temp, celleDisponibili->Array, celleDisponibili->NumeroElementi * sizeof(cella *));
    free(celleDisponibili->Array);
    celleDisponibili->Array = temp;

    arrayTessera tessereDisponibili;
    tessereDisponibili.NumeroElementi = a->NumeroElementi - posInUso;
    tessereDisponibili.Array          = (tessera **)calloc(tessereDisponibili.NumeroElementi, sizeof(tessera *));

    unsigned int posLibere = 0;
    for (unsigned int i = 0; i < a->NumeroElementi; i++) { // Per ogni tessera
        bool checkUso = false;
        for (unsigned int j = 0; j < posInUso; j++) { // Per ogni tessera in uso
            if (a->Array[i] == inUso[j]) {            // Se è in uso
                checkUso = true;
                break; // Mi interrompo
            }
        }
        if (!checkUso) {
            tessereDisponibili.Array[posLibere++] = a->Array[i]; // La segno come non in uso
        }
    }

    return tessereDisponibili;
}

// Restituisce il tubo orizzontale all'interno della cella
tubo getTuboOrizzontale(cella *c) {
    if (c->Rotazione) { // Se la tessera è ruotata
        return c->Tessera->Tubo2;
    } else { // Se non lo è
        return c->Tessera->Tubo1;
    }
}

// Restituisce il tubo verticale all'interno della cella
tubo getTuboVerticale(cella *c) {
    if (c->Rotazione) { // Se la tessera è ruotata
        return c->Tessera->Tubo1;
    } else { // Se non lo è
        return c->Tessera->Tubo2;
    }
}

// Calcola il punteggio di una scacchiera completa
unsigned int calcolaPunteggio(scacchiera *s) {
    unsigned int punteggioCelle = 0, punteggio = 0;
    char colorePrecedente;

    // Controllo tutte le righe
    for (unsigned int i = 0; i < s->Righe; i++) { // Per ogni riga
        colorePrecedente = getTuboOrizzontale(&s->Matrice[i][0]).Colore;
        punteggioCelle   = 0;
        for (unsigned int j = 0; j < s->Colonne; j++) { // Per ogni colonna
            tubo t = getTuboOrizzontale(&s->Matrice[i][j]);

            if (t.Colore == colorePrecedente) { // Se ha lo stesso colore del precedente
                punteggioCelle += t.Valore;
            } else {
                punteggioCelle = 0;
                break;
            }
        }
        punteggio += punteggioCelle;
    }

    // Controllo tutte le colonne
    for (unsigned int j = 0; j < s->Colonne; j++) { // Per ogni colonna
        colorePrecedente = getTuboVerticale(&s->Matrice[0][j]).Colore;
        punteggioCelle   = 0;
        for (unsigned int i = 0; i < s->Righe; i++) { // Per ogni riga
            tubo t = getTuboVerticale(&s->Matrice[i][j]);

            if (t.Colore == colorePrecedente) { // Se ha lo stesso colore del precedente
                punteggioCelle += t.Valore;
            } else {
                punteggioCelle = 0;
                break;
            }
        }
        punteggio += punteggioCelle;
    }

    return punteggio;
}

int main(int argc, char const *argv[]) {
    FILE *tilesStream = smartFopen("data/tiles.txt", "r");
    unsigned int numeroTessere;
    fscanf(tilesStream, "%d\n", &numeroTessere);

    puts("Le tessere presenti sono:");
    arrayTessera a = parseArrayTessera(tilesStream, numeroTessere);
    printArrayTessera(&a, stdout);

    puts("\nLa scacchiera è:");
    FILE *boardStream = smartFopen("data/board.txt", "r");
    scacchiera s;
    parseScacchiera(&s, boardStream, &a);
    printScacchiera(&s, stdout);

    puts("\nLe tessere libere sono:");
    arrayCelle celleDisponibili;
    arrayTessera disponibili = tessereDisponibili(&a, &s, &celleDisponibili);
    printArrayTessera(&disponibili, stdout);

    printf("E sono presenti %d celle libere nella scacchiera\n", celleDisponibili.NumeroElementi);
    return 0;
}
