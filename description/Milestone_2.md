# Angabe Milestone 2

Nachdem die Grundelemente des Spiels in Milestone 1 erstellt wurden, wird das Spiel in Milestone 2 über die Kommandozeilenumgebung spielbar gemacht. Dafür werden Befehle von der\*dem Spieler\*in entgegengenommen und verarbeitet. Das Programm signalisiert mit einem Prompt, dass es auf einen Befehl der\*des Spieler\*in wartet (siehe [Milestone 1](description/Milestone_1.md)). Wird nichts oder nur Whitespace eingegeben, wird der Prompt ein weiteres Mal ausgegeben.  
Eingegebene Befehle werden nur dann ausgeführt, wenn sie mit den Spielregeln (siehe [Befehle](#befehle)) im Einklang stehen. Sollte ein Befehl eine Spielregel verletzen, muss

- die entsprechende Fehlermeldung ausgegeben werden (siehe [Fehlermeldungen](#fehlermeldungen)),
- nichts am Spielplan geändert werden und
- nach dem nächsten Befehl gefragt werden (Prompt ausgeben).

Ein Spielzug verläuft so:
1. Der Spielplan mit Zusatzinformationen wird ausgegeben, falls die Spielplanausgabe nicht deaktiviert ist (siehe [Befehl `map`](#ausgabe-des-spielplans-ein-oder-ausschalten))).
2. Der Prompt wird ausgegeben
3. Die\*der Spieler\*in gibt Befehle ein

Die Befehle werden nachfolgend erklärt.

## Befehle

Whitespace am Beginn/Ende von Eingaben und mehrfache Leerzeichen zwischen Befehl und Parameter sowie zwischen den Parametern sollen ignoriert werden. Das heißt wieviele Leerzeichen vor dem Befehl, zwischen Befehl und Parametern, zwischen den Parametern, oder am Ende der Eingabe stehen, hat keine Einfluss darauf ob eine Eingabe gültig ist oder nicht.

Eingaben in Groß- und/oder Kleinbuchstaben sind gültig (case-insensitive). Das heißt, dass `quit` genauso gültig ist wie `QUIT`, `Quit`, `qUIt`, ….

Nachfolgend werden Befehle aufgelistet, welche die\*der Spieler\*in in der Kommandozeilenumgebung eingeben kann.

### Beenden des Spiels
- `quit` / EOF (end of file): Beendet das Spiel. (Returnwert `0`)


### Hilfetext
- `help`: Gibt den folgenden Hilfetext aus.
```
Commands:\n
 - help\n
    Prints this help text.\n
\n
 - quit\n
    Terminates the game.\n
\n
 - map\n
    Activates or deactivates the map.\n
\n
 - flip\n
    Changes the possible move direction.\n
\n
 - move <CHARACTER> <DIRECTION> <DISTANCE>\n
    Moves a character.\n
    <CHARACTER>: the character to move, T/F/S\n
    <DIRECTION>: the direction to move, up/down/left/right\n
    <DISTANCE>: optional, the distance to move\n
\n
 - unlock\n
    Unlocks a nearby door.\n
\n
 - fight\n
    Fights a nearby monster.\n
\n
 - scry <KNOWN_ROOM> <DIRECTION>\n
    Reveals an unknown room.\n
    <KNOWN_ROOM>: where to scry from\n
    <DIRECTION>: which direction to scry\n
\n
```
Danach wird wieder der Prompt ausgegeben.


### Ausgabe des Spielplans ein- oder ausschalten
- `map`: Aktiviert oder deaktiviert die Ausgabe des Spielplans.

Zu Beginn des Spiels ist die Spielplanausgabe aktiv.

Wenn die Spielplanausgabe aktiv ist, und `map` eingegeben wird, wird die Spielplanausgabe deaktiviert. Es wird sofort erneut der Prompt ausgegeben. Für die nachfolgenden Züge wird der Spielplan nicht mehr zur Gänze ausgegeben, sondern nur noch der Card-Flip-Counter und die mögliche Bewegungsrichtung.

Wenn die Spielplanausgabe deaktiviert ist, und `map` eingegeben wird, wird sofort einmalig der Spielplan inklusive Card Flip Counter und der möglichen Bewegungsrichtung und danach erneut der Prompt ausgegeben. Für die nachfolgenden Züge ist die Spielplanausgabe wieder aktiv.


### Neues Bewegungskärtchen aufdecken
- `flip`: Verschiebt das aktuelle Bewegungskärtchen an die letzte Stelle im Bewegungskärtchenstapel und deckt das nächste auf.

Dieser Befehl erhöht den Card-Flip-Counter um 1.

Danach werden der Card-Flip-Counter, der Spielplan (falls Spielplanausgabe aktiv), die (neue) mögliche Bewegungsrichtung und der Prompt ausgegeben.

### Charakter bewegen
- `move`: Bewegt einen Charakter und hat die folgenden Parameter:
  - `T`/`F`/`S`: Gibt an, welcher Charakter bewegt werden soll (Thief, Fighter, Seer).
  - `up`/`down`/`left`/`right`: Gibt an, in welche Richtung der Charakter bewegt werden soll.
  - \<Distanz\>: Die Anzahl der Felder, die gegangen werden soll (als positiver Integer). Dies ist ein optionaler zusätzlicher Parameter. Wird keine Distanz angegeben, bewegt sich der Charakter um genau ein Feld.

Kommt ein Charakter am Ende einer Bewegung auf einem Feld zu stehen, das direkt an einen bisher unbekannten Raum grenzt, wird dieser Raum automatisch aufgedeckt.

Kommt ein Charakter am Ende einer Bewegung auf einem Sanduhr-Feld zu stehen, wird der Card-Flip-Counter automatisch um 5 (minimal auf 0) reduziert. Das Sanduhr-Feld verwandelt sich dabei in einen normalen Gang.

Wenn nach Ende einer Bewegung alle Charaktere gleichzeitig auf dem ihnen zugehörigen Schalter stehen, werden alle geheimen Türen geöffnet. Alle geheimen Türen und alle Schalter verwandeln sich in einen normalen Gang.

Nach Ende der Bewegung werden der Card-Flip-Counter, der Spielplan (falls Spielplanausgabe aktiv), die mögliche Bewegungsrichtung und der Prompt ausgegeben.

##### Unmögliche Bewegungsbefehle
Wenn ein unmöglicher Bewegungsbefehl eingegeben wird, soll die entsprechende [Fehlermeldung](#fehlermeldungen) und danach erneut der Prompt ausgegeben werden.  
- Es können nur Bewegungen in jene Richtung durchgeführt werden, deren Bewegungskärtchen gerade aufgedeckt ist.
- Sollte ein nicht über- bzw. betretbares Feld (z. B. Wand, geschlossene Tür, Monster) den Weg blockieren, oder die Bewegung aus der Karte hinausführen, kann die Bewegung nicht ausgeführt werden. Bei mehreren Schritten in eine Richtung (also wenn eine Distanz > 1 eingegeben wurde), muss die gesamte Bewegung möglich sein.  
- Nach Ende der Bewegung darf sich nur je ein Charakter auf einem Feld befinden. Es ist einem Charakter jedoch erlaubt, ein Feld, auf dem sich ein anderer Charakter befindet, zu überqueren.
- Sollten sich in einem Raum ein oder mehrere Monster befinden, kann dieser Raum nur von der\*dem Kämpfer\*in betreten werden.


### Verschlossene Tür aufsperren
- `unlock`: Sperrt eine verschlossene Tür auf.

Wenn sich die\*der Dieb\*in neben (also links/rechts/darüber/darunter - Felder diagonal zum Charakter zählen nicht) einer verschlossenen Tür befindet, wird diese aufgesperrt. Dies bedeutet, dass sich das Tür-Feld in einen normalen Gang verwandelt. Dies funktioniert nur für eine normale Tür, die geheimen Türen können nur durch das gleichzeitige Betreten der Schalter durch alle Charaktere geöffnet werden.

Wenn sich die\*der Dieb\*in gleichzeitig neben mehreren verschlossenen Türen befindet, werden alle diese Türen aufgesperrt.

Danach werden der Card-Flip-Counter, der Spielplan (falls Spielplanausgabe aktiv), die mögliche Bewegungsrichtung und der Prompt ausgegeben.

*Tipp: Dieser Befehl betrifft immer die\*den Dieb\*in.*

##### Unmögliche `unlock` Befehle

Befindet sich keine Tür neben dem\*der Dieb\*in, wenn der Befehl eingegeben wird, so soll die entsprechende [Fehlermeldung](#fehlermeldungen) und danach erneut der Prompt ausgegeben werden.


### Monster bekämpfen

- `fight`: Bekämpft ein Monster.

Wenn sich die\*der Kämpfer\*in neben (also links/rechts/darüber/darunter - Felder diagonal zum Charakter zählen nicht) einem Monster befindet, wird dieses bekämpft. Dies bedeutet, dass sich das Monster-Feld in einen normalen Gang verwandelt.

Wenn sich die\*der Kämpfer\*in gleichzeitig neben mehreren Monstern befindet, werden alle diese Monster bekämpft.

Danach werden der Card-Flip-Counter, der Spielplan (falls Spielplanausgabe aktiv), die mögliche Bewegungsrichtung und der Prompt ausgegeben.

*Tipp: Dieser Befehl betrifft immer die\*den Kämpfer\*in.*

##### Unmögliche `fight` Befehle

Befindet sich kein Monster neben der\*dem Kämpfer\*in, wenn der Befehl eingegeben wird, so soll die entsprechende [Fehlermeldung](#fehlermeldungen) und danach erneut der Prompt ausgegeben werden.


### Entfernten Raum aufdecken

- `scry`: Deckt einen noch unaufgedeckten Raum auf und benötigt folgende Parameter:
  - `<room_id>` eines bereits aufgedeckten Raumes: Gibt an, von welchem Raum aus ein nebenliegender Raum aufgedeckt werden soll.
  - `up`/`down`/`left`/`right`: Gibt an, ob der Raum über, unter, links oder rechts von dem zuvor definierten Ausgangsraum aufgedeckt werden soll.

Wenn sich die\*der Wahrsager\*in auf einem Kristallkugel-Feld ('Crystal ball') befindet, kann sie\*er einen noch unbekannten Raum, der an einen bereits aufgedeckten Raum angrenzt, aufdecken. Somit wird bei allen folgenden Ausgaben des Spielplans auch der neu aufgedeckte Raum angezeigt. Durch diese Aktion verwandelt sich das Kristallkugel-Feld in einen normalen Gang.

Danach werden der Card-Flip-Counter, der Spielplan mit dem neu aufgedeckten Raum (falls Spielplanausgabe aktiv), die mögliche Bewegungsrichtung und der Prompt ausgegeben.

*Tipp: Dieser Befehl betrifft immer die\*den Wahrsager\*in.*

##### Unmögliche `scry` Befehle
Wenn ein unmöglicher `scry` Befehl eingegeben wird, soll die entsprechende [Fehlermeldung](#fehlermeldungen) und danach erneut der Prompt ausgegeben werden.
- Die\*der Wahrsager\*in muss auf einem Kristallkugel-Feld stehen.
- Es muss ein *aufgedeckter* Raum mit der gegebenen `room_id` vorhanden sein.
- An der angegebenen Stelle muss sich ein noch unbekannter Raum befinden.
  - Wenn z. B. der Raum `C` in der untersten Reihe ist, dann ist `scry C down` nicht möglich.
  - Wenn sich z. B. unter dem Raum `C` ein bereits aufgedeckter Raum befindet, dann ist `scry C down` nicht möglich.


## Fehlermeldungen
Wenn ein falscher oder unmöglicher Befehl eingegeben wird, soll die entsprechende Fehlermeldung und danach erneut der Prompt ausgegeben werden.

Vor der jeweiligen Fehlermeldung soll der Charakter welcher von der Fehlermeldung betroffen ist, ausgegeben werden (*Ausnahme: Fehlermeldung 1-3*). Dieser wird in den folgender Tabelle mit `<C>`dargestellt, sofern nicht aufgrund des Befehls klar ist, welcher Charakter betroffen ist. In der Ausgabe ist `<C>` dann jeweils mit `Thief`, `Fighter` oder `Seer` zu ersetzen.

Pro eingegebenem Befehl wird immer nur eine einzige Fehlermeldung ausgegeben. Falls mehrerer Fehler auftreten sollten, wird jene Fehlermeldung ausgegeben, die sich in der folgenden Tabelle weiter oben befindet.

| Befehl      | Grund                  | Fehlermeldung          |
| ----------- |:----------------------:|:----------------------:| 
| ?           | Ein unbekannter Befehl wurde eingegeben. | `Please enter a KNOWN COMMAND to not confuse your treasure hunters!\n`|
| alle        | Eine für den jeweiligen Befehl unzulässige Anzahl an Parametern wurde angegeben. | `Please enter a correct NUMBER OF PARAMETERS to not confuse your treasure hunters!\n`|
| `move`      | Der zu bewegende Charakter wurde nicht korrekt spezifiziert. | `Who do you want to move?!?\n`|
| `move`      | Die Bewegungsrichtung wurde nicht korrekt eingegeben. | `<C>: "I don't understand where I should go!"\n`|
| `move`      | Die angegebene Distanz ist kein positiver Integer. | `<C>: "I don't understand how far I should go!"\n`|
| `move`      | Das entsprechende Bewegungskärtchen ist gerade nicht aktiv. | `<C>: "I can't go that way right now!"\n`|
| `move`      | Nicht über- bzw. betretbares Feld blockiert den Weg oder der Weg führt aus der Karte. | `<C>: "My way is blocked!"\n`|
| `move`      | Befehl würde zu zwei Charakteren am gleichen Feld führen. | `<C>: "There is not enough space on that tile!"\n`|
| `move`      | Ein Monster befindet sich in dem Raum, welchen die\*der Dieb\*in oder die\*der Wahrsager\*in betreten möchte. | `<C>: "That room is too scary for me!"\n`|
| `unlock`    | Neben der\*dem Dieb\*in befindet sich keine verschlossene Tür. | `Thief: "Nothing to unlock here!"\n`|
| `fight`     | Neben der\*dem Krieger\*in befindet sich kein Monster. | `Fighter: "Nothing to fight here!"\n`|
| `scry`      | Die\*der Wahrsager\*in befindet sich nicht auf einem Kristallkugel-Feld.   | `Seer: "I can't scry without my magic crystal ball!"\n`|
| `scry`      | Invalide Parameter wurden eingegeben.  | `Seer: "I don't understand which room I should scry!"\n`|
| `scry`      | Es befindet sich kein Raum an der angegebenen Position.    | `Seer: "There is no room I can reveal at this position!"\n`|
| `scry`      | Der gefragte Raum ist bereits aufgedeckt. | `Seer: "We already know that room..."\n`|


## Spielende

Wenn sich alle Charaktere gleichzeitig auf einem Schatzfeld befinden, ist das Spiel zu Ende. Es wird die folgende Nachricht ausgegeben und das Programm mit Returnwert 0 beendet.
```
You win, congratulations! It took you <card flip counter> card flips to find the treasure.\n
```


