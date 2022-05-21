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

void MagicMaze::Game::flip()
{
  cards_.push(cards_.front());
  cards_.pop();

  flips_number_++;
}

void MagicMaze::Game::placeCharacterOnStartingPosition()
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

void MagicMaze::Game::prepareGame()
{
  placeCharacterOnStartingPosition();

  std::cout << "Welcome to the magical OOP1 Maze!!!" << std::endl;
  std::cout << "Card Flip Counter:   " << getFlipsNumber() << std::endl;
  printMap();
  std::cout << "Possible move: " << getPossibleMoveAsString() << std::endl;
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

void MagicMaze::Game::useHourglass(std::shared_ptr<Tile>& tile)
{
  if (flips_number_ >= 5)
    flips_number_ -= 5;
  else
    flips_number_ = 0;
  std::shared_ptr<MagicTile> hourglass = std::dynamic_pointer_cast<MagicTile>(tile);
  hourglass->magicUsed();

}

void MagicMaze::Game::moveInputParsing(std::vector<std::string>& input, std::shared_ptr<Character>& character_to_move,
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
  std::transform(possible_move_str.begin(), possible_move_str.end(), possible_move_str.begin(), toupper);

  if (direction_to_go != "UP" && direction_to_go != "DOWN" && direction_to_go != "RIGHT" && direction_to_go != "LEFT")
    throw character_to_move->getFullName() + ": \"I don't understand where I should go!\"";

  else if (direction_to_go != possible_move_str)
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

void MagicMaze::Game::changeNextRowCol(int& next_row, int& next_col)
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

void MagicMaze::Game::getTilesOnTheWay(std::queue<std::shared_ptr<Tile>>& tiles_on_way,
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
      if (current_room->getRow() + 1 == static_cast<int>(rooms_.size()))
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
      if (current_room->getColumn() + 1 == static_cast<int>(rooms_.at(0).size()))
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


void MagicMaze::Game::fightMonster()
{
  std::shared_ptr<Character> character = getCharacter(CharacterType::FIGHTER);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  int row = tile->getRow();
  int col = tile->getColumn();
  const char room_id = tile->getInsideRoomId();
  const int game_row = rooms_.size();
  const int game_col = rooms_[0].size();
  std::queue<std::shared_ptr<Tile>> monster;

  std::shared_ptr<Room> single_room = getRoomById(room_id);

  if(col != 4 && single_room->getRoomMap().at(row).at(col + 1)->getTileType() == TileType::MONSTER)
    monster.push(single_room->getRoomMap().at(row).at(col + 1));

  else if(col == 4 && game_col > (single_room->getColumn() + 1) && rooms_[single_room->getRow()][single_room->getColumn() + 1]
    ->getRoomMap().at(row).at(0)->getTileType() == TileType::MONSTER)
    monster.push(rooms_[single_room->getRow()][single_room->getColumn() + 1]->getRoomMap().at(row).at(0));

  if(col != 0 && single_room->getRoomMap().at(row).at(col - 1)->getTileType() == TileType::MONSTER)
    monster.push(single_room->getRoomMap().at(row).at(col - 1));

  else if(col == 0 && (single_room->getColumn() - 1) > 0 && rooms_[single_room->getRow()][single_room->getColumn() - 1]
    ->getRoomMap().at(row).at(4)->getTileType() == TileType::MONSTER)
    monster.push(rooms_[single_room->getRow()][single_room->getColumn() - 1]->getRoomMap().at(row).at(4));

  if(row != 4 && single_room->getRoomMap().at(row + 1).at(col)->getTileType() == TileType::MONSTER)
    monster.push(single_room->getRoomMap().at(row + 1).at(col));

  else if(row == 4 && (single_room->getRow() + 1) < game_row && rooms_[single_room->getRow() + 1][single_room->getColumn()]
    ->getRoomMap().at(0).at(col)->getTileType() == TileType::MONSTER)
    monster.push(rooms_[single_room->getRow() + 1][single_room->getColumn()]->getRoomMap().at(0).at(col));

  if(row != 0 && single_room->getRoomMap().at(row - 1).at(col)->getTileType() == TileType::MONSTER)
    monster.push(single_room->getRoomMap().at(row - 1).at(col));

  else if(row == 0 && (single_room->getRow() - 1) > 0 && rooms_[single_room->getRow() - 1][single_room->getColumn()]
    ->getRoomMap().at(4).at(col)->getTileType() == TileType::MONSTER)
    monster.push(rooms_[single_room->getRow() - 1][single_room->getColumn()]->getRoomMap().at(4).at(col));

  if(monster.empty())
    throw character->getFullName() + ":  \"Nothing to fight here!\"";

  while(!monster.empty())
  {
    std::dynamic_pointer_cast<MagicTile>(monster.front())->magicUsed();
    getRoomById(monster.front()->getInsideRoomId())->decreaseNumMonsters();
    monster.pop();
  }
}


MagicMaze::DIRECTIONS_TYPES MagicMaze::Game::checkDirection(std::string direction,
                                                            std::shared_ptr<Character>& character)
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

void MagicMaze::Game::scryInputParsing(std::vector<std::string>& input, std::shared_ptr<Room>& room_to_scry,
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

void MagicMaze::Game::scry(std::vector<std::string>& input)
{
  std::shared_ptr<Character> character = getCharacter(CharacterType::SEER);
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  const int row = tile->getRow();
  const int col = tile->getColumn();
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
    rooms_.at(room_to_scry->getRow()).at(room_to_scry->getColumn() + 1)->reveal();
    std::dynamic_pointer_cast<MagicTile>(character_room->getRoomMap().at(row).at(col))->magicUsed();
    break;
  case DIRECTIONS_TYPES::LEFT:
    rooms_.at(room_to_scry->getRow()).at(room_to_scry->getColumn() - 1)->reveal();
    std::dynamic_pointer_cast<MagicTile>(character_room->getRoomMap().at(row).at(col))->magicUsed();
    break;
  case DIRECTIONS_TYPES::DOWN:
    rooms_.at(room_to_scry->getRow() + 1).at(room_to_scry->getColumn())->reveal();
    std::dynamic_pointer_cast<MagicTile>(character_room->getRoomMap().at(row).at(col))->magicUsed();
    break;
  case DIRECTIONS_TYPES::UP:
    rooms_.at(room_to_scry->getRow() - 1).at(room_to_scry->getColumn())->reveal();
    std::dynamic_pointer_cast<MagicTile>(character_room->getRoomMap().at(row).at(col))->magicUsed();
    break;
  }

  character_room->getRoomMap().at(row).at(col)->setCharacter(character);
  character->setCurrentTile(character_room->getRoomMap().at(row).at(col));
}
