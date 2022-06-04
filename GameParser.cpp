#include "GameParser.hpp"
#include <exception>
#include <string>
#include <sstream>
#include "Exceptions.hpp"
#include <algorithm>
#include "IO.hpp"


GameParser& GameParser::getInstance() noexcept
{
  static GameParser parser{};
  return parser;
}

void GameParser::parseRooms(const int argc, const char* const argv[])
{
  if (argc < 2)
    throw Exceptions::InvalidConfiguration();

  std::string helper;
  for (int iter{1}; iter < argc; iter++)
  {
    helper += argv[iter];
    helper += ',';
  }
  helper.erase(helper.size() - 1, 1);
  std::stringstream s_stream{ helper };

  std::string rooms_row_string;

  while(!s_stream.eof())
  {
    getline(s_stream, rooms_row_string, ',');
    checkRowLength(rooms_row_string);
    checkIfLetters(rooms_row_string);
    MagicMaze::Game::getInstance().addRoomsInRow(rooms_row_string);

    rooms_row_string.clear();
  }
  std::vector<std::vector<std::shared_ptr<Room>> > all_rooms = MagicMaze::Game::getInstance().getRooms();
  containsOneStartingRoom(all_rooms);

  for (const auto& room_row : all_rooms)
     for (const std::shared_ptr<Room>& single_room : room_row)
       ifEveryRoomUnique(all_rooms, single_room);

  unsigned long number_columns{all_rooms.at(0).size()};
  std::for_each(all_rooms.begin(), all_rooms.end(), [number_columns](const auto& room_row) -> void
  {
    if (room_row.size() != number_columns)
      throw Exceptions::InvalidConfiguration();
  });
}

void GameParser::checkIfLetters(const std::string& rooms_row_string) const
{
  for (const char& ch : rooms_row_string)
    if (ch < 'A' || ch > 'Z')
      throw Exceptions::InvalidConfiguration();
}

void GameParser::checkRowLength(const std::string& rooms_row_string) const
{
  if (rooms_row_string.length() > 5)
    throw Exceptions::InvalidConfiguration();
}

void GameParser::containsOneStartingRoom(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms) const
{
  int counter{0};
  for (const auto& room_row : all_rooms)
    for (const std::shared_ptr<Room>& r : room_row)
      if (r->getRoomId() == 'S')
        counter++;
  if (counter != 1)
    throw Exceptions::InvalidConfiguration();
}

void GameParser::ifEveryRoomUnique(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms, 
                                   const std::shared_ptr<Room>& current_room) const
{
  const char room_id{ current_room->getRoomId() };
  int counter{0};

  for (const auto& room_row : all_rooms)
    for (const std::shared_ptr<Room>& room : room_row)
    {
      if (room_id == room->getRoomId())
        counter += 1;
      if (counter >= 2)
        throw Exceptions::InvalidConfiguration();
    }
}


void GameParser::parseInput(std::vector<std::string>& container, MagicMaze::COMMANDS& command, bool& if_eof)
{
  if (!container.empty())
    container.clear();

  std::string input{};
  while(1)
  {
    std::cout << " > ";
    std::getline(std::cin, input);

    if(std::cin.eof())
    {
      if_eof = true;
      return;
    }
    if(input.find_first_not_of(" \t\n") != std::string::npos)
      break;
  }
  std::transform(input.begin(), input.end(), input.begin(), toupper);

  std::string temp{};
  std::istringstream ss_input{input};

  while(ss_input >> temp)
    container.push_back(temp);

  checkFirstParameter(container.front(), command);
  checkSizeOfInputParameters(container, command);
}

void GameParser::checkFirstParameter(const std::string& command_as_string, MagicMaze::COMMANDS& command) const
{
  if(command_as_string == "QUIT")
  {
    command = MagicMaze::COMMANDS::QUIT;
  }
  else if(command_as_string == "MAP")
  {
    command = MagicMaze::COMMANDS::MAP;
  }
  else if(command_as_string == "HELP")
  {
    command = MagicMaze::COMMANDS::HELP;
  }
  else if (command_as_string == "FLIP")
  {
    command = MagicMaze::COMMANDS::FLIP;
  }
  else if(command_as_string == "MOVE")
  {
    command = MagicMaze::COMMANDS::MOVE;
  }
  else if(command_as_string == "FIGHT")
  {
    command = MagicMaze::COMMANDS::FIGHT;
  }
  else if(command_as_string == "SCRY")
  {
    command = MagicMaze::COMMANDS::SCRY;
  }
  else if(command_as_string == "UNLOCK")
  {
    command = MagicMaze::COMMANDS::UNLOCK;
  }
  else
  {
    throw IO::NOT_KNOWN_COMMAND_MSG;
  }
}

void GameParser::checkSizeOfInputParameters(const std::vector<std::string>& container,
                                            const MagicMaze::COMMANDS& command) const
{
  if(command != MagicMaze::COMMANDS::MOVE && command != MagicMaze::COMMANDS::SCRY)
  {
    if(container.size() != 1)
      throw IO::NUMBER_PARAMETERS_MSG;
  }
  else if(command == MagicMaze::COMMANDS::MOVE)
  {
    if(container.size() != 3 && container.size() != 4)
      throw IO::NUMBER_PARAMETERS_MSG;
  }
  else if(command == MagicMaze::COMMANDS::SCRY)
  {
    if(container.size() != 3)
      throw IO::NUMBER_PARAMETERS_MSG;
  }
}

bool GameParser::checkDirection(const std::string& direction, MagicMaze::DIRECTIONS& direction_type) const noexcept
{
  if(direction == "UP")
    direction_type = MagicMaze::DIRECTIONS::UP;
  else if(direction == "DOWN")
    direction_type = MagicMaze::DIRECTIONS::DOWN;
  else if(direction == "RIGHT")
    direction_type = MagicMaze:: DIRECTIONS::RIGHT;
  else if(direction == "LEFT")
    direction_type = MagicMaze:: DIRECTIONS::LEFT;
  else
     return false;

  return true;
}