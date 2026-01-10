//
// Created by hoerz on 15.12.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "deck.h"


#define STANDARD_DECK_SIZE 52

/**
 * @brief Setzt für jeden Durchgang einen neuen Seed für den Zufallsgenerator, welcher die Karten mischt.
 * @note Der Seed ist dabei der Timestamp des Erstellungszeitpunkts
 */
static void ensure_rng_seeded(void) {
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        srand((unsigned) time(NULL));
    }
}

/**
 * @brief Gibt den reservierten Speicherplatz des Decks frei
 * @param deck Das freizugebende Deck
 */
void deck_free(Deck *deck) {
    if (!deck) return;
    free(deck->cards);
    free(deck);
}

/**
 * @brief Tauscht zwei Karten; wird im Mischalgorithmus verwendet
 * @param a Karte a, später Karte b
 * @param b Karte b, später Karte a
 */
static void swap_cards(Card *a, Card *b) {
    const Card tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * @brief Stellt sicher, dass das Deck mindestens die angegebene Kapazität besitzt.
 * @param source_deck Zeiger auf das zu erweiternde Deck. Darf nicht NULL sein.
 * @param min_capacity Mindestens benötigte Kapazität (Anzahl Karten), die nach
 * Rückkehr verfügbar sein soll. Muss >= 0 sein.
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
static status deck_ensure_capacity(Deck *source_deck, const int min_capacity) {
    // ReSharper disable once CppDFAConstantConditions
    if (!source_deck) return NULL_POINT_ERROR; // Initialisierungsfehler
    if (min_capacity <= 0) return INVALID_ARGUMENT;
    if (min_capacity <= source_deck->capacity) return OK;

    // Wachstumsstrategie: entweder verdoppeln, oder auf min_capacity gehen
    int new_capacity = source_deck->capacity > 0 ? source_deck->capacity * 2 : 1;
    if (new_capacity < min_capacity) new_capacity = min_capacity;

    Card *new_card_array = realloc(source_deck->cards, (size_t) new_capacity * sizeof(Card));
    if (!new_card_array) return CRITICAL_ERROR; // realloc fehlgeschlagen, alte Daten bleiben intakt

    source_deck->cards = new_card_array;
    source_deck->capacity = new_capacity;
    return OK;
}

/**
 * @brief erzeugt einen Kartenstapel mit dem weitergearbeitet werden kann
 * @return Ein sortiertes Standard-Deck, falls ein Fehler auftritt: NULL
 */
Deck *create_standard_deck(void) {
    // 52 Karten: (2..A) × (♠, ♣, ♥, ♦)
    Deck *deck = (Deck *) malloc(sizeof(Deck));
    if (!deck) return NULL;

    deck->capacity = STANDARD_DECK_SIZE;
    deck->card_count = STANDARD_DECK_SIZE;
    deck->cards = (Card *) malloc((size_t) deck->capacity * sizeof(Card));
    if (!deck->cards) {
        free(deck);
        return NULL;
    }

    // Füllreihenfolge fix: Pik, Kreuz, Herz, Karo; Ränge 2 ...A
    int card_count_index = 0;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = RANK_2; rank <= RANK_A; ++rank) {
            deck->cards[card_count_index].suit = (Suit) suit;
            deck->cards[card_count_index].rank = (Rank) rank;
            ++card_count_index;
        }
    }

    return deck;
}

/**
 * @brief Erstellt ein gültiges, aber leeres Deck
 * @param initial_capacity Die Anzahl der freien Plätze, ohne, dass es erweitert werden muss
 * @return das leere Deck, falls ein Fehler auftritt: NULL
 */
Deck *create_empty_deck(int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10; // Standard-Kapazität, falls ungültiger Wert eingegeben wird

    Deck *d = (Deck *) malloc(sizeof *d);
    if (!d) return NULL;

    d->cards = (Card *) malloc((size_t) initial_capacity * sizeof(Card));
    if (!d->cards) {
        free(d);
        return NULL;
    }
    d->card_count = 0;
    d->capacity = initial_capacity;

    return d;
}

/**
 * @brief mischt das übergebene Deck
 * @param d das zu mischende Deck
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: Deck nicht initialisiert
 */
status shuffle(const Deck *d) {
    if (!d || d->card_count <= 1) return NULL_POINT_ERROR;

    ensure_rng_seeded();

    // Fisher-Yates: i von n-1 herunter, j zufällig in [0..i]
    for (int i = d->card_count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap_cards(&d->cards[i], &d->cards[j]);
    }
    return OK;
}

/**
 * @brief Prüft, ob das übergebene Deck leer ist oder noch Elemente enthält
 * @param d das zu überprüfende Deck
 * @return TRUE, falls das Deck entweder leer oder null ist, sonst FALSE
 */
boolean is_empty(const Deck *d) {
    return (!d || d->card_count == 0) == 0 ? FALSE : TRUE;
}

/**
 * @brief Zieht (virtuell) die oberste Karte (LIFO-Prinzip). Nur zufällig, falls davor gemischt wurde.
 * @param d Das Deck aus welchem gezogen werden soll
 * @param out Die Karte die gezogen wird
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 */
status deck_draw_top(Deck *d, Card *out) {
    if (!d || is_empty(d)) return NULL_POINT_ERROR;
    // Top ist das letzte Element (LIFO)
    *out = d->cards[--d->card_count];
    return OK;
}

/**
 * @brief Zieht eine Karte aus einer bestimmten Stelle
 * @param source_deck Das Deck aus welchem gezogen werden soll
 * @param out Die Karte die gezogen wird
 * @param index Der Index an welchem die Karte abliegt
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 */
status deck_draw_index(Deck *source_deck, Card *out, const int index) {
    if (!source_deck || is_empty(source_deck)) return NULL_POINT_ERROR;
    if (index >= source_deck->card_count) return NULL_POINT_ERROR;

    // Index herausnehmen
    *out = source_deck->cards[index];
    // Daten aufrucken
    for (int i = index; i < source_deck->card_count - 1; i++)
        source_deck->cards[i] = source_deck->cards[i + 1];
    source_deck->card_count--;
    return OK;
}


/**
 * @brief fügt eine Karte dem Deck hinzu
 * @param source_deck Das Deck, welchem die Karte hinzugefügt werden soll
 * @param card_output Die Karte die eingefügt wird
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
status insert(Deck *source_deck, const Card *card_output) {
    if (!source_deck || !card_output || source_deck->card_count < 0) return NULL_POINT_ERROR; // Initialisierungsfehler

    // Kapazität sicherstellen
    status error;
    if ((error = deck_ensure_capacity(source_deck, source_deck->card_count + 1)) != OK) return error;
    // Wachstum fehlgeschlagen

    source_deck->cards[source_deck->card_count++] = *card_output;
    return OK;
}

/**
 * @brief Zählt den Punktewert des Decks
 * @param source_deck Das zu zählende Deck
 * @return Statuscode:
 * - Wert >= 0: Fehlerfrei, enthält den entsprechenden Deck-Wert
 * - -1: NULL_POINT_ERROR
 * @warning Leert das Deck vollständig!
 */
int consume_and_count_worth(Deck *source_deck) {
    if (!source_deck || source_deck->card_count <= 0) return -1;

    Card c;
    unsigned int worth = 0;
    while (!is_empty(source_deck) && deck_draw_top(source_deck, &c) == OK) {
        if (c.rank >= 2 && c.rank <= 10)
            worth += c.rank; // Zahlen zählen gemäß ihren Augenzahlen
        else if (c.rank > 10 && c.rank < 14) {
            worth += c.rank - 9;
            /* Offset = 9
             * Bube → 11-9=2 Punkte in der Wertung (nicht vom Stichwert her)
             * Dame → 12-9=3 Punkte
             * König → 13-9=4 Punkte
             */
        } else if (c.rank == 14)
            worth += 11; // Ass-Punktewert = 11
    }
    return (int) worth;
}

/**
 * @brief Teilt die Spielkarten entsprechend aus
 * @param main_deck Der "Ziehstapel"
 * @param destination_deck Der Spielerstapel; die Handkarten
 * @param card_count Die Anzahl der zu verwendenden Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
status card_deal(Deck *main_deck, Deck *destination_deck, const int card_count) {
    int player_index = 0;
    status error;
    Card card_holder;

    while (player_index++ < card_count) {
        if ((error = deck_draw_top(main_deck, &card_holder)) != OK) return error;
        if ((error = insert(destination_deck, &card_holder)) != OK) return error;
    }
    return OK;
}

/**
 *
 * @param source_deck Das Deck, welches angezeigt werden soll
 * @param print_indexes Anzeige der Indexnummern jeder Karte eine Zeile unter den Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Deck ist leer oder =NULL
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status print_deck(Deck *source_deck, const boolean print_indexes) {
    if (!source_deck || source_deck->card_count <= 0) return NULL_POINT_ERROR;
    if (printf("Deine Karten:\n%5s", "") < 0) return PRINT_ERROR;

    for (int i = 0; i < source_deck->card_count; i++)
        if (printf("%s%2s%s%s", i != 0 ? " " : "", ranks[source_deck->cards[i].rank], suits[source_deck->cards[i].suit],
                   i + 1 == source_deck->card_count ? ".\n" : "") < 0)
            return PRINT_ERROR;
    if (printf("%5s", "") < 0) return PRINT_ERROR;

    if (print_indexes) {
        for (int i = 0; i < source_deck->card_count; i++)
            if (printf("%1s[%d]", "", i) < 0) return PRINT_ERROR;
        if (printf("\n") < 0) return PRINT_ERROR;
    }
    return OK;
}
