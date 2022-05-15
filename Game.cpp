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
#include "MagicTile.hpp"
#include "BasicTile.hpp"

std::string stringToUppercase(std::string& s);
bool checkSizeOfInputParameters(const std::vector<std::string>& container, const COMMANDS& command);

Game::Game()
 : characters_{ std::make_shared<Character>(CharacterType::FIGHTER),
                std::make_shared<Character>(CharacterType::THIEF),
                std::make_shared<Character>(CharacterType::SEER) },
  flips_number_{0},
  show_map_{true}
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

  std::shared_ptr<Tile> tile_helper{ room_map.at(1).at(1) };
  tile_helper->setCharacter(characters_.at(0));
  tile_helper->setAvailable(false);
  characters_.at(0)->setCurrentTile(tile_helper);

  tile_helper = room_map.at(1).at(3);
  tile_helper->setCharacter(characters_.at(1));
  tile_helper->setAvailable(false);
  characters_.at(1)->setCurrentTile(tile_helper);

  tile_helper = room_map.at(3).at(1);
  tile_helper->setCharacter(characters_.at(2));
  tile_helper->setAvailable(false);
  characters_.at(2)->setCurrentTile(tile_helper);
}

const int NUMBER_LINES_IN_ROOM{15};

void Game::printMap()
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
  placeCharacterOnStartingPosition();

  std::cout << "Welcome to the magical OOP1 Maze!!!" << std::endl;
  std::cout << "Card Flip Counter:   " << getFlipsNumber() << std::endl;
  printMap();
  std::cout << "Possible move: " << getPossibleMoveAsString() << std::endl;

  while(1)
  {
    std::string user_input = parseInput();
    std::istringstream ss_input{user_input};

    std::vector<std::string> container;
    std::string temp{};
    while(ss_input >> temp)
      container.push_back(temp);

    COMMANDS command = checkFirstParameter(container.front());
    if(!checkSizeOfInputParameters(container, command))
      command = COMMANDS::ERROR;
    try
    {
      switch (command)
      {
        case COMMANDS::HELP:
          break;
        case COMMANDS::QUIT:
          return;
        case COMMANDS::MAP:
          setMapActivity(!ifMapActivated());
          if (show_map_)
            break;
          continue;
        case COMMANDS::FLIP:
          flip();
          break;
        case COMMANDS::MOVE:
          move(container);
          break;
        case COMMANDS::UNLOCK:
          break;
        case COMMANDS::FIGHT:
          fightMonster();
          break;
        case COMMANDS::SCRY:
          scry(container);
          break;
        case COMMANDS::ERROR:
          continue;
      }
    }
    catch(std::string& e)
    {
      std::cout << e << std::endl;
      continue;
    }

    std::cout << "Card Flip Counter:" <<  std::setw(4) << getFlipsNumber() << std::endl;
    if (show_map_)
      printMap();
    std::cout << "Possible move: " << getPossibleMoveAsString() << std::endl;
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

  while(1)
  {
    std::cout << " > ";
    std::getline(std::cin, input);

    if(std::cin.eof())
      throw Exceptions::EndOfFile();

    if(input.find_first_not_of(" \t\n") != std::string::npos)
      break;
  }

  input = stringToUppercase(input);

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
    if(container.size() != 3 && container.size() != 4)
      return false;
  }
  else if(command == COMMANDS::SCRY)
  {
    if(container.size() != 3)
      return false;
  }

  return true;
}

std::shared_ptr<Character> Game::getCharacter(CharacterType type) const
{
  if (type == CharacterType::FIGHTER)
    return characters_.at(0);
  else if (type == CharacterType::THIEF)
    return characters_.at(1);
  else // SEER
    return characters_.at(2);
}


void Game::move(std::vector<std::string>& input)
{
  std::shared_ptr<Character> character_to_move;
  std::queue<std::shared_ptr<Tile>> tiles_on_the_way;
  std::shared_ptr<Tile> current_tile;
  int distance = 1;

  moveInputParsing(input, character_to_move, distance);
  std::shared_ptr<Tile> first_tile = character_to_move->getCurrentile().lock();
  getTilesOnTheWay(tiles_on_the_way, character_to_move, distance);

  std::shared_ptr<Room> current_room;

  while(!tiles_on_the_way.empty())
  {
    current_tile = tiles_on_the_way.front();
    tiles_on_the_way.pop();
    current_room = getRoomById(current_tile->getInsideRoomId());

    if (!tiles_on_the_way.size() && !current_room->isRevealed())
      throw character_to_move->getFullName() + ": \"My way is blocked!\"";

    else if(current_room->getNumOfMonsters() && character_to_move->getCharacterType() != CharacterType::FIGHTER)
      throw character_to_move->getFullName() + ": \"That room is too scary for me!\"";

    else if (tiles_on_the_way.size() && current_tile->ifPassable() && current_room->isRevealed())
      continue;

    else if (!tiles_on_the_way.size() && current_tile->ifAvailable() && current_room->isRevealed())
      stopCharacterOnTile(first_tile, current_room, current_tile, character_to_move);

    else if(!tiles_on_the_way.size() && current_tile->ifContainsCharacter())
      throw character_to_move->getFullName() + ": \"There is not enough space on that tile!\"";

    else
      throw character_to_move->getFullName() + ": \"My way is blocked!\"";
  }
}

void Game::stopCharacterOnTile(std::shared_ptr<Tile>& first_tile, std::shared_ptr<Room>& current_room,
                               std::shared_ptr<Tile>& current_tile, std::shared_ptr<Character>& moving_character)
{
  first_tile->setCharacter(nullptr);
  first_tile->setAvailable(true);
  current_tile->setCharacter(moving_character);
  current_tile->setAvailable(false);
  moving_character->setCurrentTile(current_tile);

  checkIfNewRoomsNeedToBeRevealed(current_tile, current_room);
  if (current_tile->getTileType() == TileType::HOURGLASS)
  {
    useHourglass(current_tile);
    current_tile->setCharacter(moving_character);
    moving_character->setCurrentTile(current_tile);
  }
  if (current_tile->getTileType() == TileType::FIGHTER_BUTTON &&
           moving_character->getCharacterType() == CharacterType::FIGHTER)
    moving_character->setOnButton(true);
  else if (current_tile->getTileType() == TileType::THIEF_BUTTON &&
           moving_character->getCharacterType() == CharacterType::THIEF)
    moving_character->setOnButton(true);
  else if (current_tile->getTileType() == TileType::SEER_BUTTON &&
           moving_character->getCharacterType() == CharacterType::SEER)
    moving_character->setOnButton(true);
  else if (moving_character->ifOnButton())
    moving_character->setOnButton(false);
}

void Game::useHourglass(std::shared_ptr<Tile>& tile)
{
  if (flips_number_ >= 5)
    flips_number_ -= 5;
  else
    flips_number_ = 0;
  MagicTile::magicUsed(tile);
}

void Game::moveInputParsing(std::vector<std::string>& input, std::shared_ptr<Character>& character_to_move,
                            int& distance)
{
  std::string character_str = input.at(1);

  if (character_str == "F")
    character_to_move = getCharacter(CharacterType::FIGHTER);
  else if (character_str == "S")
    character_to_move = getCharacter(CharacterType::SEER);
  else if (character_str == "T")
    character_to_move = getCharacter(CharacterType::THIEF);
  else
    throw std::string{"Who do you want to move?!?"};

  std::string direction_to_go{ input.at(2) };
  std::string possible_move_str{ getPossibleMoveAsString() };
  if (direction_to_go != "UP" && direction_to_go != "DOWN" && direction_to_go != "RIGHT" && direction_to_go != "LEFT")
    throw character_to_move->getFullName() + ": \"I don't understand where I should go!\"";

  else if (direction_to_go != stringToUppercase(possible_move_str))
    throw character_to_move->getFullName() + ": \"I can't go that way right now!\"";

  if (input.size() == 4)
  {
    try
    {
      distance = stoi(input.at(3));
    }
    catch(std::invalid_argument& e)
    {
      distance = -1;
    }
    if (distance <= 0)
      throw character_to_move->getFullName() + ": \"I don't understand how far I should go!\"";
  }
}

void Game::changeNextRowCol(int& next_row, int& next_col)
{
  switch(getCurrentDirection())
    {
      case DIRECTIONS_TYPES::UP:
        next_row -= 1;
        break;
      case DIRECTIONS_TYPES::DOWN:
        next_row += 1;
        break;
      case DIRECTIONS_TYPES::RIGHT:
        next_col += 1;
        break;
      case DIRECTIONS_TYPES::LEFT:
        next_col -= 1;
        break;
    }
}

void Game::getTilesOnTheWay(std::queue<std::shared_ptr<Tile>>& tiles_on_way,
                            const std::shared_ptr<Character>& character, const int& distance)
{
  std::shared_ptr<Tile> current_tile{ character->getCurrentile().lock() };
  std::shared_ptr<Room> current_room{ getRoomById(current_tile->getInsideRoomId()) };
  int next_row{ current_tile->getRow() };
  int next_col{ current_tile->getColumn() };

  for (int i = 0; i < distance; i++)
  {
    changeNextRowCol(next_row, next_col);

    if (next_row == 5) // switch row to down
    {
      if (current_room->getRow() + 1 == (int)rooms_.size())
        throw character->getFullName() + ": \"My way is blocked\"";
      next_row = 0;
      current_room = rooms_.at(current_room->getRow() + 1).at(current_room->getColumn());
    }
    else if (next_row == -1) // switch row to up
    {
      if (!current_room->getRow())
        throw character->getFullName() + ": \"My way is blocked\"";
      next_row = 4;
      current_room = rooms_.at(current_room->getRow() - 1).at(current_room->getColumn());
    }
    else if (next_col == 5) // switch column to right
    { 
      if (current_room->getColumn() + 1 == (int)rooms_.at(0).size())
        throw character->getFullName() + ": \"My way is blocked\"";
      next_col = 0;
      current_room = rooms_.at(current_room->getRow()).at(current_room->getColumn() + 1);
    }
    else if (next_col == -1) // switch column to left
    {
      if (!current_room->getColumn())
        throw character->getFullName() + ":  \"My way is blocked\"";
      next_col = 4;
      current_room = rooms_.at(current_room->getRow()).at(current_room->getColumn() - 1);
    }
    tiles_on_way.push(current_room->getRoomMap().at(next_row).at(next_col));
  }
}

void Game::checkIfNewRoomsNeedToBeRevealed(const std::shared_ptr<Tile>& current_tile,
                                           const std::shared_ptr<Room> current_room)
{
  const int map_rows{ (int)rooms_.size() };
  const int map_cols{ (int)rooms_.at(0).size() };
  if (current_tile->getRow() == 0)
  {
    if (current_room->getRow() - 1 >= 0 && !rooms_.at(current_room->getRow() - 1)
                                                  .at(current_room->getColumn())->isRevealed())
      rooms_.at(current_room->getRow() - 1).at(current_room->getColumn())->setRevealed(true);
  }
  else if (current_tile->getRow() == 4)
  {
    if (current_room->getRow() + 1 < map_rows && !rooms_.at(current_room->getRow() + 1)
                                                        .at(current_room->getColumn())->isRevealed())
      rooms_.at(current_room->getRow() + 1).at(current_room->getColumn())->setRevealed(true);
  }
  if (current_tile->getColumn() == 0)
  {
    if (current_room->getColumn() - 1 >= 0 && !rooms_.at(current_room->getRow())
                                                     .at(current_room->getColumn() - 1)->isRevealed())
      rooms_.at(current_room->getRow()).at(current_room->getColumn() - 1)->setRevealed(true);
  }
  else if (current_tile->getColumn() == 4)
  {
    if (current_room->getColumn() + 1 < map_cols && !rooms_.at(current_room->getRow())
                                                           .at(current_room->getColumn() + 1)->isRevealed())
      rooms_.at(current_room->getRow()).at(current_room->getColumn() + 1)->setRevealed(true);
  }
}


void Game::fightMonster()
{
  std::shared_ptr<Character> character = getCharacter(CharacterType::FIGHTER);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  int row = tile->getRow();
  int col = tile->getColumn();
  const char room_id = tile->getInsideRoomId();
  const int game_row = rooms_.size();
  const int game_col = rooms_[0].size();

  std::shared_ptr<Room> single_room = getRoomById(room_id);
  bool fight_with_monster = false;
  int num_of_fight_monsters = single_room->getNumOfMonsters();
  std::shared_ptr<Room> next_room = rooms_[single_room->getRow()][single_room->getColumn()];

  if(col != 4 && single_room->getRoomMap().at(row).at(col + 1)->getTileType() == TileType::MONSTER)
  {
    MagicTile::magicUsed(single_room->getRoomMap().at(row).at(col + 1));
    num_of_fight_monsters--;
    single_room->setNumOfMonsters(num_of_fight_monsters);
    fight_with_monster = true;
  }
  else if(col == 4 && game_col > (single_room->getColumn() + 1) && rooms_[single_room->getRow()][single_room->getColumn() + 1]
  ->getRoomMap().at(row).at(0)->getTileType() == TileType::MONSTER)
  {
    next_room = rooms_[single_room->getRow()][single_room->getColumn() + 1];
    next_room->setNumOfMonsters(next_room->getNumOfMonsters() - 1);
    MagicTile::magicUsed(next_room->getRoomMap().at(row).at(0));
    fight_with_monster = true;
  }
  if(col != 0 && single_room->getRoomMap().at(row).at(col - 1)->getTileType() == TileType::MONSTER)
  {
    MagicTile::magicUsed(single_room->getRoomMap().at(row).at(col - 1));
    num_of_fight_monsters--;
    single_room->setNumOfMonsters(num_of_fight_monsters);
    fight_with_monster = true;
  }
  else if(col == 0 && (single_room->getColumn() - 1) > 0 && rooms_[single_room->getRow()][single_room->getColumn() - 1]
  ->getRoomMap().at(row).at(4)->getTileType() == TileType::MONSTER)
  {
    next_room = rooms_[single_room->getRow()][single_room->getColumn() - 1];
    next_room->setNumOfMonsters(next_room->getNumOfMonsters() - 1);
    MagicTile::magicUsed(next_room->getRoomMap().at(row).at(4));
    fight_with_monster = true;
  }
  if(row != 4 && single_room->getRoomMap().at(row + 1).at(col)->getTileType() == TileType::MONSTER)
  {
    MagicTile::magicUsed(single_room->getRoomMap().at(row + 1).at(col));
    num_of_fight_monsters--;
    single_room->setNumOfMonsters(num_of_fight_monsters);
    fight_with_monster = true;
  }
  else if(row == 4 && (single_room->getRow() + 1) < game_row && rooms_[single_room->getRow() + 1][single_room->getColumn()]
  ->getRoomMap().at(0).at(col)->getTileType() == TileType::MONSTER)
  {
    next_room = rooms_[single_room->getRow() + 1][single_room->getColumn()];
    next_room->setNumOfMonsters(next_room->getNumOfMonsters() - 1);
    MagicTile::magicUsed(next_room->getRoomMap().at(0).at(col));
    fight_with_monster = true;
  }
  if(row != 0 && single_room->getRoomMap().at(row - 1).at(col)->getTileType() == TileType::MONSTER)
  {
    MagicTile::magicUsed(single_room->getRoomMap().at(row - 1).at(col));
    num_of_fight_monsters--;
    single_room->setNumOfMonsters(num_of_fight_monsters);
    fight_with_monster = true;
  }
  else if(row == 0 && (single_room->getRow() - 1) > 0 && rooms_[single_room->getRow() - 1][single_room->getColumn()]
  ->getRoomMap().at(4).at(col)->getTileType() == TileType::MONSTER)
  {
    next_room = rooms_[single_room->getRow() - 1][single_room->getColumn()];
    next_room->setNumOfMonsters(next_room->getNumOfMonsters() - 1);
    MagicTile::magicUsed(next_room->getRoomMap().at(4).at(col));
    fight_with_monster = true;
  }
  if(fight_with_monster == false)
  {
    throw character->getFullName() + ":  \"Nothing to fight here!\"";
  }

  single_room->setNumOfMonsters(num_of_fight_monsters);
}


DIRECTIONS_TYPES Game::checkDirection(std::string direction, std::shared_ptr<Character>& character)
{
  DIRECTIONS_TYPES direction_type;

  if(direction == "UP")
    direction_type = DIRECTIONS_TYPES::UP;
  else if(direction == "DOWN")
    direction_type = DIRECTIONS_TYPES::DOWN;
  else if(direction == "RIGHT")
    direction_type = DIRECTIONS_TYPES::RIGHT;
  else if(direction == "LEFT")
    direction_type = DIRECTIONS_TYPES::LEFT;
  else
     throw character->getFullName() + ":  \"I don't understand which room I should scry!\"";

  return direction_type;
}



void Game::scryInputParsing(std::vector<std::string>& input, std::shared_ptr<Room>& room_to_scry,
                            DIRECTIONS_TYPES& direction, std::shared_ptr<Character> character)
{
  std::string room_id = input.at(1);
  room_to_scry = getRoomById(room_id.at(0));
  const int game_row = rooms_.size();
  const int game_col = rooms_[0].size();

  if(room_to_scry == nullptr)
    throw character->getFullName() + ":  \"I don't understand which room I should scry!\"";

  direction = checkDirection(input.at(2), character);

  switch(direction)
  {
  case DIRECTIONS_TYPES::DOWN:
    if(room_to_scry->getRow() + 1 >= game_row)
      throw character->getFullName() + ":  \"There is no room I can reveal at this position!\"";
    else if(rooms_.at(room_to_scry->getRow() + 1).at(room_to_scry->getColumn())->isRevealed() == true)
      throw character->getFullName() + ":  \"We already know that room...\"";
  break;
  case DIRECTIONS_TYPES::RIGHT:
    if(room_to_scry->getColumn() + 1 >= game_col)
      throw character->getFullName() + ":  \"There is no room I can reveal at this position!\"";
    else if(rooms_.at(room_to_scry->getRow()).at(room_to_scry->getColumn() + 1)->isRevealed() == true)
      throw character->getFullName() + ":  \"We already know that room...\"";
    break;
  case DIRECTIONS_TYPES::LEFT:
    if(room_to_scry->getColumn() + 1 < 0)
      throw character->getFullName() + ":  \"There is no room I can reveal at this position!\"";
    else if(rooms_.at(room_to_scry->getRow()).at(room_to_scry->getColumn() - 1)->isRevealed() == true)
      throw character->getFullName() + ":  \"We already know that room...\"";
    break;
  case DIRECTIONS_TYPES::UP:
    if(room_to_scry->getRow() - 1 < 0)
      throw character->getFullName() + ":  \"There is no room I can reveal at this position!\"";
    else if(rooms_.at(room_to_scry->getRow() - 1).at(room_to_scry->getColumn())->isRevealed() == true)
      throw character->getFullName() + ":  \"We already know that room...\"";
    break;
  }

}


void Game::scry(std::vector<std::string>& input)
{
  std::shared_ptr<Character> character = getCharacter(CharacterType::SEER);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  int row = tile->getRow();
  int col = tile->getColumn();
  const char character_room_id = tile->getInsideRoomId();
  std::shared_ptr<Room> character_room = getRoomById(character_room_id);

  if(!(character_room->getRoomMap().at(row).at(col)->getTileType() == TileType::CRYSTAL_BALL))
    throw character->getFullName() + ":  \"I can't scry without my magic crystal ball!";

  DIRECTIONS_TYPES direction;
  std::shared_ptr<Room> room_to_scry;
  scryInputParsing(input, room_to_scry, direction, character);

  switch(direction)
  {
  case DIRECTIONS_TYPES::RIGHT:
    rooms_.at(room_to_scry->getRow()).at(room_to_scry->getColumn() + 1)->setRevealed(true);
    MagicTile::magicUsed(character_room->getRoomMap().at(row).at(col));
    break;
  case DIRECTIONS_TYPES::LEFT:
     rooms_.at(room_to_scry->getRow()).at(room_to_scry->getColumn() - 1)->setRevealed(true);
    MagicTile::magicUsed(character_room->getRoomMap().at(row).at(col));
    break;
  case DIRECTIONS_TYPES::DOWN:
    rooms_.at(room_to_scry->getRow() + 1).at(room_to_scry->getColumn())->setRevealed(true);
    MagicTile::magicUsed(character_room->getRoomMap().at(row).at(col));
    break;
  case DIRECTIONS_TYPES::UP:
    rooms_.at(room_to_scry->getRow() - 1).at(room_to_scry->getColumn())->setRevealed(true);
    MagicTile::magicUsed(character_room->getRoomMap().at(row).at(col));
    break;
  }

  character_room->getRoomMap().at(row).at(col)->setCharacter(character);
  character->setCurrentTile(character_room->getRoomMap().at(row).at(col));
}