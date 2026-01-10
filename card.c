//
// Created by hoerz on 15.12.2025.
//

#include "card.h"

/**
 * @brief Vergleicht beide Karten gemäß den Spielregeln und kürt eine dementsprechend zum Gewinner
 * @param attacker_card Angreiferkarte
 * @param defender_card Verteidigerkarte
 * @return Status:
 * - DEFENDER_WINS: Verteidiger gewinnt
 * - ATTACKER_WINS: Angreifer gewinnt
 * - TIE: Gleichstand, Angreifer gewinnt
 */
winner card_clash(const Card *attacker_card, const Card *defender_card) {
    const int attacker_card_worth = attacker_card->rank;
    const int defender_card_worth = defender_card->rank;

    if (defender_card_worth > attacker_card_worth) return DEFENDER_WINS;
    if (defender_card_worth < attacker_card_worth) return ATTACKER_WINS;
    return TIE;
}
