//
// Created by hoerz on 18.12.2025.
//

#ifndef KARTENSPIEL_PLAYER_H
#define KARTENSPIEL_PLAYER_H

#include "deck.h"

typedef status (*card_strategy_chooser)(Deck *deck, Card *card, Card *card_to_beat, boolean isAttacker);

// Vollständiges Objekt eines Spielers
typedef struct {
    Deck *hand; // Spielkarten
    Deck *points; // Punktekarten
    card_strategy_chooser strategy; // Strategie
    wchar_t name[32]; // Spielername
} player;

/**
 * @brief Ermittelt Spielernamen für ein Playerobjekt
 * @param player das entsprechende Spielerobjekt
 * @return statuscode:
 * - OK: Fehlerfrei
 */
status player_name(player *player);

/**
 * @brief Gibt den Speicher eines Spielers frei
 * @param player Das Playerobjekt, dessen Speicher freigegeben werden soll
 */
void player_free(const player *player);

#endif
