//
// Created by hoerz on 11.02.2026.
//

#ifndef KARTENSPIEL_RULES_H
#define KARTENSPIEL_RULES_H

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
winner card_clash(Card attacker_card, Card defender_card);

#endif //KARTENSPIEL_RULES_H
