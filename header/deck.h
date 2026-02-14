//
// Created by hoerz on 15.12.2025.
//

#ifndef DECK_H
#define DECK_H

#include "card.h"
#include "main.h"

typedef struct {
    Card *cards;
    size_t card_count;
    size_t capacity;
} Deck;

void deck_free(Deck *deck);

/**
 * @brief erzeugt einen Kartenstapel mit dem weitergearbeitet werden kann
 * @return Ein sortiertes Standard-Deck, falls ein Fehler auftritt: NULL
 */
Deck *create_standard_deck(void);

/**
 * @brief Erstellt ein gültiges, aber leeres Deck
 * @param initial_capacity Die Anzahl der freien Plätze, ohne, dass es erweitert werden muss
 * @return das leere Deck, falls ein Fehler auftritt: NULL
 */
Deck *create_empty_deck(size_t initial_capacity);

/**
 * @brief mischt das übergebene Deck
 * @param deck das zu mischende Deck
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: Deck nicht initialisiert
 */
status shuffle(const Deck *deck);

/**
 * @brief Prüft, ob das übergebene Deck leer ist oder noch Elemente enthält
 * @param deck das zu überprüfende Deck
 * @return TRUE, falls das Deck entweder leer oder null ist, sonst FALSE
 */
boolean is_empty(const Deck *deck);

/**
 * @brief Zieht (virtuell) die oberste Karte (LIFO-Prinzip). Nur zufällig, falls davor gemischt wurde.
 * @param deck Das Deck aus welchem gezogen werden soll
 * @param output_card Die Karte die gezogen wird
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 */
status deck_draw_top(Deck *deck, Card *output_card);

/**
 * @brief Zieht eine Karte aus einer bestimmten Stelle
 * @param deck Das Deck aus welchem gezogen werden soll
 * @param output_card Die Karte die gezogen wird
 * @param card_index Der Index an welchem die Karte abliegt
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 */
status deck_draw_index(Deck *deck, Card *output_card, size_t card_index);

/**
 * @brief fügt eine Karte dem Deck hinzu
 * @param deck Das Deck, welchem die Karte hinzugefügt werden soll
 * @param output_card Die Karte die eingefügt wird
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
status insert(Deck *deck, Card *output_card);

/**
 * @brief Zählt den Punktewert des Decks
 * @param deck Das zu zählende Deck
 * @return Statuscode:
 * - Wert >= 0: Fehlerfrei, enthält den entsprechenden Deck-Wert
 * - -1: NULL_POINT_ERROR
 * @warning Leert das Deck vollständig!
 */
size_t consume_and_count_worth(Deck *deck);

/**
 * @brief Teilt die Spielkarten entsprechend aus
 * @param source_deck Der "Ziehstapel"
 * @param destination_deck Der Spielerstapel; die Handkarten
 * @param card_count Die Anzahl der zu verwendenden Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
status card_deal(Deck *source_deck, Deck *destination_deck, size_t card_count);

/**
 * @brief Zieht eine bestimmte Karte aus dem Deck. Hier: Die niedrigste
 * @param deck Das Deck aus welchem die Karte genommen wird
 * @param lowest_card die Karte selbst; hier die niedrigste Karte des Decks
 * @return statuscodes:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Karte oder Deck ist nicht initialisiert/ist NULL
 */
status deal_lowest_card(Deck *deck, Card *lowest_card);

/**
 * @brief Zieht eine bestimmte Karte aus dem Deck. Hier: Die niedrigste
 * @param deck Das Deck aus welchem die Karte genommen wird
 * @param highest_card die Karte selbst; hier die höchste Karte des Decks
 * @return statuscodes:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Karte oder Deck ist nicht initialisiert/ist NULL
 */
status deal_highest_card(Deck *deck, Card *highest_card);

/**
 * @brief Zieht eine bestimmte Karte aus dem Deck. Hier: Die niedrigste
 * @param deck Das Deck aus welchem die Karte genommen wird
 * @param card die Karte selbst; hier die Karte des Decks am übergebenen Index
 * @param index der Index der Karte, die gezogen werden soll
 * @return statuscodes:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Karte oder Deck ist nicht initialisiert/ist NULL
 */
status deal_card_by_index(Deck *deck, Card *card, size_t index);

#endif
