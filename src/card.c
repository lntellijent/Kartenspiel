//
// Created by hoerz on 15.12.2025.
//

#include "../header/card.h"

status print_card(const Card card, wchar_t *s) {
    s[0] = rank[card.rank][0];
    s[1] = suit[card.suit];
    s[2] = L'\0';
    return OK;
}
