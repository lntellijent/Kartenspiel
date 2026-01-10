//
// Created by hoerz on 18.12.2025.
//

#ifndef KARTENSPIEL_PLAYER_H
#define KARTENSPIEL_PLAYER_H
#include "deck.h"

typedef struct {
    Deck *hand;
    Deck *points;
    int strategy;
} player;

status player_play_card(player player, Card *played_card);
#endif
