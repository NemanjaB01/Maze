#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <cctype>
#include <iomanip>

#include "Game.hpp"
#include "Exceptions.hpp"
#include "RoomInfo.hpp"
#include "Random.hpp"

std::string stringToUppercase(std::string& s);
bool checkSizeOfInputParameters(const std::vector<std::string>& container, const COMMANDS& command);

Game::Game()
 : characters_{ std::make_shared<Character>(CharacterType::FIGHTER),
                std::make_shared<Character>(CharacterType::THIEF),
                std::make_shared<Character>(CharacterType::SEER) },
  flips_number_{0}
{
  shuffleCards();
}

Game& Game::getInstance()
{
  static Game game;
  return game;
}

void Game::parse(const int argc, const char* const argv[])
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
    addRoom(rooms_row_string);

    rooms_row_string.clear();
  }
  containsOneStartingRoom();

  for (const auto& room_row : rooms_)
     for (const std::shared_ptr<Room>& single_room : room_row)
       ifEveryRoomUnique(single_room);

   ifRoomsFormRectangle();
}

void Game::checkIfLetters(const std::string& rooms_row_string) const
{
  for (const char& ch : rooms_row_string)
    if (ch < 'A' || ch > 'Z')
      throw Exceptions::InvalidConfiguration();
}

void Game::checkRowLength(const std::string& rooms_row_string) const
{
  if (rooms_row_string.length() > 5)
    throw Exceptions::InvalidConfiguration();
}

void Game::addRoom(const std::string& rooms_row_string)
{
  std::vector<std::shared_ptr<Room>> rooms_row;

  static int row{0};
  int column{0};

  for (const char& room_id : rooms_row_string)
  {
    std::string room_info_string{ Oop::RoomInfo::getInstance().getRoomInfoString(room_id) };

    if (room_info_string.empty())
      throw Exceptions::InvalidConfiguration();

    std::shared_ptr<Room> new_room = std::make_shared<Room>(room_id, room_info_string, row, column);

   if(room_id == 'S')
      new_room->setRevealed(true);

    rooms_row.push_back(new_room);
    column++;
  }

  rooms_.push_back(rooms_row);
  row++;

}

void Game::containsOneStartingRoom() const
{
  int counter{0};
  for (const auto& room : rooms_)
    for (const std::shared_ptr<Room>& r : room)
      if (r->getRoomId() == 'S')
        counter++;
  if (counter != 1)
    throw Exceptions::InvalidConfiguration();
}

void Game::ifEveryRoomUnique(const std::shared_ptr<Room>& current_room) const
{
  const char room_id{ current_room->getRoomId() };
  int counter{0};

  for (const auto& room_row : rooms_)
    for (const std::shared_ptr<Room>& room : room_row)
    {
      if (room_id == room->getRoomId())
        counter += 1;
      if (counter >= 2)
        throw Exceptions::InvalidConfiguration();
    }
}

void Game::ifRoomsFormRectangle() const
{
  const unsigned long number_columns{ rooms_.at(0).size() };

  for (const std::vector<std::shared_ptr<Room>>& room_row : rooms_)
    if (room_row.size() != number_columns)
      throw Exceptions::InvalidConfiguration();
}

std::shared_ptr<Room> Game::getRoomById(const char id)
{
  for (auto& rooms_row : rooms_)
    for (std::shared_ptr<Room>& single_room : rooms_row)
      if (id == single_room->getRoomId())
        return single_room;

  return nullptr;
}


void Game::shuffleCards()
{
  std::vector<DIRECTIONS_TYPES> cards_dir{DIRECTIONS_TYPES::UP, DIRECTIONS_TYPES::RIGHT,
                                          DIRECTIONS_TYPES::DOWN, DIRECTIONS_TYPES::LEFT};
  Oop::Random &r = Oop::Random::getInstance();

  for (unsigned card_left = 4; card_left > 0; card_left--)
  {
    size_t random_card = r.getRandomCard(card_left);
    cards_.push(cards_dir.at(random_card - 1));
    cards_dir.erase(cards_dir.begin() + random_card - 1);
  }
}


void Game::flip()
{
  cards_.push(cards_.front());
  cards_.pop();

  flips_number_++;
}

DIRECTIONS_TYPES Game::getCurrentDirection() const
{
  return cards_.front();
}

void Game::placeCharacterOnStartingPosition()
{
  std::shared_ptr<Room> starting_room = getRoomById('S');
  auto room_map = starting_room->getRoomMap();
  room_map.at(1).at(1)->setCharacter(characters_.at(0));
  room_map.at(1).at(3)->setCharacter(characters_.at(1));
  room_map.at(3).at(1)->setCharacter(characters_.at(2));
}

const int NUMBER_LINES_IN_ROOM{15};

void Game::printRooms()
{
  int current_row_in_tile = 1;
  for(const auto& rooms_in_row : rooms_)
  {
    int current_row_in_room{0};
    for(int line{0}; line < NUMBER_LINES_IN_ROOM; line++)
    {
      if(!line)
          printHorizontalFrame();
      for(const auto& single_room : rooms_in_row)
      {
        std::cout << "\u2551";
        const auto row_in_room = single_room->getRoomMap().at(current_row_in_room);
        for(const auto& tile : row_in_room)
        {
          std::cout<<tile->getLineOfTile(current_row_in_tile, single_room->isRevealed());
        }
      }
      if(++current_row_in_tile >= 4)
      {
        current_row_in_tile = 1;
        current_row_in_room++;
      }
      std::cout << "\u2551" << std::endl;
    }
  }
  printHorizontalFrame();
}

const std::string FRAME = "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
                          "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
                          "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550";

void Game::printHorizontalFrame() const
{
  std::size_t number_rooms_in_row{ rooms_.at(0).size() };
  for (std::size_t i{0}; i < number_rooms_in_row; i++)
    std::cout << "\u256C" << FRAME;
  std::cout << "\u256C" << std::endl;
}

std::string Game::getPossibleMoveAsString() const
{
  switch (getCurrentDirection())
  {
  case DIRECTIONS_TYPES::UP:
    return "up";
  case DIRECTIONS_TYPES::DOWN:
    return "down";
  case DIRECTIONS_TYPES::RIGHT:
    return "right";
  case DIRECTIONS_TYPES::LEFT:
    return "left";
  }
  return "";
}

void Game::startTheGame()
{
  std::cout << "Welcome to the magical OOP1 Maze!!!" << std::endl;

  while(1)
  {
    std::cout << "Card Flip Counter:   " << getFlipsNumber() << std::endl;
    placeCharacterOnStartingPosition();
    printRooms();
    std::cout << "Possible move: " << getPossibleMoveAsString() << std::endl;

    std::string user_input = parseInput();
    std::string input = stringToUppercase(user_input);
    std::istringstream ss_input{input};

    std::vector<std::string> container;
    std::string temp{};
    for(unsigned index{0}; ss_input >> temp; index++)
      container.push_back(temp);

    COMMANDS command = checkFirstParameter(container.front());
    if(!checkSizeOfInputParameters(container, command))
      continue;

    switch (command)
    {
    case COMMANDS::HELP:
      break;
    case COMMANDS::QUIT:
      break;
    case COMMANDS::MAP:
      break;
    case COMMANDS::FLIP:
      break;
    case COMMANDS::MOVE:
      break;
    case COMMANDS::UNLOCK:
      break;
    case COMMANDS::FIGHT:
      break;
    case COMMANDS::SCRY:
      break;
    case COMMANDS::ERROR:
      break;
    }
  }
}

std::string stringToUppercase(std::string& s)
{
  for(auto& el : s)
  {
    if(isalpha(el) && islower(el))
      el = toupper(el);
  }

  return s;
}

std::string Game::parseInput()
{
  std::string input{};

  while(input.find_first_not_of(" \t\n") == std::string::npos)
  {
    std::cout << " > ";
    std::getline(std::cin, input);
  }

  if(std::cin.eof())
    throw Exceptions::EndOfFile();

  return input;
}

COMMANDS Game::checkFirstParameter(const std::string& input) noexcept
{
  COMMANDS command;

  if(input == "QUIT")
  {
    command = COMMANDS::QUIT;
  }
  else if(input == "MAP")
  {
    command = COMMANDS::MAP;
  }
  else if(input == "HELP")
  {
    command = COMMANDS::HELP;
  }
  else if (input == "FLIP")
  {
    command = COMMANDS::FLIP;
  }
  else if(input == "MOVE")
  {
    command = COMMANDS::MOVE;
  }
  else if(input == "FIGHT")
  {
    command = COMMANDS::FIGHT;
  }
  else if(input == "SCRY")
  {
    command = COMMANDS::SCRY;
  }
  else if(input == "UNLOCK")
  {
    command = COMMANDS::UNLOCK;
  }
  else
  {
    command = COMMANDS::ERROR;
  }

  return command;
}

bool checkSizeOfInputParameters(const std::vector<std::string>& container, const COMMANDS& command)
{
  if(command != COMMANDS::MOVE && command != COMMANDS::SCRY)
  {
    if(container.size() != 1)
      return false;
  }
  else if(command == COMMANDS::MOVE)
  {
    if(container.size() != 4)
      return false;
  }
  else if(command == COMMANDS::SCRY)
  {
    if(container.size() != 3)
      return false;
  }

  return true;
}