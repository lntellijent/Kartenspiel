//
// Created by hoerz on 15.12.2025.
//

#include <stdlib.h>
#include <time.h>

#include "../header/deck.h"


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
static status deck_ensure_capacity(Deck *source_deck, const size_t min_capacity) {
    // ReSharper disable once CppDFAConstantConditions
    if (!source_deck) return NULL_POINT_ERROR; // Initialisierungsfehler
    if (min_capacity <= 0) return INVALID_ARGUMENT;
    if (min_capacity <= source_deck->capacity) return OK;

    // Wachstumsstrategie: entweder verdoppeln, oder auf min_capacity gehen
    size_t new_capacity = source_deck->capacity > 0 ? source_deck->capacity * 2 : 1;
    if (new_capacity < min_capacity) new_capacity = min_capacity;

    Card *new_card_array = realloc(source_deck->cards, new_capacity * sizeof(Card));
    if (!new_card_array) return CRITICAL_ERROR; // realloc fehlgeschlagen, alte Daten bleiben intakt

    source_deck->cards = new_card_array;
    source_deck->capacity = new_capacity;
    return OK;
}

Deck *create_standard_deck(void) {
    // 52 Karten: (2..A) × (♠, ♣, ♥, ♦)
    Deck *deck = malloc(sizeof(Deck));
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
    for (size_t suit_index = 0; suit_index < 4; ++suit_index) {
        for (size_t rank_index = RANK_2; rank_index <= RANK_A; ++rank_index) {
            deck->cards[card_count_index].suit = (Suit) suit_index;
            deck->cards[card_count_index].rank = (Rank) rank_index;
            ++card_count_index;
        }
    }

    return deck;
}

Deck *create_empty_deck(const size_t initial_capacity) {
    Deck *d = malloc(sizeof *d);
    if (!d) return NULL;

    d->cards = (Card *) malloc(initial_capacity * sizeof(Card));
    if (!d->cards) {
        free(d);
        return NULL;
    }
    d->card_count = 0;
    d->capacity = initial_capacity;

    return d;
}

status shuffle(const Deck *deck) {
    if (!deck || deck->card_count <= 1) return NULL_POINT_ERROR;

    ensure_rng_seeded();

    // Fisher-Yates: i von n-1 herunter, j zufällig in [0..i]
    for (size_t i = deck->card_count - 1; i > 0; --i) {
        const size_t j = rand() % (i + 1);
        swap_cards(&deck->cards[i], &deck->cards[j]);
    }
    return OK;
}

boolean is_empty(const Deck *deck) {
    return (!deck || deck->card_count == 0) == 0 ? FALSE : TRUE;
}

status deck_draw_top(Deck *deck, Card *output_card) {
    if (!deck || is_empty(deck)) return NULL_POINT_ERROR;
    // Top ist das letzte Element (LIFO)
    *output_card = deck->cards[--deck->card_count];
    return OK;
}

status deck_draw_index(Deck *deck, Card *output_card, const size_t card_index) {
    if (!deck || is_empty(deck)) return NULL_POINT_ERROR;
    if (card_index >= deck->card_count) return NULL_POINT_ERROR;

    // Index herausnehmen
    *output_card = deck->cards[card_index];
    // Daten aufrucken
    for (size_t i = card_index; i < deck->card_count - 1; i++)
        deck->cards[i] = deck->cards[i + 1];
    deck->card_count--;
    return OK;
}

status insert(Deck *deck, Card *output_card) {
    if (!deck || !output_card || deck->card_count < 0) return NULL_POINT_ERROR; // Initialisierungsfehler

    // Kapazität sicherstellen
    status error;
    if ((error = deck_ensure_capacity(deck, deck->card_count + 1)) != OK) return error;
    // Wachstum fehlgeschlagen

    deck->cards[deck->card_count++] = *output_card;
    return OK;
}

size_t consume_and_count_worth(Deck *deck) {
    if (!deck || deck->card_count <= 0) return -1;

    Card current_card;
    size_t worth = 0;
    while (!is_empty(deck) && deck_draw_top(deck, &current_card) == OK) {
        if (current_card.rank >= 2 && current_card.rank <= 10)
            worth += current_card.rank; // Zahlen zählen gemäß ihren Augenzahlen
        else if (current_card.rank > 10 && current_card.rank < 14) {
            worth += current_card.rank - 9;
            /* Offset = 9
             * Bube → 11-9=2 Punkte in der Wertung (nicht vom Stichwert her)
             * Dame → 12-9=3 Punkte
             * König → 13-9=4 Punkte
             */
        } else if (current_card.rank == 14)
            worth += 11; // Ass-Punktewert = 11
    }
    return worth;
}

status card_deal(Deck *source_deck, Deck *destination_deck, const size_t card_count) {
    size_t player_index = 0;
    status error;
    Card card_holder;

    while (player_index++ < card_count) {
        if ((error = deck_draw_top(source_deck, &card_holder)) != OK) return error;
        if ((error = insert(destination_deck, &card_holder)) != OK) return error;
    }
    return OK;
}

status deal_lowest_card(Deck *deck, Card *lowest_card) {
    if (!deck || !lowest_card) return NULL_POINT_ERROR;
    status error;
    size_t lowest_card_index = 0;
    for (size_t i = 0; i < deck->card_count; i++) {
        if (deck->cards[lowest_card_index].rank > deck->cards[i].rank)
            lowest_card_index = i;
    }
    if ((error = deck_draw_index(deck, lowest_card, lowest_card_index) != OK)) return error;
    return OK;
}

status deal_highest_card(Deck *deck, Card *highest_card) {
    if (!deck || !highest_card) return NULL_POINT_ERROR;
    status error;
    size_t highest_card_index = 0;
    for (size_t card_index = 0; card_index < deck->card_count; card_index++) {
        if (deck->cards[highest_card_index].rank < deck->cards[card_index].rank)
            highest_card_index = card_index;
    }
    if ((error = deck_draw_index(deck, highest_card, highest_card_index) != OK)) return error;
    return OK;
}

status deal_card_by_index(Deck *deck, Card *card, const size_t index) {
    if (!deck || !card) return NULL_POINT_ERROR;
    status error;
    if ((error = deck_draw_index(deck, card, index) != OK)) return error;
    return OK;
}
