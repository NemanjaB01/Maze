# Assignment 2 - OOP1 Maze

## Einleitung

In Assignment 2 (A2) der Konstruktionsübung (KU) wird ein Spiel programmiert, welches lose vom Brettspiel [Magic Maze](https://de.wikipedia.org/wiki/Magic_Maze) inspiriert wurde. Achte bitte genau auf die Angabe, auch wenn du das Original
schon kennst, denn *OOP1 Maze* unterscheidet sich von Magic Maze auch in vielen Punkten.
Das Spielprinzip wird auf dieser Seite allgemein beschrieben.

Assignment 2 ist in zwei Milestones unterteilt. Wir empfehlen, zuerst Milestone 1 abzuschließen, bevor mit Milestone 2 begonnen wird, da Milestone 2 auf Milestone 1 aufbaut.

- [Milestone 1 - Grundstruktur und Ausgabe](description/Milestone_1.md)
- [Milestone 2 - Spiellogik](description/Milestone_2.md)

## Spezifikation

- nur geforderte Ausgaben
- Die zur Verfügung gestellten Klassen (`RoomInfoString` und `Random`) dürfen nicht verändert werden.
- Es muss eine Ableitungshierarchie (Klassen mit Vererbung) implementiert werden.
- Alle Bibliotheken der C++ Standard Library sind erlaubt.

### Abgabe

- Push auf das Gitlab-Repository des Teams **auf einen Branch namens `submission`**
- **Abzugeben bis: 04.06.2022 um 23:59 Uhr**
- Eine der Voraussetzungen für das positive Absolvieren der KU ist das Bestehen von **mindestens 50 % der öffentlichen Test Cases** von A2. (Es gibt jedoch die Möglichkeit einer [Ausbesserung](#ausbesserung).)

## Bewertung
- Bei Assignment 2 können 60 Punkte erreicht werden.
- Die Kriterien, nach welchen das Programm bewertet wird, können folgenden Dokumenten entnommen werden:
  - [Beurteilungsschema](https://tc.tugraz.at/main/mod/page/view.php?id=256924)
  - [Bewertung von Übungsbeispielen](https://tc.tugraz.at/main/mod/page/view.php?id=138898)
    - In diesem Assignment werden von jedem Teammitglied mindestens **zehn Commits** und **fünf Tage mit Commits** gefordert.
  - [Coding Standard](https://tc.tugraz.at/main/mod/page/view.php?id=260668)
- Ein Teil der Punkte kann in A2 nur dann erreicht werden, wenn bestimmte C++-Features verwendet werden. Um die Punkte für eines der folgenden Features zu erhalten, muss dieses in bestandenen Test Cases genutzt werden.
  - 5 % der Gesamtpunkte sind an die Verwendung von Smart Pointern gebunden.
  - 5 % der Gesamtpunkte sind an die Definition und Verwendung einer eigenen Exception-Klasse gebunden. 

## Optionen nach A2
### Ausbesserung

Auch wenn die benötigten 50 % der öffentlichen Test Cases am Ende der Abgabefrist noch nicht bestanden werden, bedeutet das noch nicht das Ausscheiden aus der KU. Es gibt die Möglichkeit einer **Ausbesserung**, welche anstelle von Assignment 3 gemacht werden kann. Diese Ausbesserung _ersetzt_ die bei A2 erreichten Punkte. Informationen dazu gab es im ersten Stream des Semesters am 3. März.

### Assignment 3

Da Assignment 3 (A3) auf Assignment 2 aufbauen wird, gibt es eine Teilnahmevoraussetzung für A3. Für eine Teilnahme an A3 sind zumindest **zwei Drittel aller Test Cases** in A2 zu bestehen.

### Ende der KU mit den in A1 und A2 erreichten Punkten
A3 ist ebenso wie die Ausbesserung optional. Wer sich mit den in A1 und A2 erreichten Punkten zufrieden gibt, muss nach A2 keine weitere Aufgabe machen.

## Spielaufbau

*OOP1 Maze* ist ein Single-Player-Spiel im Stil eines [Dungeon Crawlers](https://de.wikipedia.org/wiki/Dungeon_(Rollenspiele)). Die\*der Spieler\*in steuert dabei drei Charaktere durch ein Labyrinth, wobei der Spielplan durch die Bewegungen der Charaktere erst nach und nach sichtbar wird.

Im Labyrinth verteilt befinden sich Hindernisse und Bonusfelder, die bewältigt werden müssen bzw. spezielle Effekte auslösen.

Es gibt die folgenden Charaktere:
- Krieger\*in ('Fighter')
- Dieb\*in ('Thief')
- Wahrsager\*in ('Seer')

## Spielbeginn

Zu Beginn des Spiels ist nur ein Raum (der Startraum 'S') sichtbar, in dem sich alle Charaktere befinden. Der Rest des Spielplans ist unbekannt. Die Anordnung und Darstellung der Räume (sowie der einzelnen Felder, aus denen Räume bestehen können) wird in [Milestone 1](description/Milestone_1.md) beschrieben.

Die\*der Spieler\*in hat vier Kärtchen, auf denen jeweils eine Bewegungsrichtung (hinunter, hinauf, rechts, links) abgebildet ist. Diese werden gemischt und das oberste Kärtchen aufgedeckt.

## Spielablauf

Die\*der Spieler\*in führt nacheinander Spielzüge durch, um das Spielziel zu erreichen. Hier gibt es verschiedene Möglichkeiten.
Die\*der Spieler\*in kann:

1. einen Charakter bewegen
2. die Spezialfähigkeit eines Charakters einsetzen

### 1. Charakter bewegen

Ein Zug kann dafür verwendet werden, einen Charakter um ein beliebige Anzahl an Feldern in eine bestimmte Richtung zu bewegen. 
Es ist jedoch nicht immer möglich, die Charaktere in jede beliebige Richtung zu bewegen. Die\*der Spieler\*in darf nur Bewegungen in jene Richtung durchführen, deren Bewegungskärtchen gerade aufgedeckt ist. Dies kann sie\*er jedoch *beliebig oft* für *alle* Charaktere tun.  

Hierbei sind Hindernisse (z. B. Wände und Monster) zu beachten. Es gibt bestimmte Bonusfelder (z. B. Sanduhr, Monster, Kristallkugel, Schalter), die entweder automatisch eingesetzt werden, wenn ein Charakter seine Bewegung auf diesem Feld beendet, oder es dem Charakter ermöglichen, seine Spezialfähigkeit einzusetzen, wenn sie\*er sich auf einem benachbarten Feld befindet.

Kommt ein Charakter auf einem Durchgang zu einem noch unbekannten Nachbarraum zu stehen, wird dieser Raum automatisch aufgedeckt.

Wenn die\*der Spieler\*in keine Bewegungen in die derzeit erlaubte Richtung mehr durchführen will oder kann, fügt sie\*er das derzeitige Bewegungskärtchen an letzter Stelle in der Reihenfolge hinzu und deckt das nächste auf.

### 2. Spezialfähigkeit einsetzen

Jeder der Charaktere hat eine Spezialfähigkeit, die sie\*er unter bestimmten Umständen einsetzen kann.
- Krieger\*in ('Fighter') -> Monster bekämpfen ('fight')
- Dieb\*in ('Thief')      -> verschlossene Türe aufsperren ('unlock')
- Wahrsager\*in ('Seer')  -> unbekannten Raum aus Entfernung aufdecken ('scry')

Spezialfähigkeiten können jederzeit und beliebig oft eingesetzt werden, solange ihre Voraussetzungen erfüllt sind (siehe [Milestone 2](description/Milestone_2.md)).

## Spielende

Jeder der Charaktere muss einen ihm zugewiesenen Schalter erreichen. Wenn sich alle Charaktere gleichzeitig auf ihrem Schalter befinden, öffnen sich die geheimen Türen zur Schatzkammer. Alle Charaktere müssen sich nun zur Schatzkammer bewegen, um das Spiel zu gewinnen.


## Spielziel

Ziel des Spiels ist es, möglichst wenige neue Bewegungskärtchen aufzudecken, also die Schatzkammer möglichst effizient zu erreichen.
