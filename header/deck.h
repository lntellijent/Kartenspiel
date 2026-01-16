//
// Created by hoerz on 15.12.2025.
//

#ifndef DECK_H
#define DECK_H
#include "card.h"
#include "main.h"

typedef struct {
    Card *cards;
    int card_count;
    int capacity;
} Deck;

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
Deck *create_empty_deck(int initial_capacity);

/**
 * @brief mischt das übergebene Deck
 * @param d das zu mischende Deck
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: Deck nicht initialisiert
 */
status shuffle(const Deck *d);

/**
 * @brief Prüft, ob das übergebene Deck leer ist oder noch Elemente enthält
 * @param d das zu überprüfende Deck
 * @return TRUE, falls das Deck entweder leer oder null ist, sonst FALSE
 */
boolean is_empty(const Deck *d);

/**
 * @brief Zieht (virtuell) die oberste Karte (LIFO-Prinzip). Nur zufällig, falls davor gemischt wurde.
 * @param d Das Deck aus welchem gezogen werden soll
 * @param out Die Karte die gezogen wird
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 */
status deck_draw_top(Deck *d, Card *out);

/**
 * @brief Zieht eine Karte aus einer bestimmten Stelle
 * @param source_deck Das Deck aus welchem gezogen werden soll
 * @param out Die Karte die gezogen wird
 * @param index Der Index an welchem die Karte abliegt
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 */
status deck_draw_index(Deck *source_deck, Card *out, int index);

/**
 * @brief fügt eine Karte dem Deck hinzu
 * @param source_deck Das Deck, welchem die Karte hinzugefügt werden soll
 * @param card_output Die Karte die eingefügt wird
 * @return Statuscode:
 * - OK: Erfolgreich
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
status insert(Deck *source_deck, const Card *card_output);

/**
 * @brief Zählt den Punktewert des Decks
 * @param source_deck Das zu zählende Deck
 * @return Statuscode:
 * - Wert >= 0: Fehlerfrei, enthält den entsprechenden Deck-Wert
 * - -1: NULL_POINT_ERROR
 * @warning Leert das Deck vollständig!
 */
int consume_and_count_worth(Deck *source_deck);

/**
 * @brief Teilt die Spielkarten entsprechend aus
 * @param main_deck Der "Ziehstapel"
 * @param destination_deck Der Spielerstapel; die Handkarten
 * @param card_count Die Anzahl der zu verwendenden Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: leeres oder nicht initialisiertes Deck
 * - CRITICAL_ERROR: Array konnte nicht vergrößert werden
 */
status card_deal(Deck *main_deck, Deck *destination_deck, int card_count);

/**
 *
 * @param source_deck Das Deck, welches angezeigt werden soll
 * @param print_indexes Anzeige der Indexnummern jeder Karte eine Zeile unter den Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Deck ist leer oder =NULL
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status print_deck(Deck *source_deck, boolean print_indexes);

status deal_lowest_card(Deck *deck, Card *lowest_card);
#endif
