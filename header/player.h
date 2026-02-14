//
// Created by hoerz on 18.12.2025.
//

#ifndef KARTENSPIEL_PLAYER_H
#define KARTENSPIEL_PLAYER_H

#include "deck.h"

typedef status (*card_strategy_chooser)(Deck *deck, Card *card, Card *card_to_beat, boolean isAttacker);

typedef struct {
    Deck *hand;
    Deck *points;
    card_strategy_chooser strategy;
    wchar_t name[32];
} player;

/**
 * @brief Ermittelt Spielernamen für ein Playerobjekt
 * @param player das entsprechende Spielerobjekt
 * @return statuscode:
 * -
 */
status player_name(player *player);

void player_free(const player *player);

#endif
