#include "calcoloCombinatorio.h"

// * PRESETS

/* Preset per le disposizioni ripetute
check = funzione che si occupa della condizione di terminazione
ending = funzione che si occupa del return nella condizione di terminazione
next = funzione che si occupa di che fare con l'elemento successivo */
unsigned int _PRESET_disposizioniRipetute(array valori, array soluzione, unsigned int posizione, bool (*check)(array, unsigned int), unsigned int (*ending)(array, unsigned int), bool (*next)(array, array, unsigned int, unsigned int)) {
    unsigned int count = 0; // Contatore pre il numero di soluzioni

    if ((*check)(soluzione, posizione)) {       // Condizione di terminazione
        return (*ending)(soluzione, posizione); // Condizione di verifica per il return
    }

    for (unsigned int i = 0; i < valori->ObjectsNumber; i++) { // Per ogni valore
        // Funzione di esplorazione con pruning
        if ((*next)(valori, soluzione, posizione, i)) {                                                   // Esploro la posizione successiva solo se il pruning lo consente
            count += _PRESET_disposizioniRipetute(valori, soluzione, posizione + 1, check, ending, next); // Esplorazione sulla posizione successiva
        }
    }

    return count; // Restituisco il conteggio
}

// * CHECKS

// Controlla se la posizione ha raggiunto o superato la lunghezza massima dell'array soluzione
bool _CHECKS_posizioneLunghezzaSoluzione(array soluzione, unsigned int posizione) {
    return posizione >= soluzione->ObjectsNumber;
}

// * ENDINGS

// Stampa la soluzione e restituisce 1
unsigned int _ENDINGS_printOnly(array soluzione, unsigned int posizione) {
    if (soluzione->ObjectsNumber != posizione) {
        unsigned int tmp         = soluzione->ObjectsNumber;
        soluzione->ObjectsNumber = posizione;
        printArray(soluzione);
        soluzione->ObjectsNumber = tmp;
    }

    printArray(soluzione);
    return 1;
}

// * NEXTS

// Copia l'elemento successivo del ciclo nella soluzione
bool _NEXT_copyNext(array valori, array soluzione, unsigned int posizione, unsigned int i) {
    soluzione->Objects[posizione] = valori->Objects[i];
    return true;
}

// * Models

// Modello per le disposizioni ripetute con limite dato dalla lunghezza dell'array delle soluzioni e stampa dei dati
unsigned int diposizioniRipetuteDefault(array valori, array soluzione) {
    return _PRESET_disposizioniRipetute(valori, soluzione, 0,
                                        &_CHECKS_posizioneLunghezzaSoluzione,
                                        &_ENDINGS_printOnly,
                                        &_NEXT_copyNext);
}

/* Funzione per generare le combinazioni ripetute
   Il limite di elementi k è dato dalla lunghezza dell'array soluzione */
unsigned int combinazioniRipetute(array valori, array soluzione) {
    return combinazioniRipetuteRecursive(0, valori, soluzione, 0);
}

/* Funzione per generare le combinazioni ripetute
   Il limite di elementi k è dato dalla lunghezza dell'array soluzione
   Nella chiamata posizione e start devono esser pari a 0 */
unsigned int combinazioniRipetuteRecursive(unsigned int posizione, array valori, array soluzione, unsigned int start) {
    unsigned int count = 0;
    if (posizione >= soluzione->ObjectsNumber) {
        unsigned int tempSize    = soluzione->ObjectsNumber; // Salvo il valore della soluzione
        soluzione->ObjectsNumber = posizione;

        printArray(soluzione); // Stampo la soluzione

        soluzione->ObjectsNumber = tempSize; // Ripristino la dimensione precedente
        return 1;
    }
    for (unsigned i = start; i < valori->ObjectsNumber; i++) {
        soluzione->Objects[posizione] = valori->Objects[i];
        count += combinazioniRipetuteRecursive(posizione + 1, valori, soluzione, start);
        start++;
    }
    return count;
}