#include "diagonale.h"

static const uint8_t MAX_ELEMENTI = 5;

// Calcola e restituisce la difficoltà di una diagonale
unsigned int calcolaDifficoltaDiagonale(diagonale d) {
    d->Difficolta = 0;
    for (unsigned int i = 0; i < d->Elementi->ObjectsNumber; i++) {
        elemento tmp = (elemento)d->Elementi->Objects[i];
        d->Difficolta += tmp->Difficolta;
    }
    return d->Difficolta;
}

// Calcola e restituisce il punteggio di una diagonale
float calcolaPunteggioDiagonale(diagonale d) {
    d->Punteggio = 0;
    for (unsigned int i = 0; i < d->Elementi->ObjectsNumber; i++) {
        elemento tmp = (elemento)d->Elementi->Objects[i];
        d->Punteggio += tmp->Valore;
    }
    return d->Punteggio;
}

// Copia SRC in DEST
void copiaDiagonale(diagonale DEST, diagonale SRC) {
    DEST->Difficolta = SRC->Difficolta;
    DEST->Punteggio  = SRC->Punteggio;
    copyArray(DEST->Elementi, SRC->Elementi);
}

/* Crea e alloca una diagonale
   Se NumeroElementi è pari a 0 il numero di elementi sarà il valore di default */
diagonale creaDiagonale(unsigned int NumeroElementi) {
    NumeroElementi = NumeroElementi == 0 ? MAX_ELEMENTI : NumeroElementi;
    diagonale d    = (diagonale)malloc(sizeof(struct Diagonale));
    d->Difficolta  = 0;
    d->Punteggio   = 0;
    d->Elementi    = newArray((void *)&freeElemento, (void *)&parseElemento, (void *)&printElemento);
    allocateArray(d->Elementi, NumeroElementi);
    return d;
}

// Elimina una diagonale
void freeDiagonale(diagonale d) {
    freeArray(d->Elementi, false);
    free(d);
}

// Stampa una diagonale a video
void printDiagonale(diagonale d) {
    printf("Il punteggi della diagonale è %f e la sua difficoltà %u:\n", d->Punteggio, d->Difficolta);
    printArray(d->Elementi);
}

// Ricalcola i controlli
bool updateChecks(elemento e, checks c, unsigned int difficoltaMassima, unsigned int difficoltaMinima) {
    bool valido = false;
    if (c->Richiesti[elementoAvanti]) { // Se il controllo è richiesto
        if (e->Ingresso == avanti) {
            c->Valori[elementoAvanti]    = true;
            c->Richiesti[elementoAvanti] = false; // Rimuovo la richiesta
        }
    }
    if (c->Richiesti[elementoIndietro]) { // Se il controllo è richiesto
        if (e->Ingresso == indietro) {
            c->Valori[elementoIndietro]    = true;
            c->Richiesti[elementoIndietro] = false; // Rimuovo la richiesta
        }
    }
    if (c->Richiesti[dueElementi]) {                                 // Se il controllo è richiesto
        if (difficoltaMinima <= difficoltaMassima - e->Difficolta) { // Se rientro nel limite dell'elemento di difficoltà minima
            c->Valori[dueElementi]    = true;
            c->Richiesti[dueElementi] = false; // Rimuovo la richiesta
        }
    }
    return verificaChecks(c);
}

// Ricalcola i controlli per tutti gli elementi di una diagonale
void updateChecksDiagonale(diagonale d, checks c) {
    c->Valori[elementoAvanti]   = false;
    c->Valori[elementoIndietro] = false;
    for (unsigned int i = 0; i < d->Elementi->ObjectsNumber; i++) { // Per ogni elemento della diagonale
        elemento tmp = d->Elementi->Objects[i];
        updateChecks(tmp, c);
        if (c->Valori[elementoAvanti] && c->Valori[elementoIndietro]) { // Se sono già entrambi validi mi interrompo
            break;
        }
    }
    c->Valori[dueElementi] = d->Elementi->ObjectsNumber > 1;
}

bool insertCheck(elemento e, unsigned int difficoltaDiagonale, checks controlli, unsigned int elementiInseribili) {
    if (e->Difficolta > difficoltaDiagonale) { // Interruzione per limite difficoltà
        return false;
    }
    if (e->Finale && elementiInseribili != 1) { // Interruzione per elemento finale non in ultima posizione
        return false;
    }
    if (!updateChecks(e, controlli)) { // Interruzione basata sui controlli
        return false;
    }
    return true;
}

// Genera tutte le diagonali possibili rispettando il limite di difficoltà e l'ordine di inserimento
uint8_t generaDiagonaleR(array elementi, unsigned int difficoltaDiagonale, checks controlli, link soluzione, uint8_t elementiInseribili) {
    uint8_t elementiInseriti = 0;
    if (elementiInseribili == 0) { // Interruzione per limite di inserimenti
        return 0;
    }
    if (difficoltaDiagonale == 0) { // Interruzione per limite di difficoltà
        return 0;
    }
    for (unsigned int i = 0; i < elementi->ObjectsNumber; i++) { // Esploro tutti gli elementi
        elemento tmp = elementi->Objects[i];
        if (insertCheck(tmp, difficoltaDiagonale, controlli, elementiInseribili)) { // Se l'elemento è inseribile
            putItem(soluzione, tmp);
            elementiInseribili--; // Riduco il numero di elementi inseribili
            elementiInseriti++;
            // Ricorro per l'elemento successivo
            return elementiInseriti += generaDiagonaleR(elementi, (difficoltaDiagonale - tmp->Difficolta), controlli, soluzione, elementiInseribili);
        }
    }
}

// Genera la miglior diagonale in base ai limiti e ai controlli
diagonale generaDiagonale(array elementi, unsigned int DD, unsigned int DP, checks controlli) {
    link maxDiagonale        = creaLink(NULL);
    uint8_t elementiInseriti = generaDiagonaleR(elementi, DD, controlli, maxDiagonale, MAX_ELEMENTI);

    diagonale tempDiag = creaDiagonale(elementiInseriti); // Creo la diagonale

    uint8_t index = 0;
    while (maxDiagonale->Next != NULL && index < tempDiag->Elementi->ObjectsNumber) { // Sinché ho elementi in lista
        maxDiagonale                         = maxDiagonale->Next;
        tempDiag->Elementi->Objects[index++] = maxDiagonale->Item; // Inserisco l'elemento nella diagonale
    }

    // Ricalcolo i valori
    calcolaDifficoltaDiagonale(tempDiag);
    calcolaPunteggioDiagonale(tempDiag);

    freeList(maxDiagonale); // Elimino la lista

    return tempDiag;
}

// Restituisce il valore massimo ottenibile come sequenza consecutiva dello stesso elemento senza superare una data diffioltà
float maxValoreConDifficolta(elemento e, unsigned int difficolta, bool moltiplicatore) {
    uint8_t nElementi              = 0;
    float valore                   = 0;
    unsigned int difficoltaAttuale = 0;
    while (nElementi < MAX_ELEMENTI && (difficoltaAttuale + e->Difficolta) <= difficolta) { // Sinché rientro nei limiti
        valore += e->Valore;
        difficoltaAttuale += e->Difficolta;
        nElementi++;
        if (e->Finale) { // Se l'elemento è di tipo finale mi interrompo ad un solo inserimento
            break;
        }
    }
    valore *= difficoltaAttuale && moltiplicatore >= 8 ? 1.5 : 1; // Se il moltiplicatore è attivo incremento il punteggio
    return valore;
}

// Restituisce true se l'elemento a è più ottimale rispetto a b
bool maggiorValore(elemento a, elemento b, unsigned int *difficoltaDiagonale) {
    return maxValoreConDifficolta(a, *difficoltaDiagonale, false) > maxValoreConDifficolta(b, *difficoltaDiagonale, false);
}

// Restituisce true se l'elemento a è più ottimale rispetto a b
bool maggiorValoreConMoltiplicatore(elemento a, elemento b, unsigned int *difficoltaDiagonale) {
    return maxValoreConDifficolta(a, *difficoltaDiagonale, true) > maxValoreConDifficolta(b, *difficoltaDiagonale, true);
}