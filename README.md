# Allgemein
### Ziel
Entwickeln Sie das allerneuste, super-interessante, von alle gespielte DHBW-Kartenspiel in C, bei dem ein menschlicher Spieler gegen jeweils einen, aber mit verschiedenen Strategien ausgerüsteten, Computergegner antritt. Das Programm soll die Karten, die Spielzüge und die Auswertung am Ende darstellen, sowie den Namen des Spielers erfragen und darstellen.
- Reihenfolge der Karten: 2<3<4<5<6<7<8<9<10<B<D<K<A
- Punktewertung:
	- $2-10\to$ Kartenwert
	- $B\to2$ Punkte
	- $D\to3$ Punkte
	- $K\to4$ Punkte
	- $A\to11$ Punkte
### Grundlegende Spielregeln
- Es gibt ein Standarddeck mit 52 Karten: Werte 2–10, Bube (B), Dame (D), König (K), Ass (As), jeweils in den vier Farben ♠, ♥, ♦, ♣.
- Farben spielen keine Rolle für die Stichentscheidung
- Für passionierte Kartenspieler (Skat, Doppelkopf, Binokel, etc.): Es gibt auch keinen Trumpf oder Bedienzwang
### Spielablauf:
1. Das Deck wird gemischt.
2. Jeder Spieler (Mensch und Computer) erhält 10 Handkarten. Die restlichen Karten finden in dieser Runde keine Bedeutung
3. Der Startstpieler legt eine Karte. Die erste Karte im Spiel legt immer der Computer, danach kann es wechseln.
4. Der Gegenspieler legt eine Karte.
5. Die höhere Karte gewinnt den Stich. Bei Gleichstand gewinnt die zuerst gespielte Karte.
6. Der Gewinner des Stichs erhält beide Karten und spielt die nächste Karte aus.
7. Gespielt wird, bis keine Handkarten mehr übrig sind.
8. Am Ende werden die Punkte der gewonnenen Karten gezählt. Der Spieler mit den meisten Punkten gewinnt. Bei Gleichstand gewinnt der menschliche Spieler
### Computergegner
- Implementieren Sie verschiedene Computergegner mit unterschiedlichen Strategien
	- Einfacher Gegner: spielt Karten der Reihe nach.
	- Strategischer Gegner: spielt von der höchsten zur niedrigsten Karte.
	- Wechselnder Gegner: spielt abwechselnd die kleinste und größte Karte.
	- Reaktiver Gegner: versucht, die Karte des Gegners gezielt zu übertrumpfen, sonst spielt er die kleinste Karte.
- Zu Beginn einer Spielrunde wird der Computergegner zufällig ausgewählt
## Hilfestellung
- Verwenden Sie structs, um Karten und Spieler zu modellieren. Eine Karte besteht aus Wert und Farbe, ein Spieler unter anderem aus seinem Namen, seinen Handkarten und gewonnenen Stichen
- Nutzen Sie enums, um die möglichen Werte und Farben der Karten darzustellen.
- Arbeiten Sie mit Arrays und Zeigern, um Kartenstapel und Hände zu verwalten
- Implementieren Sie Funktionen für zentrale Aufgaben wie:
	- Erzeugen des Kartenstapels
	- Mischen des Stapels
	- Ziehen von Handkarten
	- Vergleichen von Karten
	- Berechnen der Punkte
- Für die verschiedenen Computergegner können Sie Funktionszeiger einsetzen, um unterschiedliche Strategien flexibel einzubinden.
- Achten Sie auf eine klare Ausgabe der Karten und Ergebnisse auf dem Bildschirm.
- Aus- und Eingabe finden aber ausschließlich auf der Konsole statt. Keine graphischen Bibliotheken sind erforderlich. Aufwendige Darstellungen außerhalb der Konsole führen nicht zur Aufwertung!
- Teilen Sie das Programm sinnvoll auf - bitte nicht alles in einer monolithischen C-Datei. Es sollte auch eine Header-Datei geben und mindestens die Aufteilung in Spielablauf und Karten-/Spielerrelevantes
### Rahmenbedingungen und Termine
- Der Code muss ganz klar von Ihnen erstellt worden sein, die Nutzung von KI-Generatoren ist untersagt. Hilfestellungen von Stackoverflow, anderen Internet-foren, Code-Ergänzungen in der IDE und Gespräche mit Kommilitonen können aber nützlich sein.
- Projektsprache ist Deutsch. Innerhalb von Funktionen sind Sie frei, aber Funktions- und Datenstrukturnamen sollten zur Sprache in der Spielspezifikation passen.
- Abgabe erfolgt über Moodle spätestens bis 08.03.2026 20:00 Uhr
## Planung
- [ ] CLI sauber/leer halten (Stichwort "gotoxy")
- [ ] Mehrere Klassen erstellen zur Trennung
- [ ] structs = Java-Interfaces?
