//
// Created by hoerz on 15.12.2025.
//

#include "card.h"

winner card_clash(const Card *attacker_card, const Card *defender_card) {
    const int attacker_card_worth = attacker_card->rank;
    const int defender_card_worth = defender_card->rank;

    if (defender_card_worth > attacker_card_worth) return DEFENDER_WINS;
    if (defender_card_worth < attacker_card_worth) return ATTACKER_WINS;
    return TIE;
}
