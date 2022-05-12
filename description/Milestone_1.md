# Angabe Milestone 1

Ziel des Milestone 1 (M1) ist es, eine Grundstruktur für das Spiel zu erstellen und der Spielplan auszugeben. Im Rahmen von M1 werden grundlegende Klassen erstellt.

## Feedbackgespräche

- Nach M1 werden Feedbackgspräche stattfinden, im Rahmen derer jedes Team Feedback zum bisherigen Stand des Projekts bekommen kann. Das Feedbackgespräch ist optional aber es wird stark empfohlen, diese Möglichkeit zu nutzen.
- Push auf das Gitlab Repository des Teams **auf einen Branch namens `feedback`**
- **Abzugeben bis: 14.05.2022 um 23:59 Uhr**

## Vorgegebene Klassen

Begonnen werden sollte am besten mit dem Erstellen der Klassen, um eine Grundstruktur für das Spiel festzulegen.

Die hier angegebenen Klassen können so implementiert werden, wie sie hier beschrieben sind. Es wird empfohlen, die angegebenen Klassen sinnvoll zu erweitern (soweit dies nötig erscheint) sowie weitere Klassen zu erstellen, wo dies sinnvoll ist. Natürlich ist es auch erlaubt, eine andere (objektorientierte) Programmstruktur zu wählen.

> **Achtung:** Es muss eine geeignete Ableitungshierarchie (Klassen mit Vererbung) implementiert werden, da das Anwenden von Vererbung ein zentrales Lernziel der KU ist. Eine Lösung ohne einer selbst geschriebenen Basisklasse und davon abgeleiteten Klassen wird daher als Spezifikationsverstoß gewertet. Ein Beispiel dafür, wo Vererbung genutzt werden könnte, ist die Klasse [Tile](#klasse-tile).

Wenn in den folgenden Ausführungen kein Datentyp vorgegeben ist, soll eigenständig ein passender Datentyp gewählt werden. 

Sofern nicht näher beschrieben, sind bei den Klassen Konstruktor(en) und Destruktor sowie Getter und Setter für alle Attribute zu implementieren.

Wenn nicht anders definiert, dürfen Destruktor, Copy-Konstruktor und Copy-Assignment-Konstruktor auf default/delete gesetzt werden.

> **Achtung:** Shallow copy muss vermieden werden!

### Klasse `Room`
Klasse, die einen Raum, welcher aus mehreren Feldern (Tiles) besteht, beschreibt.

#### Attribute
- `room_id_`       (ein Großbuchstabe (A-Z), welcher den Raum kennzeichnet)
- `room_map_` (ein 5x5 Array aus `Tile*`, die auf die einzelnen Felder des Raumes zeigen)
- `revealed_` (gibt an, ob der Raum bereits aufgedeckt wurde)

#### Methoden
- Konstruktor mit den Parametern `(room_id, room_info_str)` 
  (Der `room_info_str` ist hierbei der bereitgestellte String, welcher die Anordnung der Felder in diesem Raum mit Kürzeln beschreibt (siehe [Erstellen der einzelnen Räume](#erstellen-der-einzelnen-räume).)
- Getter für `room_id_`, `room_map_`, `revealed_`
- Setter für `revealed_` (keine Setter für `room_id_` und `room_map_`) 


### Klasse `Tile`
Klasse, die ein Tile (Feld) beschreibt.

#### Attribute
- `character_` (Pointer zu dem jeweiligen Charakter, der sich auf dem Feld befindet)

> **Tipp:** Überlegt euch, welche weiteren Attribute eventuell sinnvoll sein könnten. 

#### Methoden
- pure virtual Methode `getTileString()` (gibt die Repräsentation des aktuellen Feldes auf dem auszugebenden Spielplan zurück)


### Klasse `BasicTile` (Subklasse von `Tile`)
Klasse, die ein Tile ohne magische Funktionen beschreibt. Dies kann entweder ein Wandfeld ('Wall') oder ein Gang ('Passage') sein.

#### Methoden
- `getTileString()` (gibt die Repräsentation des aktuellen Feldes auf dem auszugebenden Spielplan zurück)

### Klasse `MagicTile` (Subklasse von `Tile`)
Klasse, die Tiles mit magischer Funktion beschreibt. Die folgenden Tiles haben magische Funktionen:
- normale Tür ('Door')
- geheime Tür ('Secret door')
- Monster ('Monster')
- Sanduhr ('Hourglass')
- Kristallkugel ('Crystal ball')
- Schatz ('Loot')
- Schalter ('Button')

#### Methoden
- `getTileString()` (gibt die Repräsentation des aktuellen Feldes auf dem auszugebenden Spielplan zurück)
- `magicUsed()` (verwandelt das Feld in ein leeres Gangfeld ('Passage') z.B.: durch Öffnen der Tür, Bekämpfen des Monsters, ...)

**Ausnahme** ist hierbei das Schatzfeld ('Loot') welches sich nicht in ein Gangfeld verwandeln kann. Genauere Infos hierzu findet ihr in [der Beschreibung Spiellogik](Milestone_2.md). Dennoch darf die Methode `magicUsed()` für alle MagicTiles implementiert werden. 

> **Tipp:** Die Gestaltung dieser Klasse sollt ihr euch selbst einfallen lassen. Überlegt also, welche weiteren Attribute und Methoden gebraucht werden und ob weitere Ableitungen eventuell sinnvoll sein könnten. 
Lest dafür am besten auch die Angabe von [Milestone 2](Milestone_2.md), um einen guten Überblick über das Spiel zu bekommen. 


### Klasse `Character`
Klasse, die einen Charakter im Spiel, also eine Spielfigur repräsentiert. Ein Charakter kann vom Typ 'Fighter', 'Thief', oder 'Seer' sein.

#### Attribute
- `type_` gibt den Typ des Characters an


### Klasse `Game`
Die Spielklasse regelt das Spiel und hat die Daten, die es dazu braucht. Es soll während des gesamten Spiels nur ein `Game`-Objekt genutzt werden. Deshalb soll die Klasse `Game` als Singleton realisiert werden. ([Erklärung Singleton Klasse](Singleton.md))

> **Tipp:** Die genaue Gestaltung dieser Klasse sollt ihr euch selbst einfallen lassen. Überlegt also, welche  Attribute und Methoden gebraucht werden, um das Spiel zu regeln. Die Details können in [Milestone 2](Milestone_2.md) implementiert werden, aber ihr solltet für Milestone 1 bereits die Grundlagen festlegen.


## Spielstart

Das Spiel beginnt wie folgt:
1. Anordnung der Räume einlesen/parsen
2. Generieren des Spielplans
3. Platzieren der Charaktere am Spielplan
4. Erstellen und Mischen der Bewegungskärtchen
5. Ausgabe des Welcome-Prompts `Welcome to the magical OOP1 Maze!!!\n`
6. Spielplan einmalig ausgeben
7. Ausgabe des Command-Prompts

## Einlesen und Generieren des Spielplans 
Das Generieren des Spielplans erfolgt in zwei Schritten: Zunächst wird die Anordnung der Räume mittels Kommandozeilenparametern eingelesen, danach wird für jeden der Räume die `room_map_` erstellt, welche die einzelnen Felder des Raumes beinhaltet.

### Einlesen / Parsen
Beim Programmstart soll die Raumanordnung in Form von Kommandozeilenparametern übergeben werden. Dabei wird jeder Raum durch seine ID (A-Z) beschrieben. Die Anordnung der Räume wird durch die Reihenfolge und das Setzen von Leerzeichen beschrieben. Ein Leerzeichen zwischen zwei Buchstaben beschreibt hierbei den Wechsel in eine neue Raum-Zeile des Spielplans.

Durch die Eingabe von `./a2 AB CL SE` (a2 entspricht hierbei dem Namen des Executables) wird demnach die folgende Raumanordnung produziert:
```
A B
C L
S E
```

Um den Spielplan in [Beispiel 2](#beispiel-2) und [Beispiel 3](#beispiel-3) zu produzieren, wären somit folgende Kommandozeilenparameter nötig: `SAD BCL` 


Für eine valide Konfiguration müssen diese Voraussetzungen erfüllt werden:
1. Die eingegebenen Kommandozeilenparameter müssen dem beschriebenen Format entsprechen (nur Großbuchstaben bzw. Leerzeichen als Trennung zwischen den Parametern / Raum-Zeilen des Spielplans).
2. Es dürfen keine weiteren Kommandozeilenparameter eingegeben werden.
3. Der Spielplan muss aus zumindest einer Raum-Zeile bestehen.
4. Eine Raum-Zeile des Spielplans muss aus mindestens einem und maximal 5 Räumen bestehen.
5. Der Spielplan muss ein vollständiges Rechteck ergeben. Das heißt jede Raum-Zeile des Spielplans muss gleich viele Räume enthalten.
6. Jeder Raum darf maximal ein Mal vorkommen.
7. Jede Konfiguration muss genau einen Startraum `S` enthalten.

Wird eine invalide Konfiguration eingegeben, so soll folgende Fehlermeldung ausgegeben und das Programm mit dem Returnwert `2` beendet werden. 
```
Error: Invalid configuration!\n
```
*Alle möglichen Rückgabewerte des Programms werden [in einer Tabelle](#returnwerte) noch einmal zusammengefasst.*

*Hinweis*: Eine Überprüfung der Lösbarkeit des Spiels aus der gegebenen Konfiguration wird nicht verlangt und soll nicht erfolgen.

### Erstellen der einzelnen Räume
Um den Spielplan zu generieren, werden die als Kommandozeilenparameter angegebenen Räume geladen. Dafür ist die von uns zu Verfügung gestellte `RoomInfo` Klasse zu verwenden. Im Speziellen soll folgende Methode der Klasse verwendet werden:
```
std::string getRoomInfoString(char room_id) 
```

Dieser Methode wird als `room_id` jeweils einer der als Kommandozeilenparameter eingegebenen Buchstaben übergeben. Sie gibt einen String zurück, welcher den Raum in eindeutiger Form beschreibt. Dabei wird für jedes Tile genau ein Buchstabe verwendet. Da jeder Raum genau aus 5\*5 Tiles besteht, enthält der zurückgegebene String genau 25 Buchstaben. Die ersten 5 Buchstaben stellen die erste Tile-Zeile des Raumes dar etc. (*Es darf von der Korrektheit des Strings ausgegangen werden.*)

Sollte ein als Kommandozeilenparameter eingegebener Buchstabe keine gültige `room_id` sein, so wird ein leerer String zurückgegeben. In diesem Fall soll ebenfalls die nachfolgende Fehlermeldung ausgegeben und das Programm mit dem Returnwert `2` beendet werden. 
```
Error: Invalid configuration!\n
```

Das Kürzel jedes Tiles kann folgender Tabelle (sowie dem Kapitel [Felder](#felder)) entnommen werden.
| Kürzel                 | Tile-Typ               |
| ---------------------- |:----------------------:| 
| W                      | Wand                   |
| P                      | Gang                   |  
| X                      | geheime Tür            |
| M                      | Monster                |
| G                      | Sanduhr                |
| C                      | Kristallkugel          |
| H                      | Tür horizontal         |
| V                      | Tür vertikal           |
| L                      | Schatz                 |
| T                      | Schalter Dieb\*in       |
| F                      | Schalter Krieger\*in    |
| S                      | Schalter Wahrsager\*in  |

Damit können aus dem `room_info_string` die Tiles erstellt werden, die im jeweiligen Raum enthalten sind.

## Platzieren der Charaktere am Spielplan
Die Charaktere des Spiels werden immer in der selben Anordung am Spielplan platziert. Dabei befinden sie sich immer im Startraum. Die genaue Anordung ist den [Beispielen](#beispiel-spielpläne) zu entnehmen.

## Erstellen und Mischen der Bewegungskärtchen

Die\*der Spieler\*in hat vier Kärtchen, auf denen jeweils eine Bewegungsrichtung (hinauf, rechts, hinunter, links) abgebildet ist. Diese werden zu Spielbeginn gemischt und das oberste Kärtchen aufgedeckt. Es zeigt an, welche Bewegungsrichtung gerade erlaubt ist.

Die Bewegungskärtchen werden zu Beginn in der Reihenfolge `up`, `right`, `down`, `left` gespeichert.

Es wird mit der bereitgestellten `Random`-Klasse eine Zahl von 1 bis zur Anzahl der noch nicht ausgeteilten Bewegungskärtchen bestimmt (Aufruf der Methode `getRandomCard()`). Das Bewegungskärtchen, welches ursprünglich diese Position hat (also z. B. Position 3 wäre das Kärtchen `down`) wird in der Reihenfolge an erster Stelle hinzugefügt, ist also zu Spielbeginn aktiv.

Danach wird erneut eine Zahl über die `Random`-Klasse bestimmt. Das Bewegungskärtchen, welches diese Position hat, wird in der Reihenfolge an zweiter Stelle eingefügt. Dies wird wiederholt bis alle vier Bewegungskärtchen verwendet wurden.

### Beispiel

| Position | Kärtchen |
| -------- | -------- |
| 1        | up       |
| 2        | right    |
| 3        | down     |
| 4        | left     |

`getRandomCard()` gibt 3 zurück &rarr; Kärtchen `down` wird hinzugefügt\
&rarr; Kärtchen-Reihenfolge: `down`  

| Position | Kärtchen |
| -------- | -------- |
| 1        | up       |
| 2        | right    |
| 3        | left     |

`getRandomCard()` gibt 1 zurück &rarr; Kärtchen `up` wird hinzugefügt\
&rarr; Kärtchen-Reihenfolge: `down`, `up`  

| Position | Kärtchen |
| -------- | -------- |
| 1        | right    |
| 2        | left     |

`getRandomCard()` gibt 2 zurück &rarr; Kärtchen `left` wird hinzugefügt\
&rarr; Kärtchen-Reihenfolge: `down`, `up`, `left`  

| Position | Kärtchen |
| -------- | -------- |
| 1        | right    |

`getRandomCard()` gibt 1 zurück (da nun nur mehr ein Kärtchen übrig ist) &rarr; Kärtchen `right` wird hinzugefügt\
&rarr; Kärtchen-Reihenfolge: `down`, `up`, `left`, `right`  

Zu Spielbeginn ist das Kärtchen `down` aktiv.


## Ausgabe des Spielplans

Der Spielplan soll als ASCII-Art im Terminal ausgegeben werden. Hierzu wurde die Raumanordnung zuvor mittels Kommandozeilenparametern eingegeben (siehe [Einlesen und Generieren des Spielplans ](#einlesen-und-generieren-des-spielplans)).
Jeder Raum ('Room') besteht dabei aus 5\*5 Feldern ('Tile'). Jedes dieser Felder besteht wiederum aus 7\*3 Zeichen (7 Spalten und 3 Zeilen). Sollte sich ein Charakter auf einem Feld befinden, wird das über einen Buchstaben in Feldmitte angezeigt ('F' für Fighter, 'T' für Thief, 'S' für Seer).

Das Feld in der linken oberen Ecke jedes Raumes ist immer eine Wand ('Wall'). Hier soll mittig als Zusatzinformation ein Buchstabe ausgegeben werden, um den Raum zu kennzeichnen (`room_id_` A-Z). Solange ein Raum noch nicht aufgedeckt wurde, werden alle seine Felder als unbekannt dargestellt.

Um die Raumgrenzen anzuzeigen, wird eine doppelte Trennlinie verwendet. Die Charaktere starten immer in Raum 'S' in der in [Beispiel 1](#beispiel-1) gezeigten Anordnung. Über dem Spielplan wird angezeigt, wie oft bereits ein Bewegungskärtchen aufgedeckt wurde (rechtsbündig, mit Platz für 3 Stellen) (siehe [Beispiel-Spielpläne](#beispiel-spielpläne)).

Zu Spielbeginn sind alle Türen (geheime und normale) geschlossen. Im Laufe des Spiels benutzte Bonusfelder und Schalter, besiegte Monster und geöffnete Türen werden gleich dargestellt wie ein normaler Gang.

*Hinweis:* Die Schalter dienen dem Öffnen der Schatzkammer. Jedem Charakter ist ein individueller Schalter zugeordnet (Siehe [Spielende](../README.md#spielende) oder [Charakter bewegen](Milestone_2.md#charakter-bewegen)).

### Felder
Die einzelnen ASCII-Felder werden nachfolgend dargestellt (bitte markieren, um die Leerzeichen sichtbar zu machen). Zusätzlich wird angeführt, welches Kürzel verwendet wird, um ein Feld dieser Art in einem Raum zu symbolisieren (siehe [Erstellen der einzelnen Räume](#erstellen-der-einzelnen-räume)).  
Weiters ist angegeben, ob das Feld (in seinem derzeitigen Zustand) betretbar und/oder übertretbar ist. Betretbar bedeutet, dass ein Charakter auf diesem Feld seine Bewegung beenden kann, übertretbar, dass das Feld überquert werden kann.
Die Zeile 'veränderbar' gibt an, ob und wann das Feld seine Darstellung ändert. Dies sind Zusatzinformationen, die für Milestone 2 relevant sind. Sie könnten jedoch vielleicht eure Gestaltung der Klasse MagicTile beeinflussen, daher haben wir sie bereits in Milestone 1 angegeben.

**Unbekanntes Feld**  
Kürzel: keines  
betretbar: nein  
übertretbar: nein  
veränderbar: ja, nach Aufdecken des Raumes wird das sich tatsächlich an dieser Stelle befindliche Feld angezeigt  

```
UUUUUUU
UUUUUUU
UUUUUUU
```

----

**Wand**  
Kürzel: W ('Wall')  
betretbar: nein  
übertretbar: nein  
veränderbar: nein  

```
███████
███████
███████
```

----

**Wand mit Raumkennzeichnung (Beispiel Raum S)**  
Kürzel: W ('Wall')  
betretbar: nein  
übertretbar: nein  
veränderbar: nein  

```
███████
███S███
███████
```

----

**Gang**  
Kürzel: P ('Passage')  
betretbar: ja  
übertretbar: ja  
veränderbar: ja, durch Betreten mit Charakter  

```
       
       
       
```

----

**Gang mit Charakter (Beispiel Fighter)**  
Kürzel: P ('Passage')  
betretbar: nein (Es kann sich nur ein einzelner Charakter auf einem Feld befinden.)  
übertretbar: ja (Charaktere können sich gegenseitig überspringen.)  
veränderbar: ja, durch Verlassen mit Charakter  

```
       
   F   
       
```

----

**Geschlossene geheime Tür**  
Kürzel: X  
betretbar: nein  
übertretbar: nein  
veränderbar: ja, durch Öffnen  

```
███████
███████
███████
```

----

**Offene geheime Tür**  
Kürzel: X  
betretbar: ja  
übertretbar: ja  
veränderbar: nein  

```
       
       
       
```

----

**Monster**  
Kürzel: M ('Monster')  
betretbar: nein  
übertretbar: nein  
veränderbar: ja, durch Besiegen des Monsters  

```
       
   M   
       
```

----

**Sanduhr**  
Kürzel: G ('hourGlass')  
betretbar: ja  
übertretbar: ja  
veränderbar: ja, durch Verwendung des Bonusfelds  

```
 \   / 
  | |  
 /   \ 
```

----

**Kristallkugel**  
Kürzel: C ('Crystal ball')  
betretbar: ja  
übertretbar: ja  
veränderbar: ja, durch Verwendung des Bonusfelds  

```
  / \  
 |   | 
  \ /  
```

----

**Tür horizontal (geschlossen)**  
Kürzel: H ('Horizontal door')  
betretbar: nein  
übertretbar: nein  
veränderbar: ja, durch Öffnen  

```
       
███████
       
```

----

**Tür vertikal (geschlossen)**  
Kürzel: V ('Vertical door')  
betretbar: nein  
übertretbar: nein  
veränderbar: ja, durch Öffnen  

```
   █   
   █   
   █   
```

----

**Tür horizontal oder vertikal (offen)**  
Kürzel: H bzw. V  
betretbar: ja  
übertretbar: ja  
veränderbar: nein  

```
       
       
       
```

----

**Schatz**  
Kürzel: L ('Loot')  
betretbar: ja  
übertretbar: ja  
veränderbar: nein  

```
$$$$$$$
$     $
$$$$$$$
```

----

**Schalter Dieb\*in**  
Kürzel: T ('Thief')  
betretbar: ja  
übertretbar: ja  
veränderbar: ja, durch Verwendung des Schalters  

```
TTTTTTT
T     T
TTTTTTT
```

----

**Schalter Krieger\*in**  
Kürzel: F ('Fighter')  
betretbar: ja  
übertretbar: ja  
veränderbar: ja, durch Verwendung des Schalters  

```
FFFFFFF
F     F
FFFFFFF
```

----

**Schalter Wahrsager\*in**  
Kürzel: S ('Seer')  
betretbar: ja  
übertretbar: ja  
veränderbar: ja, durch Verwendung des Schalters  

```
SSSSSSS
S     S
SSSSSSS
```

## Beispiel-Spielpläne

### Beispiel 1
Spielstart mit einem Spielplan, der nur aus zwei Räumen besteht (2 Spalten, 1 Reihe). Der rechte Raum wurde noch nicht aufgedeckt.
Über dem Spielplan befindet sich der Card Flip Counter, unter dem Spielplan wird angezeigt, dass zu Spielstart das Bewegungskärtchen `right` aufgedeckt wurde und jetzt aktiv ist.

```
Card Flip Counter:   1
╬═══════════════════════════════════╬═══════════════════════════════════╬
║██████████████       ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███S██████████       ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████   F             T   ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║                                   ║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║                                   ║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║                                   ║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████   S                 ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
╬═══════════════════════════════════╬═══════════════════════════════════╬
Possible move: right

```

### Beispiel 2
Während des Spiels mit einem Spielplan, der aus sechs Räumen besteht (3 Räume breit und 2 Räume hoch). Es wurden bereits einige Räume aufgedeckt aber noch nicht die Schalter betätigt, um die Tür zur Schatzkammer zu öffnen.
Das Bewegungskärtchen `up` ist aktiv.
```
Card Flip Counter:  27
╬═══════════════════════════════════╬═══════════════════════════════════╬═══════════════════════════════════╬
║██████████████       ██████████████║███████              ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███S██████████       ██████████████║███A███          M   ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║███████              ██████████████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║              ███████          █   ║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║              ███████          █   ║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║              ███████          █   ║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║                                   ║               \   /        ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║                                   ║          F     | |         ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║                                   ║               /   \        ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║█████████████████████       ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║█████████████████████       ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║███████                     ███████║█████████████████████       ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║SSSSSSS                     ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║S     S                 M   ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
║██████████████       ██████████████║SSSSSSS                     ███████║UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU║
╬═══════════════════════════════════╬═══════════════════════════════════╬═══════════════════════════════════╬
║██████████████       ██████████████║███████       █████████████████████║███████████████████████████████████║
║███B██████████       ██████████████║███C███████████████████████████████║███L███████████████████████████████║
║██████████████       ██████████████║███████       █████████████████████║███████████████████████████████████║
║███████              ██████████████║                            ███████║███████$$$$$$$       $$$$$$$███████║
║███████              ██████████████║          T                 ███████║███████$     $       $     $███████║
║███████              ██████████████║                            ███████║███████$$$$$$$       $$$$$$$███████║
║███████       ███████              ║       ██████████████              ║███████                     ███████║
║███████       ███████              ║       ██████████████              ║███████                     ███████║
║███████       ███████              ║       ██████████████              ║███████                     ███████║
║███████                     ███████║                     ██████████████║███████$$$$$$$       $$$$$$$███████║
║███████          S          ███████║                     ██████████████║███████$     $       $     $███████║
║███████                     ███████║                     ██████████████║███████$$$$$$$       $$$$$$$███████║
║██████████████  / \  ██████████████║██████████████       TTTTTTT███████║███████████████████████████████████║
║██████████████ |   | ██████████████║██████████████       T     T███████║███████████████████████████████████║
║██████████████  \ /  ██████████████║██████████████       TTTTTTT███████║███████████████████████████████████║
╬═══════════════════════════════════╬═══════════════════════════════════╬═══════════════════════════════════╬
Possible move: up
```

### Beispiel 3
Spielende auf dem selben Spielplan wie Beispiel 2. Es wurden
 - alle Räume aufgedeckt,
 - Monster besiegt und
 - Bonusfelder verwendet.

Diese drei Dinge sind nicht unbedingt notwendig, um das Spiel zu gewinnen. Außerdem wurden die Schalter durch alle Charaktere gleichzeitig betätigt, um die Türen zur Schatzkammer zu öffnen. Die Charaktere befinden sich jeweils auf einem Schatzfeld.
Das Bewegungskärtchen `left` ist aktiv.
```
Card Flip Counter:  42
╬═══════════════════════════════════╬═══════════════════════════════════╬═══════════════════════════════════╬
║██████████████       ██████████████║███████              ██████████████║███████                            ║
║███S██████████       ██████████████║███A███              ██████████████║███D███                            ║
║██████████████       ██████████████║███████              ██████████████║███████                            ║
║███████                     ███████║              ███████              ║              █████████████████████║
║███████                     ███████║              ███████              ║              █████████████████████║
║███████                     ███████║              ███████              ║              █████████████████████║
║                                   ║                            ███████║              ███████       ███████║
║                                   ║                            ███████║              ███████       ███████║
║                                   ║                            ███████║              ███████       ███████║
║███████                     ███████║█████████████████████       ███████║███████                     ███████║
║███████                     ███████║█████████████████████       ███████║███████                     ███████║
║███████                     ███████║█████████████████████       ███████║███████                     ███████║
║██████████████       ██████████████║                            ███████║███████████████████████████████████║
║██████████████       ██████████████║                            ███████║███████████████████████████████████║
║██████████████       ██████████████║                            ███████║███████████████████████████████████║
╬═══════════════════════════════════╬═══════════════════════════════════╬═══════════════════════════════════╬
║██████████████       ██████████████║███████       █████████████████████║██████████████       ██████████████║
║███B██████████       ██████████████║███C███       █████████████████████║███L██████████       ██████████████║
║██████████████       ██████████████║███████       █████████████████████║██████████████       ██████████████║
║███████              ██████████████║                            ███████║███████$$$$$$$       $$$$$$$███████║
║███████              ██████████████║                            ███████║███████$  T  $       $  S  $███████║
║███████              ██████████████║                            ███████║███████$$$$$$$       $$$$$$$███████║
║███████       ███████              ║       ██████████████              ║                                   ║
║███████       ███████              ║       ██████████████              ║                                   ║
║███████       ███████              ║       ██████████████              ║                                   ║
║███████                     ███████║                     ██████████████║███████$$$$$$$       $$$$$$$███████║
║███████                     ███████║                     ██████████████║███████$     $       $  F  $███████║
║███████                     ███████║                     ██████████████║███████$$$$$$$       $$$$$$$███████║
║██████████████       ██████████████║██████████████              ███████║██████████████       ██████████████║
║██████████████       ██████████████║██████████████              ███████║██████████████       ██████████████║
║██████████████       ██████████████║██████████████              ███████║██████████████       ██████████████║
╬═══════════════════════════════════╬═══════════════════════════════════╬═══════════════════════════════════╬
Possible move: left
```

>Wichtig: Bitte unbedingt exakt die gleichen Symbole wie in den Beispielen verwenden, da sonst die Test Cases fehlschlagen.

## Ausgabe des Command-Prompts
Nach der Ausgabe des Spielplans wird ein Prompt ausgegeben und auf den Input der\*des Spieler\*in gewartet.
Der Prompt sieht wie folgt aus: (Achtung, Leerzeichen vor und nach dem Symbol!)
```
 > 
```
Wird `EOF` eingegeben (End of File, nicht der String `"EOF"`), wird das Programm mit dem Returnwert `0` beendet.
Wird nichts oder nur Whitespace eingegeben, wird der Prompt ein weiteres Mal ausgegeben.
In Milestone 1 müssen noch keine weiteren Benutzereingaben für den Prompt behandelt werden.

## Returnwerte
| Rückgabewert | Beschreibung                      |
| ------------ | --------------------------------- |
| 0            | Spiel per Kommando beendet oder Spiel wurde gewonnen :tada: |
| 1            | Speicher konnte nicht alloziert werden. Fehlermeldung: `Error: Not enough memory!\n`            |
| 2            | Falsche oder zu viele Argumente wurden mitgegeben. Fehlermeldung: `Error: Invalid configuration!\n`       |
