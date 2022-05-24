#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <cctype>
#include <iomanip>
#include <queue>
#include <algorithm>

#include "Game.hpp"
#include "Exceptions.hpp"
#include "RoomInfo.hpp"
#include "Random.hpp"
#include "MagicTile.hpp"
#include "BasicTile.hpp"
#include "GameParser.hpp"
#include "IO.hpp"

const int CHARACTERS_NUMBER = 3;

MagicMaze::Game::Game()
 : characters_{ std::make_shared<Character>(CharacterType::FIGHTER),
                std::make_shared<Character>(CharacterType::THIEF),
                std::make_shared<Character>(CharacterType::SEER) },
  flips_number_{0},
  show_map_{true}
{
  shuffleCards();
}

MagicMaze::Game& MagicMaze::Game::getInstance() noexcept
{
  static Game game;
  return game;
}

std::ostream& MagicMaze::operator<<(std::ostream& out, const Game& game)
{
  out << "Card Flip Counter:" << std::setw(4) << game.getFlipsNumber() << std::endl;
  if (game.ifMapActivated())
    game.printMap();
  out << "Possible move: " << game.getPossibleMoveAsString();

  return out;
}

void MagicMaze::Game::addRoom(const std::string& rooms_row_string)
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
      new_room->reveal();

    rooms_row.push_back(new_room);
    column++;
  }

  rooms_.push_back(rooms_row);
  row++;

}

std::shared_ptr<Room> MagicMaze::Game::getRoomById(const char id) const
{
  for (const auto& rooms_row : rooms_)
    for (const std::shared_ptr<Room>& single_room : rooms_row)
      if (id == single_room->getRoomId())
        return single_room;

  return nullptr;
}


void MagicMaze::Game::shuffleCards()
{
  std::vector<DIRECTIONS> cards_dir{DIRECTIONS::UP, DIRECTIONS::RIGHT,
                                          DIRECTIONS::DOWN, DIRECTIONS::LEFT};
  Oop::Random &r = Oop::Random::getInstance();

  for (unsigned card_left = 4; card_left > 0; card_left--)
  {
    size_t random_card = r.getRandomCard(card_left);
    cards_.push(cards_dir.at(random_card - 1));
    cards_dir.erase(cards_dir.begin() + random_card - 1);
  }
}

void MagicMaze::Game::flip()
{
  cards_.push(cards_.front());
  cards_.pop();

  flips_number_++;
}

void MagicMaze::Game::placeCharactersOnStartingPositions()
{
  std::shared_ptr<Room> starting_room = getRoomById('S');
  auto room_map = starting_room->getRoomMap();

  std::shared_ptr<Tile> tile_helper{ room_map.at(1).at(1) };
  tile_helper->setCharacter(characters_.at(0));
  characters_.at(0)->setCurrentTile(tile_helper);

  tile_helper = room_map.at(1).at(3);
  tile_helper->setCharacter(characters_.at(1));
  characters_.at(1)->setCurrentTile(tile_helper);

  tile_helper = room_map.at(3).at(1);
  tile_helper->setCharacter(characters_.at(2));
  characters_.at(2)->setCurrentTile(tile_helper);
}

void MagicMaze::Game::printMap() const
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
          std::cout<< tile->getLineOfTile(current_row_in_tile, single_room->isRevealed());
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

void MagicMaze::Game::printHorizontalFrame() const
{
  std::size_t number_rooms_in_row{ rooms_.at(0).size() };
  for (std::size_t i{0}; i < number_rooms_in_row; i++)
    std::cout << "\u256C" << IO::FRAME;
  std::cout << "\u256C" << std::endl;
}

std::string MagicMaze::Game::getPossibleMoveAsString() const
{
  switch (getCurrentDirection())
  {
  case DIRECTIONS::UP:
    return "up";
  case DIRECTIONS::DOWN:
    return "down";
  case DIRECTIONS::RIGHT:
    return "right";
  case DIRECTIONS::LEFT:
    return "left";
  }
  return "";
}

void MagicMaze::Game::prepareGame()
{
  placeCharactersOnStartingPositions();
  std::cout << "Welcome to the magical OOP1 Maze!!!" << std::endl;
  std::cout << *this << std::endl;
}

void MagicMaze::Game::run()
{
  prepareGame();
  bool if_eof{false};
  COMMANDS command;
  std::vector<std::string> container{};
  while(1)
  {
    try
    {
      GameParser::getInstance().parseInput(container, command, if_eof);
      if (if_eof)
        return;
      switch (command)
      {
        case COMMANDS::HELP:
          help();
          continue;
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
          unlock();
          break;
        case COMMANDS::FIGHT:
          fightMonster();
          break;
        case COMMANDS::SCRY:
          scry(container);
          break;
        default:
          break;
      }
    }
    catch(std::string& e)
    {
      std::cout << e << std::endl;
      continue;
    }
    std::cout << *this << std::endl;
    if(endOfGame())
    {
      std::cout << "You win, congratulations! It took you "<< getFlipsNumber() <<" card flips to find the treasure.\n";
      break;
    }
  }
}

std::shared_ptr<Character> MagicMaze::Game::getCharacter(CharacterType type) const
{
  if (type == CharacterType::FIGHTER)
    return characters_.at(0);
  else if (type == CharacterType::THIEF)
    return characters_.at(1);
  else // SEER
    return characters_.at(2);
}


void MagicMaze::Game::move(std::vector<std::string>& input)
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

void MagicMaze::Game::stopCharacterOnTile(std::shared_ptr<Tile>& first_tile, std::shared_ptr<Room>& current_room,
                               std::shared_ptr<Tile>& current_tile, std::shared_ptr<Character>& moving_character)
{
  first_tile->setCharacter(nullptr);
  current_tile->setCharacter(moving_character);
  moving_character->setCurrentTile(current_tile);

  checkIfNewRoomsNeedToBeRevealed(current_tile, current_room);
  if (current_tile->getTileType() == TileType::HOURGLASS)
  {
    useHourglass(current_tile, current_room, moving_character);
  }
  ifCharacterStoppedOnButton(current_tile, moving_character);
  if(checkIfAllCharactersOnButton())
  {
    setAllButtonsToPassage();
    removeAllSecretDoors();
  }

  if(current_tile->getTileType() == TileType::LOOT)
    moving_character->setOnLoot(true);
  else if(moving_character->isOnLoot())
    moving_character->setOnLoot(false);
}

void MagicMaze::Game::ifCharacterStoppedOnButton(const std::shared_ptr<Tile>& tile,
                                                 const std::shared_ptr<Character>& character)
{
  if (tile->getTileType() == TileType::FIGHTER_BUTTON && character->getCharacterType() == CharacterType::FIGHTER)
    character->setOnButton(true);

  else if (tile->getTileType() == TileType::THIEF_BUTTON && character->getCharacterType() == CharacterType::THIEF)
    character->setOnButton(true);

  else if (tile->getTileType() == TileType::SEER_BUTTON && character->getCharacterType() == CharacterType::SEER)
    character->setOnButton(true);

  else if (character->ifOnButton())
    character->setOnButton(false);
}

void MagicMaze::Game::useHourglass(std::shared_ptr<Tile>& tile, std::shared_ptr<Room>& room,
                                   std::shared_ptr<Character>& character)
{
  if (flips_number_ >= 5)
    flips_number_ -= 5;
  else
    flips_number_ = 0;
  std::shared_ptr<MagicTile> hourglass = std::dynamic_pointer_cast<MagicTile>(tile);
  if (hourglass && hourglass->magicUsed())
    room->setTileToPassage(hourglass->getRow(), hourglass->getColumn(), character);
}

void MagicMaze::Game::moveInputParsing(std::vector<std::string>& input, std::shared_ptr<Character>& character_to_move,
                                       int& distance)
{
  CharacterType character_type = Character::getCharacterTypeFromChar(input.at(1));
  if (character_type == CharacterType::NONE)
    throw std::string{"Who do you want to move?!?"};

  character_to_move = getCharacter(character_type);

  std::string direction_upper{input.at(2)};
  std::transform(direction_upper.begin(), direction_upper.end(), direction_upper.begin(), toupper);

  MagicMaze::DIRECTIONS direction;
  if (!checkDirection(direction_upper, direction))
    throw character_to_move->getFullName() + ": \"I don't understand where I should go!\"";

  else if (direction != getCurrentDirection())
    throw character_to_move->getFullName() + ": \"I can't go that way right now!\"";

  if (input.size() == 4)
  {
    try
    {
      size_t index{0};
      distance = stoi(input.at(3), &index);
      if (index != input.at(3).length() || distance <= 0)
        throw std::invalid_argument("");
    }
    catch(std::invalid_argument& e)
    {
      throw character_to_move->getFullName() + ": \"I don't understand how far I should go!\"";
    }
  }
}

void MagicMaze::Game::changeNextRowCol(int& next_row, int& next_col, const MagicMaze::DIRECTIONS& dir) const
{
  switch(dir)
    {
      case DIRECTIONS::UP:
        next_row -= 1;
        break;
      case DIRECTIONS::DOWN:
        next_row += 1;
        break;
      case DIRECTIONS::RIGHT:
        next_col += 1;
        break;
      case DIRECTIONS::LEFT:
        next_col -= 1;
        break;
    }
}

void MagicMaze::Game::getTilesOnTheWay(std::queue<std::shared_ptr<Tile>>& tiles_on_way,
                            const std::shared_ptr<Character>& character, const int& distance)
{
  std::shared_ptr<Tile> current_tile{ character->getCurrentile().lock() };
  std::shared_ptr<Room> current_room{ getRoomById( current_tile->getInsideRoomId()) };
  int next_row{ current_tile->getRow() };
  int next_col{ current_tile->getColumn() };

  for (int i = 0; i < distance; i++)
  {
    changeNextRowCol(next_row, next_col, getCurrentDirection());
    try
    {
      if (next_row == 5)
      {
        next_row = 0;
        current_room = rooms_.at(current_room->getRow() + 1).at(current_room->getColumn());
      }
      else if (next_row == -1)
      {
        next_row = 4;
        current_room = rooms_.at(current_room->getRow() - 1).at(current_room->getColumn());
      }
      else if (next_col == 5)
      {
        next_col = 0;
        current_room = rooms_.at(current_room->getRow()).at(current_room->getColumn() + 1);
      }
      else if (next_col == -1)
      {
        next_col = 4;
        current_room = rooms_.at(current_room->getRow()).at(current_room->getColumn() - 1);
      }
      tiles_on_way.push(current_room->getRoomMap().at(next_row).at(next_col));
    }
    catch(std::out_of_range& e)
    {
      throw character->getFullName() + ": \"My way is blocked\"";
    }
  }
}

void MagicMaze::Game::checkIfNewRoomsNeedToBeRevealed(const std::shared_ptr<Tile>& current_tile,
                                                      const std::shared_ptr<Room> current_room)
{
  const int map_rows{ static_cast<int>(rooms_.size()) };
  const int map_cols{ static_cast<int>(rooms_.at(0).size()) };
  if (current_tile->getRow() == 0)
  {
    if (current_room->getRow() - 1 >= 0 && !rooms_.at(current_room->getRow() - 1)
                                                  .at(current_room->getColumn())->isRevealed())
      rooms_.at(current_room->getRow() - 1).at(current_room->getColumn())->reveal();
  }
  else if (current_tile->getRow() == 4)
  {
    if (current_room->getRow() + 1 < map_rows && !rooms_.at(current_room->getRow() + 1)
                                                        .at(current_room->getColumn())->isRevealed())
      rooms_.at(current_room->getRow() + 1).at(current_room->getColumn())->reveal();
  }
  if (current_tile->getColumn() == 0)
  {
    if (current_room->getColumn() - 1 >= 0 && !rooms_.at(current_room->getRow())
                                                     .at(current_room->getColumn() - 1)->isRevealed())
      rooms_.at(current_room->getRow()).at(current_room->getColumn() - 1)->reveal();
  }
  else if (current_tile->getColumn() == 4)
  {
    if (current_room->getColumn() + 1 < map_cols && !rooms_.at(current_room->getRow())
                                                           .at(current_room->getColumn() + 1)->isRevealed())
      rooms_.at(current_room->getRow()).at(current_room->getColumn() + 1)->reveal();
  }
}

void MagicMaze::Game::unlock()
{
  std::queue<std::shared_ptr<Tile>> doors;
  std::shared_ptr<Character> character = getCharacter(CharacterType::THIEF);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  checkCorrespondingTiles(TileType::HORIZONTAL_DOOR, doors, tile);
  checkCorrespondingTiles(TileType::VERTICAL_DOOR, doors, tile);

  if(doors.empty())
    throw character->getFullName() + ": \"Nothing to unlock here!\"";

  while(!doors.empty())
  {
    std::shared_ptr<MagicTile> door =  std::dynamic_pointer_cast<MagicTile>(doors.front());
    if(door && door->magicUsed())
      getRoomById(door->getInsideRoomId())->setTileToPassage(door->getRow(), door->getColumn(), nullptr);
    doors.pop();
  }
}

std::shared_ptr<Room> MagicMaze::Game::getNeighborsRoom(int game_row, int game_column)
{
  std::shared_ptr<Room> neighbour_room;

  try
  {
    neighbour_room = rooms_.at(game_row).at(game_column);
  }
  catch(const std::out_of_range&)
  {
    return nullptr;
  }

  return neighbour_room;
}

void MagicMaze::Game::checkNeighborsTile(const TileType tile_type, const int row, const int column,
  std::queue<std::shared_ptr<Tile>>& container, std::shared_ptr<Room> room)
{
  if(room->getRoomMap().at(row).at(column)->getTileType() == tile_type)
    container.push(room->getRoomMap().at(row).at(column));
}

void MagicMaze::Game::checkTileType(const TileType tile_type, const int row, const int column,
  std::queue<std::shared_ptr<Tile>>& container, std::shared_ptr<Room> current_room, const int index)
{
  try
  {
    checkNeighborsTile(tile_type, row, column, container, current_room);
  }
  catch(const std::out_of_range& e)
  {
    DIRECTIONS direction = static_cast<DIRECTIONS>(index);
    std::shared_ptr<Room> neighbour_room;

    switch(direction)
    {
      case DIRECTIONS::UP:
        neighbour_room = getNeighborsRoom(current_room->getRow() - 1, current_room->getColumn());
        if(neighbour_room)
          checkNeighborsTile(tile_type, 4, column, container, neighbour_room);
        break;
      case DIRECTIONS::RIGHT:
         neighbour_room = getNeighborsRoom(current_room->getRow(), current_room->getColumn() + 1);
         if(neighbour_room)
          checkNeighborsTile(tile_type, row, 0, container, neighbour_room);
        break;
      case DIRECTIONS::DOWN:
        neighbour_room = getNeighborsRoom(current_room->getRow() + 1, current_room->getColumn());
        if(neighbour_room)
          checkNeighborsTile(tile_type, 0, column, container, neighbour_room);
        break;
      case DIRECTIONS::LEFT:
        neighbour_room = getNeighborsRoom(current_room->getRow(), current_room->getColumn() - 1);
        if(neighbour_room)
          checkNeighborsTile(tile_type, row, 4, container, neighbour_room);
        break;
    }
  }
}

void MagicMaze::Game::checkCorrespondingTiles(const TileType tile_type, std::queue<std::shared_ptr<Tile>>& container,
       std::shared_ptr<Tile> current_tile)
{
  const int row = current_tile->getRow();
  const int column = current_tile->getColumn();
  const char room_id = current_tile->getInsideRoomId();
  std::shared_ptr<Room> current_room = getRoomById(room_id);
  std::array<int, 4> row_values { -1, 0,  1,  0}; // UP  RIGHT  DOWN  LEFT
  std::array<int, 4> col_values {  0, 1,  0, -1}; //

  for(unsigned n{0}; n < 4; n++)
  {
    checkTileType(tile_type, row + row_values.at(n), column + col_values.at(n), container, current_room, n);
  }
}


void MagicMaze::Game::fightMonster()
{
  std::queue<std::shared_ptr<Tile>> monsters;
  std::shared_ptr<Character> character = getCharacter(CharacterType::FIGHTER);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  checkCorrespondingTiles(TileType::MONSTER, monsters, tile);

  if(monsters.empty())
    throw std::string{character->getFullName() +  ": \"Nothing to fight here!\""};

  while(!monsters.empty())
  {
    std::shared_ptr<MagicTile> monster = std::dynamic_pointer_cast<MagicTile>(monsters.front());
    if(monster && monster->magicUsed())
    {
      auto monster_room = getRoomById(monsters.front()->getInsideRoomId());
      monster_room->setTileToPassage(monster->getRow(), monster->getColumn(), nullptr);
      monster_room->decreaseNumMonsters();
    }
    monsters.pop();
  }
}


bool MagicMaze::Game::checkDirection(const std::string& direction, MagicMaze::DIRECTIONS& direction_type) const noexcept
{
  if(direction == "UP")
    direction_type = DIRECTIONS::UP;
  else if(direction == "DOWN")
    direction_type = DIRECTIONS::DOWN;
  else if(direction == "RIGHT")
    direction_type = DIRECTIONS::RIGHT;
  else if(direction == "LEFT")
    direction_type = DIRECTIONS::LEFT;
  else
     return false;

  return true;
}

void MagicMaze::Game::scryInputParsing(std::vector<std::string>& input, std::shared_ptr<Room>& room_to_scry,
                                       std::shared_ptr<Character> character) const
{
  std::string helper = input.at(1);
  std::transform(helper.begin(), helper.end(), helper.begin(), toupper);

  std::shared_ptr<Room> scry_from = getRoomById(helper.at(0));
  if(scry_from == nullptr)
    throw character->getFullName() + ": \"I don't understand which room I should scry!\"";

  MagicMaze::DIRECTIONS direction;

  helper.clear();
  helper = input.at(2);
  std::transform(helper.begin(), helper.end(), helper.begin(), toupper);
  if (!checkDirection(helper, direction))
    throw character->getFullName() + ": \"I don't understand which room I should scry!\"";

  try
  {
    int next_row{scry_from->getRow()};
    int next_col{scry_from->getColumn()};
    changeNextRowCol(next_row, next_col, direction);
    room_to_scry = rooms_.at(next_row).at(next_col);
    if(room_to_scry->isRevealed())
      throw character->getFullName() + ": \"We already know that room...\"";
  }
  catch(std::out_of_range& e)
  {
    throw character->getFullName() + ": \"There is no room I can reveal at this position!\"";
  }
}

void MagicMaze::Game::scry(std::vector<std::string>& input)
{
  std::shared_ptr<Character> character = getCharacter(CharacterType::SEER);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  const int row = tile->getRow();
  const int col = tile->getColumn();

  std::shared_ptr<Room> character_room = getRoomById(tile->getInsideRoomId());

  if(!(character_room->getRoomMap().at(row).at(col)->getTileType() == TileType::CRYSTAL_BALL))
    throw character->getFullName() + ": \"I can't scry without my magic crystal ball!\"";

  std::shared_ptr<Room> room_to_scry;
  scryInputParsing(input, room_to_scry, character);

  room_to_scry->reveal();

  auto crystal_ball = std::dynamic_pointer_cast<MagicTile>(character_room->getRoomMap().at(row).at(col));
  if (crystal_ball && crystal_ball->magicUsed())
  {
    character_room->setTileToPassage(crystal_ball->getRow(), crystal_ball->getColumn(), character);
  }
}

bool MagicMaze::Game::checkIfAllCharactersOnButton() const
{
  for(int index{0}; index < CHARACTERS_NUMBER; index++)
  {
    if(!characters_.at(index)->ifOnButton())
      return false;
  }

  return true;
}

void MagicMaze::Game::removeAllSecretDoors()
{
  for(std::vector<std::shared_ptr<Room>>& rooms_row : rooms_)
    for(std::shared_ptr<Room>& single_room : rooms_row)
    {
      auto secret_doors = single_room->getSecretDoors();
      while(!secret_doors.empty())
      {
        auto secret_door = secret_doors.back();
        if (secret_door->magicUsed())
          single_room->setTileToPassage(secret_door->getRow(), secret_door->getColumn(), nullptr);
        secret_doors.pop_back();
      }
    }
}

void MagicMaze::Game::setAllButtonsToPassage()
{
  std::shared_ptr<MagicTile> button;
  for(int index{0}; index < CHARACTERS_NUMBER; index++)
  {
    button = std::dynamic_pointer_cast<MagicTile>(characters_.at(index)->getCurrentile().lock());
    if (button && button->magicUsed())
    {
      std::shared_ptr<Room> room = getRoomById(button->getInsideRoomId());
      room->setTileToPassage(button->getRow(), button->getColumn(), characters_.at(index));
    }
  }
}

bool MagicMaze::Game::endOfGame()
{
  std::shared_ptr<Character> thief = getCharacter(CharacterType::THIEF);
  std::shared_ptr<Character> fighter = getCharacter(CharacterType::FIGHTER);
  std::shared_ptr<Character> seer = getCharacter(CharacterType::SEER);

  if((thief->isOnLoot() == true) && (fighter->isOnLoot() == true) && (seer->isOnLoot() == true))
    return true;

  return false;
}


void MagicMaze::Game::help()
{
  std::cout << "\nCommands:\n - help\n    Prints this help text.\n\n"
               " - quit\n    Terminates the game.\n\n"
               " - map\n    Activates or deactivates the map.\n\n"
               " - flip\n    Changes the possible move direction.\n\n"
               " - move <CHARACTER> <DIRECTION> <DISTANCE>\n"
               "    Moves a character.\n"
               "    <CHARACTER>: the character to move, T/F/S\n"
               "    <DIRECTION>: the direction to move, up/down/left/right\n"
               "    <DISTANCE>: optional, the distance to move\n\n"
               " - unlock\n    Unlocks a nearby door.\n\n"
               " - fight\n    Fights a nearby monster.\n\n"
               " - scry <KNOWN_ROOM> <DIRECTION>\n"
               "    Reveals an unknown room.\n"
               "    <KNOWN_ROOM>: where to scry from\n"
               "    <DIRECTION>: which direction to scry\n" << std::endl;

}
