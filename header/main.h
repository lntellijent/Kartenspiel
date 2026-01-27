//
// Created by hoerz on 09.01.2026.
//

#ifndef KARTENSPIEL_MAIN_H
#define KARTENSPIEL_MAIN_H

typedef enum {
    FALSE,
    TRUE
} boolean; // Boolesche Werte

typedef enum {
    OK, // Erfolgreich bzw ohne Fehler
    USER_INPUT_ERROR, // falsche Benutzereingabe
    OUT_OF_RANGE_ERROR, // Index nicht in Array vorhanden
    NULL_POINT_ERROR, // Deck leer, Initialisierungsfehler, etc.
    CRITICAL_ERROR, // Kritische Errors
    INVALID_ARGUMENT, // Ein Argument im Code ist in diesem Zustand nicht brauchbar
    PRINT_ERROR // print-Statement fehlgeschlagen
} status;



status game_start();

#endif //KARTENSPIEL_MAIN_H