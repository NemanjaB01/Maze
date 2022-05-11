#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <array>
#include <queue>
#include <sstream>

#include "Room.hpp"
#include "Character.hpp"


enum class DIRECTIONS_TYPES { UP, RIGHT, DOWN, LEFT };

enum class COMMANDS { HELP, QUIT, MAP, FLIP, MOVE, UNLOCK, FIGHT, SCRY, ERROR };

class Game
{
  std::vector<std::vector<std::shared_ptr<Room>> > rooms_;
  std::array<std::shared_ptr<Character>, 3> characters_;
  std::queue<DIRECTIONS_TYPES> cards_;
  unsigned flips_number_;
  bool map_activated_;

  void checkIfLetters(const std::string& rooms_row_string) const;
  void checkRowLength(const std::string& rooms_row_string) const;
  void addRoom(const std::string& rooms_row_string);
  void containsOneStartingRoom() const;
  void ifEveryRoomUnique(const std::shared_ptr<Room>& room) const;
  void ifRoomsFormRectangle() const;

  void printHorizontalFrame() const;
  void shuffleCards();

  std::queue<DIRECTIONS_TYPES> getCards() const { return cards_; }
  void placeCharacterOnStartingPosition();

  Game();

  public:
    static Game& getInstance();
    void parse(const int argc, const char* const argv[]);
    std::shared_ptr<Room> getRoomById(const char id);
    void flip();
    DIRECTIONS_TYPES getCurrentDirection() const;
    unsigned getFlipsNumber() const { return flips_number_; }
    std::string getPossibleMoveAsString() const;

    void startTheGame();
    std::string parseInput();
    DIRECTIONS_TYPES current_direction() const;
    COMMANDS checkFirstParameter(const std::string& input) noexcept;
    bool checkNumberOfParameters() const;
    std::shared_ptr<Character> getCharacter(CharacterType type) const;
    bool ifMapActivated() const { return map_activated_; }
    void setMapActivity(bool map_activated) { map_activated_ = map_activated; }

    ~Game() = default;
    Game(const Game& copy) = delete;
    Game& operator=(const Game& game) = delete;

    void printMap();
    void fightMonster();

};

#endif // GAME_HPP
