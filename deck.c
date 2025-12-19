//
// Created by hoerz on 15.12.2025.
//

#include <stdlib.h>
#include <time.h>
#include "deck.h"

#include <stdio.h>

/**
 * @brief Setzt für jeden Durchgang einen neuen Seed für den Zufallsgenerator, welcher die Karten mischt.
 * @note Der Seed ist dabei der Timestamp des Erstellungszeitpunkts
 */
static void ensure_rng_seeded(void) {
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        srand((unsigned)time(NULL));
    }
}

/**
 * @brief Tauscht zwei Karten; Wird im shufflealgorithmus verwendet
 * @param a Karte a, später Karte b
 * @param b Karte b, später Karte A
 */
static void swap_cards(Card* a, Card* b) {
    Card tmp = *a;
    *a = *b;
    *b = tmp;
}


/**
 * @brief erzeugt einen Kartenstapel mit dem weitergearbeitet werden kann
 * @return Ein sortiertes Standard-Deck
 */
Deck* deck_create_standard(void) {
    // 52 Karten: (2..A) × (♠, ♣, ♥, ♦)
    Deck* d = (Deck*)malloc(sizeof(Deck));
    if (!d) return NULL;

    d->capacity = 52;
    d->size = 52;
    d->data = (Card*)malloc((size_t)d->capacity * sizeof(Card));
    if (!d->data) {
        free(d);
        return NULL;
    }

    // Füllreihenfolge fix: Pik, Kreuz, Herz, Karo; Ränge 2..A
    const Suit suits[4] = {SUIT_SPADES, SUIT_CLUBS, SUIT_HEARTS, SUIT_DIAMONDS};

    int idx = 0;
    for (int si = 0; si < 4; ++si) {
        for (int r = (int)RANK_2; r <= (int)RANK_A; ++r) {
            d->data[idx].suit = (Suit) si;
            d->data[idx].rank = (Rank) r;
            ++idx;
        }
    }

    return d;
}

/**
 * @brief Erstellt ein gültiges, aber leeres Deck
 * @param initial_capacity Die Anzahl der freien Plätze, ohne, dass es erweitert werden muss
 * @return das leere Deck
 */
Deck* deck_create_empty(int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 0; // Standard-Kapazität falls ungültiger Wert eingegeben wird

    Deck* d = (Deck*)malloc(sizeof *d);
    if (!d) return NULL;

    d->data = (Card*)malloc((size_t)initial_capacity * sizeof(Card));
    if (!d->data) {
        free(d);
        return NULL;
    }
    d->size = 0;
    d->capacity = initial_capacity;

    return d;
}
/**
 * @brief Gibt den reservierten Speicherplatz des Decks frei
 * @param d Das freizugebende Deck
 */
void deck_free(Deck* d) {
    if (!d) return;
    free(d->data);
    free(d);
}


/**
 * @brief mischt das übergebene Deck
 * @param d das zu mischende Deck
 */
void deck_shuffle(const Deck* d) {
    if (!d || d->size <= 1) return;

    ensure_rng_seeded();

    // Fisher-Yates: i von n-1 herunter, j zufällig in [0..i]
    for (int i = d->size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap_cards(&d->data[i], &d->data[j]);
    }
}

/**
 * @brief Prüft, ob das übergebene Deck leer ist oder noch Elemente enthält
 * @param d das zu überprüfende Deck
 * @return 1, falls das Deck entweder leer oder null ist, sonst 0
 */
int deck_is_empty(const Deck* d) {
    return (!d || d->size == 0);
}

/**
 * @brief Zieht (virtuell) die oberste Karte (LIFO-Prinzip). Nur zufällig falls davor gemischt wurde.
 * @param d Das Deck aus welchem gezogen werden soll
 * @param out Die Karte die gezogen wird
 * @return 0, falls erfolgreich, -1, falls das Deck leer ist
 */
int deck_draw_top(Deck* d, Card* out) {
    if (!d || deck_is_empty(d)) return -1;

    // Top ist das letzte Element (LIFO)
    *out = d->data[--d->size];
    return 0;
}


/**
 * @brief Stellt sicher, dass das Deck mindestens die angegebene Kapazität besitzt.
 * @param d Zeiger auf das zu erweiternde Deck. Darf nicht NULL sein.
 * @param min_capacity Mindestens benötigte Kapazität (Anzahl Karten), die nach
 * Rückkehr verfügbar sein soll. Muss >= 0 sein.
 * @return Fehler-/Statuscode:
 * - `0`  bei Erfolg (Kapazität war ausreichend oder wurde erfolgreich erhöht)
 * - `-1` wenn `d == NULL` oder gewünschte Kapazität negativ
 * - `-2` wenn Speichervergrößerung via `realloc` fehlgeschlagen ist
 */
static int deck_ensure_capacity(Deck* d, int min_capacity) {
    if (!d || min_capacity <= 0) return -1;
    if (min_capacity <= d->capacity) return 0;

    // Wachstumsstrategie: entweder verdoppeln, oder auf min_capacity gehen
    int new_capacity = d->capacity > 0 ? d->capacity * 2 : 1;
    if (new_capacity < min_capacity) new_capacity = min_capacity;

    Card* new_data = (Card*)realloc(d->data, (size_t)new_capacity * sizeof(Card));
    if (!new_data) return -2; // realloc fehlgeschlagen, alte Daten bleiben intakt

    d->data = new_data;
    d->capacity = new_capacity;
    return 0;
}


/**
 * @brief fügt eine Karte dem Deck hinzu
 * @param d Das Deck, welchem die Karte hinzugefügt werden soll
 * @param c Die Karte die eingefügt wird
 * @return Fehler-/Statuscodes:
 * - 0: keine Fehler
 * - -1: Deck oder Karte sind nicht initialisiert
 * - -2: inkonsistente Werte
 * - -3: Wachstum fehlgeschlagen
 */
int deck_insert(Deck* d, const Card* c) {

    if (!d || !c) return -1;
    if (d->size < 0) return -2; // Schutz gegen inkonsistente Werte

    // Kapazität sicherstellen
    if (deck_ensure_capacity(d, d->size + 1) != 0) {
        return -3; // Wachstum fehlgeschlagen
    }

    d->data[d->size++] = *c;
    return 0;
}

/**
 * @brief Zählt den Punktewert des Decks
 * @param d Das zu zählende Deck
 * @return Fehler-/Statuscode:
 * - Wert > -1: Fehlerfrei, enthält den entsprechenden Deck-Wert
 * - -1: Fehler
 * @warning Leert das Deck vollständig!
 */
unsigned int deck_count_worth(Deck* d) {
    if (!d || d->size <= 0) return -1;
    Card c;
    unsigned int worth = 0;
    while (deck_draw_top(d,&c) == 0) {
        if (c.rank >= 2 && c.rank <= 10) {
            worth += c.rank;
            // Zahlen zählen gemäß ihren Augenzahlen
        } else if (c.rank > 10 && c.rank < 14) {
            worth += c.rank-9;
            /* Offset = 9
             * Bube => 11-9=2 Punkte in der Wertung (nicht vom Stichwert her)
             * Dame => 12-9=3 Punkte
             * König => 13-9=4 Punkte
             */
        } else if (c.rank == 14) {
            worth += 11;
            // Ass-Punktewert = 11
        }
    }
    return worth;
}

int card_deal(Deck* stack, Deck* d, const int count) {
    int i = 0;
    Card c;
    while (deck_draw_top(stack, &c) == 0 && i++ < count) {
        //printf("input Karte: %s/%s\n", card_suit_str(c->suit), card_rank_str(c->rank));
        deck_insert(d, &c);
    }
    return 0;
}