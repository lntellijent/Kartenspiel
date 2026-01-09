//
// Created by hoerz on 15.12.2025.
//

#include "card.h"

/**
 * @brief Vergleicht beide Karten gemäß den Spielregeln und kürt eine dementsprechend zum Gewinner
 * @param a Angreiferkarte
 * @param b Verteidigerkarte
 * @return Status:
 * - DEFENDER_WINS: Verteidiger gewinnt
 * - ATTACKER_WINS: Angreifer gewinnt
 * - TIE: Gleichstand, Angreifer gewinnt
 */
winner card_clash(const Card* a, const Card* b) {
    const int a_worth = a->rank;
    const int b_worth = b->rank;

    if (b_worth > a_worth) return DEFENDER_WINS;
    if (b_worth < a_worth) return ATTACKER_WINS;
    return TIE;
}
