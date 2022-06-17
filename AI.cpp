#include "AI.hpp"
#include "Character.hpp"
#include "CharacterAI.hpp"
#include "Game.hpp"
#include "Room.hpp"
#include "BasicTile.hpp"
#include "MagicTile.hpp"
#include <queue>
#include <vector>
#include <memory>
#include "Tile.hpp"
#include "map"
#include "queue"
#include <exception>

AI& AI::getInstance()
{
  static AI instance{};
  return instance;
}

void AI::copyGameboard()
{
  auto rooms = MagicMaze::Game::getInstance().getRooms();

  int board_row{0};
  for (std::vector<std::shared_ptr<Room>>& row_of_rooms : rooms)
  {
    for (int i{0}; i < 5; i++)
    {
      std::vector<std::shared_ptr<Tile>> gameboard_row;
      int board_col{0};

      for (std::shared_ptr<Room>& room : row_of_rooms)
      {
        for (int room_row_iterations{0}; room_row_iterations < 5; room_row_iterations++)
        {
          std::shared_ptr<Tile> current_tile = room->getRoomMap().at(board_row % 5).at(board_col % 5);

          if (std::dynamic_pointer_cast<BasicTile>(current_tile))
            gameboard_row.push_back(std::make_shared<BasicTile>(current_tile->getTileType(),
                                      current_tile->getInsideRoomId(), board_row, board_col));
          else
            gameboard_row.push_back(std::make_shared<MagicTile>(current_tile->getTileType(),
                                      current_tile->getInsideRoomId(), board_row, board_col));
          board_col++;
        }
      }
      gameboard_.push_back(gameboard_row);
      board_row++;
    }
  }
  auto characters = MagicMaze::Game::getInstance().getAllCharacters();

  for (int counter{0}; counter < MagicMaze::CHARACTERS_NUMBER; counter++)
  {
    characters_.at(counter) = std::make_shared<CharacterAI>(characters.at(counter));
    characters_.at(counter)->updateCurrentTile(characters.at(counter));
  }
}

void AI::copySpecificTile(std::shared_ptr<Tile>& tile)
{
  const std::shared_ptr<Room> room = MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId());

  const int row{ tile->getRow() + room->getRow() * 5 };
  const int col{ tile->getColumn() + room->getColumn() * 5 };

  if (std::dynamic_pointer_cast<BasicTile>(tile))
    gameboard_.at(row).at(col) = std::make_shared<BasicTile>(tile->getTileType(), tile->getInsideRoomId(), row, col);
  else
    gameboard_.at(row).at(col) = std::make_shared<MagicTile>(tile->getTileType(), tile->getInsideRoomId(), row, col);
}

std::shared_ptr<CharacterAI> AI::getCharacterAIById(const char& id)
{
  switch(id)
  {
    case 'F':
      return characters_.at(0);
    case 'T':
      return characters_.at(1);
    case 'S':
      return characters_.at(2);
  }
  return nullptr;
}

void AI::copySpecificTile(std::shared_ptr<Tile>& tile, const std::shared_ptr<Character>& character)
{
  copySpecificTile(tile);
  std::shared_ptr<CharacterAI> current_ai_character{ getCharacterAIById(character->getCharacterTypeAsChar()) };

  current_ai_character->updateCurrentTile(character);
}


void AI::determineHighPriorities()
{
  const bool buttons_visible{MagicMaze::Game::getInstance().ifAllButtonsVisible()};

  if (!MagicMaze::Game::getInstance().getRoomById('L')->isRevealed() || !buttons_visible)
  {
    for (auto& character : characters_)
      character->setPriority(PRIORITY::REVEAL);
  }
  else if (buttons_visible && !MagicMaze::Game::getInstance().checkIfAllCharactersOnButton())
  {
    for (auto& character : characters_)
      character->setPriority(PRIORITY::BUTTON);
  }
  else
    for (auto& character : characters_)
      character->setPriority(PRIORITY::LOOT);
}


void AI::play()
{
  determineHighPriorities();
}

void AI::checkSidesOfTile(std::queue<std::shared_ptr<Tile>>& tiles, std::vector<std::vector<bool>>& visited)
{
  const int row = tiles.front()->getRow();
  const int column = tiles.front()->getColumn();

  static const std::array<int, 4> ROW_VALUES { -1, 0, 1, 0}; // UP  RIGHT  DOWN  LEFT
  static const std::array<int, 4> COL_VALUES { 0, 1, 0, -1};

  for(int n{0}; n < 4; n++)
  {
    try
    {
      std::shared_ptr<Tile> tile = gameboard_.at(row + ROW_VALUES.at(n)).at(column + COL_VALUES.at(n));
      if(!visited.at(tile->getRow()).at(tile->getColumn()) && tile->getTileType() != TileType::WALL)
      {
        visited.at(tile->getRow()).at(tile->getColumn()) = true;
        tiles.push(tile);
      }
    }
    catch(std::out_of_range& e)
    {
      continue;
    }
  }
}

void AI::checkPriority(const std::shared_ptr<CharacterAI> character, bool& goal_found,
                       const std::shared_ptr<Tile>& current_tile)
{
  const PRIORITY priority = character->getPriority();
  const TileType type = current_tile->getTileType();

  if(priority == PRIORITY::LOOT && type == TileType::LOOT)
  {
    character->setGoalTile(current_tile);
    goal_found = true;
  }
  switch(character->getCharacterType())
  {
    case CharacterType::FIGHTER:
      if((type == TileType::FIGHTER_BUTTON && priority == PRIORITY::BUTTON) ||
          (type == TileType::MONSTER && priority == PRIORITY::FIGHT))
        {
          character->setGoalTile(current_tile);
          goal_found = true;
        }
        break;
    case CharacterType::SEER:
      if((type == TileType::SEER_BUTTON && priority == PRIORITY::BUTTON) ||
          (type == TileType::CRYSTAL_BALL && priority == PRIORITY::REVEAL))
      {
        character->setGoalTile(current_tile);
        goal_found = true;
      }
      break;
    case CharacterType::THIEF:
      if((type == TileType::THIEF_BUTTON && priority == PRIORITY::BUTTON) ||
          ((TileType::HORIZONTAL_DOOR == type || TileType::VERTICAL_DOOR == type) && priority == PRIORITY::UNLOCK))
      {
        character->setGoalTile(current_tile);
        goal_found = true;
      }
      break;
    case CharacterType::NONE:
      break;
  }
}

void AI::findGoalTile(const std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  const int row = tile->getRow();
  const int column = tile->getColumn();
  bool goal_found = false;

  std::vector<std::vector<bool>> visited(gameboard_.size(), std::vector<bool>(gameboard_.at(0).size(), false));
  std::queue<std::shared_ptr<Tile>> current_tiles;

  current_tiles.push(gameboard_.at(row).at(column));
  visited.at(row).at(column) = true;

  while(current_tiles.empty())
  {
    checkSidesOfTile(current_tiles, visited);

    checkPriority(character, goal_found, current_tiles.front());
    if (goal_found)
      break;

    current_tiles.pop();
  }
}
