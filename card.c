//
// Created by hoerz on 15.12.2025.
//

#include "card.h"

/**
 * @brief Vergleicht beide Karten gemäß den Spielregeln und kürt eine dementsprechend zum Gewinner
 * @param a Angreiferkarte
 * @param b Verteidigerkarte
 * @return
 * - 0: Verteidiger gewinnt
 * - 1: Angreifer gewinnt
 */
int card_clash(const Card* a, const Card* b) {
    const int a_worth = a->rank;
    const int b_worth = b->rank;

    if (b_worth > a_worth)
        return 0;
    return 1;
}
