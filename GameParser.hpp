#ifndef GAME_PARSER_HPP
#define GAME_PARSER_HPP

#include <memory>
#include "Room.hpp"
#include <vector>

#include "Game.hpp"

class GameParser
{
  void checkIfLetters(const std::string& rooms_row_string) const;
  void checkRowLength(const std::string& rooms_row_string) const;
  void containsOneStartingRoom(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms) const;
  void ifEveryRoomUnique(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms,
                         const std::shared_ptr<Room>& room) const;
  void ifRoomsFormRectangle(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms) const;

  GameParser() = default;
  public:
    static GameParser& getInstance() noexcept;
    void parseRooms(int argc, const char* const argv[]);
    void parseInput(std::vector<std::string>& container, MagicMaze::COMMANDS& command, bool& if_eof);
    void checkFirstParameter(std::string& command_as_string, MagicMaze::COMMANDS& command);
    void checkSizeOfInputParameters(const std::vector<std::string>& container, const MagicMaze::COMMANDS& command);

    ~GameParser() noexcept = default;
    GameParser(const GameParser&) = delete;
    GameParser operator=(const GameParser&) = delete;
};

#endif // GAME_PARSER_HPP