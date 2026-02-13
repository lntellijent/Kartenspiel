//
// Created by hoerz on 11.02.2026.
//

#include "../header/strategy.h"
#include "../header/cli.h"

status card_play(const card_strategy_chooser csc, Deck *deck, Card *card, Card *card_to_beat,
                 const boolean isAttacker) {
    return csc(deck, card, card_to_beat, isAttacker);
}

status get_card(Deck *deck, Card *highest_card, Card *card_to_beat, boolean isAttacker) {
    return deck_draw_top(deck, highest_card);
}

status get_Highest_card(Deck *deck, Card *highest_card, Card *card_to_beat, boolean isAttacker) {
    return deal_highest_card(deck, highest_card);
}

status get_alternating_card(Deck *deck, Card *alternating_card, Card *card_to_beat, const boolean isAttacker) {
    if (!deck || !alternating_card) return NULL_POINT_ERROR;
    static boolean started_alternating_cycle = FALSE, lower_card = FALSE;
    if (started_alternating_cycle) {
        status error;
        if (lower_card) {
            if ((error = deal_lowest_card(deck, alternating_card)) != OK) return error;
            lower_card = FALSE;
        } else {
            if ((error = deal_highest_card(deck, alternating_card)) != OK) return error;
            lower_card = TRUE;
        }
    } else {
        started_alternating_cycle = TRUE;
        get_alternating_card(deck, alternating_card, card_to_beat, isAttacker);
    }
    return OK;
}

status get_intelligent_card(Deck *deck, Card *intelligent_card, const Card *card_to_beat, const boolean isAttacker) {
    if (!deck || !intelligent_card) return NULL_POINT_ERROR;
    if (isAttacker) {
        // Falls die KI angreift, legt sie stets die höchste ihr zur Verfügung stehenden Karte.
        status error;
        if ((error = deal_highest_card(deck, intelligent_card)) != OK) return error;
        return OK;
    }

    int intelligent_card_index = -1;
    for (size_t card_index = 0; card_index < deck->card_count; card_index++) {
        if (deck->cards[card_index].rank > card_to_beat->rank && // Karte muss höher als die gegnerische sein...
            deck->cards[intelligent_card_index].rank > deck->cards[card_index].rank)
            // ...aber niedriger als die bereits gefundene Karte, um so minimal gewinnen wie möglich
            intelligent_card_index = (int) card_index;
    }

    if (intelligent_card_index < 0) {
        // Falls die KI verliert, soll die niedrigste Karte gespielt werden, um so wenig wie möglich Punkte zu verlieren.
        deal_lowest_card(deck, intelligent_card);
    } else {
        // Falls die KI gewinnen kann, soll die niedrigste Karte gespielt werden, die zum Sieg führt.
        deal_card_by_index(deck, intelligent_card, intelligent_card_index);
    }
    return OK;
}

status get_humanoid_card(Deck *deck, Card *humanoid_card, Card *card_to_beat, const boolean isAttacker) {
    if (deck->card_count <= 0) return NULL_POINT_ERROR;
    status error;
    if ((error = print_deck(deck, isAttacker, TRUE)) != OK) return error;

    if (deck->card_count > 1) {
        size_t number_input = 0;
        boolean acceptable_input = FALSE; // oder: bool acceptable_input = false;

        while (!acceptable_input) {
            error = read_with(read_single_digit, &number_input);
            if (error == OK) {
                acceptable_input = TRUE;
            } else if (error == USER_INPUT_ERROR) {
                // genau EIN Aufruf pro eingegebener (und bestätigter) Zeile
                if ((error = invalid_user_response()) != OK) return error;
            } else {
                return error;
            }
        }

        if ((error = deck_draw_index(deck, humanoid_card, number_input)) != OK) return error;
    } else {
        if ((error = deck_draw_top(deck, humanoid_card)) != OK) return error;
    }
    return OK;
}
