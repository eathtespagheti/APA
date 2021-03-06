#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint8_t MAX_FILENAME = 51;

typedef enum { zaffiro,
               rubino,
               topazio,
               smeraldo,
               totale } pietra;

typedef struct Collana { // Struttura rappresentante una collana
    unsigned int Pietre[totale + 1];
    pietra *Array;
    unsigned int RipetizioniMassime;
    unsigned int ValorePietre[totale];
} collana;

unsigned int calcolaValore(collana *c) { // Calcola il valore di una collana
    unsigned int count = 0;
    for (size_t i = 0; i < totale; i++) { // Per ogni pietra
        count += c->Pietre[i] * c->ValorePietre[i];
    }
    return count;
}

bool verificaSmeraldiRubini(pietra *array) { // Verifica che l'ordine dei rubini o degli smeraldi sia rispettato
    if (array[1] == smeraldo || array[1] == topazio) {
        return true;
    }
    return false;
}

bool verificaZaffiriTopazi(pietra *array) { // Verifica che l'ordine dei topazi o degli zaffiri sia rispettato
    if (array[1] == zaffiro || array[1] == rubino) {
        return true;
    }
    return false;
}

bool verificaOrdine(pietra *array) { // Verifica che l'ordine della pietra successiva sia corretto
    switch (array[0]) {
        case zaffiro: {
            // Eseguo il controllo dell'ordine solo se non mi trovo sull'ultima pietra
            return verificaZaffiriTopazi(array);
        }

        case smeraldo: {
            // Eseguo il controllo dell'ordine solo se non mi trovo sull'ultima pietra
            return verificaSmeraldiRubini(array);
        }

        case rubino: {
            // Eseguo il controllo dell'ordine solo se non mi trovo sull'ultima pietra
            return verificaSmeraldiRubini(array);
        }

        case topazio: {
            // Eseguo il controllo dell'ordine solo se non mi trovo sull'ultima pietra
            return verificaZaffiriTopazi(array);
        }
    }
}

collana parseCollana(char *string) { // Effettua il parse di una collana da stringa e svuota la stringa
    collana c;
    c.Pietre[totale] = 0;

    for (unsigned int i = 0; i < totale; i++) {                  // Per ogni pietra
        sscanf(string, "%u %[^\n]", &c.Pietre[i], string); // Scrivo la disponibilità
        c.Pietre[totale] += c.Pietre[i];
    }

    for (unsigned int i = 0; i < totale; i++) {                        // Per ogni pietra
        sscanf(string, "%u %[^\n]", &c.ValorePietre[i], string); // Scrivo il valore
    }

    sscanf(string, "%u", &c.RipetizioniMassime);

    return c;
}

bool apriFile(char *filename, char *modalita, FILE **stream) { // Apre un file e controlla che l'operazione sia andata a buon fine
    *stream = fopen(filename, modalita);
    if (stream == NULL) { // Se l'apertura non è andata a buon fine
        puts("Errore apertura file");
        return false;
    }
    return true;
}

void printPietra(pietra p) { // Stampa una pietra
    switch (p) {
        case zaffiro: {
            printf("Z");
            break;
        }

        case smeraldo: {
            printf("S");
            break;
        }

        case rubino: {
            printf("R");
            break;
        }

        case topazio: {
            printf("T");
            break;
        }
    }
}

void contaPietre(collana *c) { // Conta il numero di pietre in una collana
    c->Pietre[zaffiro]  = 0;
    c->Pietre[smeraldo] = 0;
    c->Pietre[rubino]   = 0;
    c->Pietre[topazio]  = 0;
    for (size_t i = 0; i < c->Pietre[totale]; i++) { // Per ogni pietra
        switch (c->Array[i]) {
            case zaffiro: {
                c->Pietre[zaffiro]++;
                break;
            }

            case smeraldo: {
                c->Pietre[smeraldo]++;
                break;
            }

            case rubino: {
                c->Pietre[rubino]++;
                break;
            }

            case topazio: {
                c->Pietre[topazio]++;
                break;
            }
        }
    }
}

void printCollana(collana *c) { // Stampa una collana
    printf("%u Zaffiri di valore %u; ", c->Pietre[zaffiro], c->ValorePietre[zaffiro]);
    printf("%u Rubini di valore %u; ", c->Pietre[rubino], c->ValorePietre[rubino]);
    printf("%u Topazi di valore %u; ", c->Pietre[topazio], c->ValorePietre[topazio]);
    printf("%u Smerladi di valore %u;\n", c->Pietre[smeraldo], c->ValorePietre[smeraldo]);
    printf("Totale %u pietre con ripetizione massima di %u\n", c->Pietre[totale], c->RipetizioniMassime);
    printPietra(c->Array[0]);
    for (size_t i = 1; i < c->Pietre[totale]; i++) { // Per ogni pietra
        printf("-");
        printPietra(c->Array[i]);
    }
    printf("\n");
}

bool verificaRipetizioni(unsigned int pos, collana *c) { // Verifica che non si ecceda il numero massimo di ripetizioni
    pietra controllo = c->Array[pos - 1];
    for (size_t i = pos - 2; i >= (pos - 1) - c->RipetizioniMassime; i--) { // Per il numero di ripetizioni, contando all'indietro
        if (c->Array[i] != controllo) {                                     // Se ne ho anche una sola differente
            return true;
        }
    }
    return false;
}

bool controlloZaffiri(collana *c) { // Controlla che il numero di zaffiri non ecceda il numero di smeraldi
    collana temp        = *c;
    temp.Pietre[totale] = c->Pietre[totale];
    memcpy(temp.Array, c->Array, c->Pietre[totale]);
    contaPietre(&temp); // Riconto le pietre
    return temp.Pietre[zaffiro] <= temp.Pietre[smeraldo];
}

bool generaCollane(unsigned int pos, collana *c, collana *max) { // Restituisce true se è riuscito a trovare una combinazione, altrimenti false

    // Per collane con più di un elemento verifico l'ordine delle pietre
    if (pos > 1) {
        if (!verificaOrdine(&c->Array[pos - 2])) { // Verifico che l'ordine dell'ultima pietra inserita sia corretto
            return false;
        }
    }

    if (pos >= (c->RipetizioniMassime + 1)) { // Verifico le ripetizioni
        if (!verificaRipetizioni(pos, c)) {   // Se le ripetizioni non sono rispettate
            return false;
        }
    }

    // Condizione di terminazione, ovvero quando la posizione raggiunge il numero massimo di ripetizioni
    if (pos >= c->Pietre[totale]) {
        if (!controlloZaffiri(c)) { // Se il controllo sul numero di zaffiri non è rispettato
            return false;
        }
        if (calcolaValore(c) > calcolaValore(max)) { // Se il valore è maggiore
            max->Pietre[totale] = c->Pietre[totale];
            free(max->Array);
            max->Array = (pietra *)calloc(c->Pietre[totale], sizeof(pietra));
            memcpy(max->Array, c->Array, sizeof(pietra) * c->Pietre[totale]); // Copio l'array di pietre
        }
        return true;
    }

    for (pietra i = 0; i < totale; i++) { // Per ogni tipo di pietra
        if (max->Pietre[i] > 0) {         // Se ho ancora disponibilità
            max->Pietre[i]--;             // Tolgo la pietra
            c->Array[pos] = i;
            if (generaCollane(pos + 1, c, max)) { // Se trovo una collana successiva
                return true;
            }
            max->Pietre[i]++; // Rimetto la pietra al suo posto
        }
    }
    return false;
}

unsigned int collaneVarieLunghezze(collana *c, collana *max) {                 // Genera collane con lunghezza variabile
    memcpy(max->Pietre, c->Pietre, sizeof(unsigned int) * totale);             // Copio il numero massimo di pietre
    memcpy(max->ValorePietre, c->ValorePietre, sizeof(unsigned int) * totale); // Copio il valore delle pietre
    max->RipetizioniMassime       = c->RipetizioniMassime;
    max->Pietre[totale]           = 0;
    max->Array                    = (pietra *)malloc(sizeof(pietra));
    unsigned int lunghezzaMassima = c->Pietre[totale];
    c->Array                      = (pietra *)calloc(lunghezzaMassima, sizeof(pietra)); // Alloco la memoria nell'array

    for (size_t i = 1; i <= lunghezzaMassima; i++) {                   // Per un numero di volte pari al totale delle pietre
        memcpy(max->Pietre, c->Pietre, sizeof(unsigned int) * totale); // Copio il numero massimo di pietre
        c->Pietre[totale] = i;
        generaCollane(0, c, max);
    }

    contaPietre(max);
    free(c->Array);
    return calcolaValore(max);
}

bool checkFilestream(FILE *stream) { // Controlla errori di aperrtura del file
    if (stream == NULL) {
        puts("Errore apertura file");
    }
    return false;
}

void parseFromFile(char *filename) { // Esegue i vari test presenti in un file
    FILE *stream = fopen(filename, "r");
    checkFilestream(stream);
    unsigned int nTest, massima;
    collana max;
    fscanf(stream, "%u\n", &nTest);
    for (unsigned int i = 0; i < nTest; i++) { // Per ogni test
        char riga[MAX_FILENAME];
        fgets(riga, MAX_FILENAME - 1, stream);
        collana c = parseCollana(riga);
        printf("TEST #%u\n", i + 1);
        massima = collaneVarieLunghezze(&c, &max);
        printf("Valore collana %u; ", massima);
        printCollana(&max);
    }
}

int main() {
    char filename[MAX_FILENAME];
    puts("Inserisci il nome del file:");
    printf("==> ");
    fgets(filename, (MAX_FILENAME - 1), stdin);
    sscanf(filename, "%s", filename);
    parseFromFile(filename);
    return 0;
}
