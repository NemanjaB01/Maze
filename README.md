# Assignment 2 - OOP1 Maze

## Introduction

In Assignment 2 (A2) of the construction exercise (KU), a game loosely inspired by the board game [Magic Maze](https://de.wikipedia.org/wiki/Magic_Maze) is programmed. Please pay close attention to the information, even if you have the original
already know, because *OOP1 Maze* differs from Magic Maze in many ways.
The game principle is described in general on this page.

Assignment 2 is divided into two milestones. We recommend completing Milestone 1 first before starting Milestone 2 as Milestone 2 builds on Milestone 1.

- [Milestone 1 - Basic structure and output](description/Milestone_1.md)
- [Milestone 2 - Game Logic](description/Milestone_2.md)

## specification

- Only requested expenses
- The provided classes (`RoomInfoString` and `Random`) may not be changed.
- A derivation hierarchy (classes with inheritance) must be implemented.
- All libraries of the C++ Standard Library are allowed.

### levy

- Push to the team's gitlab repository **to a branch called `submission`**
- **To be handed in by: 04.06.2022 at 23:59**
- One of the requirements for passing the KU is passing **at least 50% of the public test cases** of A2. (However, there is a possibility of [repair](#repair).)

## Evaluation
- In Assignment 2, 60 points can be achieved.
- The criteria according to which the program is evaluated can be found in the following documents:
   - [Rating scheme](https://tc.tugraz.at/main/mod/page/view.php?id=256924)
   - [Evaluation of exercise examples](https://tc.tugraz.at/main/mod/page/view.php?id=138898)
     - In this assignment, each team member is required to have at least **ten commits** and **five days of commits**.
   - [Coding Standard](https://tc.tugraz.at/main/mod/page/view.php?id=260668)
- Some of the points can only be achieved in A2 if certain C++ features are used. In order to receive points for one of the following features, it must be used in passed test cases.
   - 5% of the total points are tied to the use of smart pointers.
   - 5% of the total points are tied to the definition and use of your own exception class.

## Options after A2
### Repair

Even if the required 50% of the public test cases are not passed by the end of the submission deadline, this does not mean that you have left the KU. There is an option of a **fix** that can be done in place of Assignment 3. This fix _replaces_ the points scored at A2. There was information about this in the first stream of the semester on March 3rd.

### Assignment 3

Since Assignment 3 (A3) will build on Assignment 2, there is a participation requirement for A3. To participate in A3, at least **two thirds of all test cases** in A2 must be passed.

### End of the KU with the points achieved in A1 and A2
A3 is optional, as is the touch-up. Anyone who is satisfied with the points achieved in A1 and A2 does not have to do any further tasks after A2.

## Setup

*OOP1 Maze* is a [Dungeon Crawler] style single player game. The player controls three characters through a labyrinth, whereby the game board only gradually becomes visible through the movements of the characters.

Distributed in the labyrinth are obstacles and bonus fields that have to be overcome or trigger special effects.

There are the following characters:
- Warrior ('Fighter')
- Thief\*in ('Thief')
- Fortune Teller ('Seer')

## Start of game

At the beginning of the game only one room (the starting room 'S') is visible, in which all the characters are located. The rest of the game plan is unknown. The arrangement and representation of the rooms (and the individual fields that rooms can consist of) are described in [Milestone 1](description/Milestone_1.md).

The player has four cards, each showing a direction of movement (down, up, right, left). These are shuffled and the top card is revealed.

## Gameplay

The player takes turns one after the other in order to reach the goal of the game. There are different ways.
The\*the player\*in can:

1. move a character
2. Use a character's special ability

### 1. Move character

A move can be used to move a character any number of spaces in a given direction.
However, it is not always possible to move the characters in any direction. The player may only move in the direction whose movement tile is currently face up. However, she\*he can do this *any* number of times for *all* characters.

Obstacles (e.g. walls and monsters) must be taken into account. There are certain bonus spaces (e.g. Hourglass, Monster, Crystal Ball, Switch) that either deploy automatically when a character ends their movement on that space, or allow the character to use their special ability when they\*he moves is on an adjacent square.

If a character comes to a passage to an unknown neighboring room, this room is automatically revealed.

When the player\*in no longer wants or can move in the currently allowed direction, he\*he adds the current movement tile at the end of the sequence and reveals the next one.

### 2. Use special ability

Each of the characters has a special ability that they can use under certain circumstances.
- Warrior\*in ('Fighter') -> Fight monsters ('fight')
- Thief\*in ('Thief') -> unlock locked door ('unlock')
- 'Seer' -> reveal unknown space from a distance ('scry')

Special abilities can be used at any time and any number of times as long as their requirements are met (see [Milestone 2](description/Milestone_2.md)).

## Playing

Each of the characters must reach a switch assigned to them. When all characters are on their switch at the same time, the secret doors to the treasury will open. All characters must now move to the treasury to win the game.


## Aim of the game

The aim of the game is to uncover as few new movement tiles as possible, i.e. to reach the treasury as efficiently as possible.
