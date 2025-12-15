//
// Created by hoerz on 15.12.2025.
//

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deck.h"

// ------------------------------------------------------------
// Interne Hilfen
// ------------------------------------------------------------

static void ensure_rng_seeded(void) {
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        srand((unsigned)time(NULL));
    }
}

static void swap_cards(Card* a, Card* b) {
    Card tmp = *a;
    *a = *b;
    *b = tmp;
}

// ------------------------------------------------------------
// Öffentliche API
// ------------------------------------------------------------

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

    // Füllreihenfolge fix: ♠, ♣, ♥, ♦; Ränge 2..A
    const Suit suits[4] = {SUIT_SPADES, SUIT_CLUBS, SUIT_HEARTS, SUIT_DIAMONDS};

    int idx = 0;
    for (int si = 0; si < 4; ++si) {
        for (int r = (int)RANK_2; r <= (int)RANK_A; ++r) {
            d->data[idx].suit = suits[si];
            d->data[idx].rank = (Rank)r;
            ++idx;
        }
    }

    return d;
}

void deck_free(Deck* d) {
    if (!d) return;
    free(d->data);
    free(d);
}

void deck_shuffle(Deck* d) {
    if (!d || d->size <= 1) return;

    ensure_rng_seeded();

    // Fisher-Yates: i von n-1 herunter, j zufällig in [0..i]
    for (int i = d->size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap_cards(&d->data[i], &d->data[j]);
    }
}

int deck_is_empty(const Deck* d) {
    return (!d || d->size == 0);
}

int deck_draw_top(Deck* d, Card* out) {
    if (!d || !out) return -1;
    if (d->size == 0) return -1;

    // Top ist das letzte Element (LIFO)
    *out = d->data[d->size - 1];
    d->size -= 1;
    return 0;
}

