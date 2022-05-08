#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <array>
#include <queue>

#include "Room.hpp"
#include "Character.hpp"


enum class DIRECTIONS_TYPES { UP, RIGHT, DOWN, LEFT };

class Game
{
  std::vector<std::vector<std::shared_ptr<Room>> > rooms_;
  std::array<std::shared_ptr<Character>, 3> characters_;
  std::queue<DIRECTIONS_TYPES> cards_;

  void checkIfLetters(const std::string& rooms_row_string) const;
  void checkRowLength(const std::string& rooms_row_string) const;
  void addRoom(const std::string& rooms_row_string);
  void containsSpecificRooms() const;
  void ifEveryRoomUnique(const std::shared_ptr<Room>& room) const;
  void ifRoomsFormRectangle() const;

  void shuffleCards();
  void flip();

  Game();

  public:
    static Game& getInstance();
    void parse(const int argc, const char* const argv[]);
    std::shared_ptr<Room> getRoomById(const char id);

    ~Game() = default;
    Game(const Game& copy) = delete;
    Game& operator=(const Game& game) = delete;

    void printRooms();
};

#endif // GAME_HPP
