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

AI& AI::getInstance()
{
  static AI instance{};
  return instance;
}

TileComplex::TileComplex(int row, int col, const std::shared_ptr<Tile>& tile)
   : row_{row}, col_{col}, visited_(false)
{
  std::shared_ptr<MagicTile> magic = std::dynamic_pointer_cast<MagicTile>(tile);
  std::shared_ptr<BasicTile> basic = std::dynamic_pointer_cast<BasicTile>(tile);

  if (magic)
    tile_ = std::make_shared<MagicTile>(*magic);
  else
    tile_ = std::make_shared<BasicTile>(*basic);
}

void AI::copyGameboard()
{
  auto rooms = MagicMaze::Game::getInstance().getRooms();

  int board_row{0};
  for (std::vector<std::shared_ptr<Room>>& row_of_rooms : rooms)
  {
    for (int i{0}; i < 5; i++)
    {
      std::vector<std::shared_ptr<TileComplex>> gameboard_row;
      int board_col{0};

      for (std::shared_ptr<Room>& room : row_of_rooms)
      {
        for (int room_row_iterations{0}; room_row_iterations < 5; room_row_iterations++)
        {
          std::shared_ptr<Tile> current_tile = room->getRoomMap().at(board_row % 5).at(board_col % 5);
          gameboard_row.push_back(std::make_shared<TileComplex>(board_row, board_col, current_tile));
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
  }
}


void AI::copySpecificTile(std::shared_ptr<Tile>& tile)
{
  const std::shared_ptr<Room> room = MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId());

  const int board_row{ tile->getRow() + room->getRow() * 5 };
  const int board_col{ tile->getColumn() + room->getColumn() * 5 };

  std::shared_ptr<Tile> searched_tile = room->getRoomMap().at(tile->getRow()).at(tile->getColumn());

  gameboard_.at(board_row).at(board_col) = std::make_shared<TileComplex>(board_row, board_col, searched_tile);
}


void AI::play()
{}

void AI::checkSidesOfTile(std::queue<std::shared_ptr<TileComplex>>& neighbour_tiles,
                           std::shared_ptr<TileComplex> current_tile)
{
  int row = current_tile->tile_->getRow();
  int column = current_tile->tile_->getColumn();
  

  std::array<int, 4> row_values { -1, 0,  1,  0}; // UP  RIGHT  DOWN  LEFT
  std::array<int, 4> col_values {  0, 1,  0, -1};

  for(int n{0}; n < 4; n++)
  {
    std::shared_ptr<TileComplex> tile = gameboard_.at(row + row_values.at(n)).at(column + col_values.at(n));
    if(tile->visited_ == false && tile->tile_->getTileType() != TileType::WALL)
    {
      tile->visited_ = true;
      neighbour_tiles.push(tile);
    }
  }
}

void AI::checkPriority(std::shared_ptr<CharacterAI> character, bool& find_goal,
                       std::queue<std::shared_ptr<TileComplex>>& current_tiles,
                       std::queue<std::shared_ptr<TileComplex>>& tiles_to_check)
{
  PRIORITY priority = character->getPriority();

  while(tiles_to_check.empty())
  {
    TileType type = tiles_to_check.front()->tile_->getTileType();
    
    if(priority == PRIORITY::LOOT && type == TileType::LOOT)
    {
      character->setTile(tiles_to_check.front()->tile_);
      find_goal = true;
    }
    
    switch(character->getCharacter()->getCharacterType())
    {
      case CharacterType::FIGHTER:
        if((type == TileType::FIGHTER_BUTTON && priority == PRIORITY::BUTTON) ||
           (type == TileType::MONSTER && priority == PRIORITY::FIGHT))
         {
            character->setTile(tiles_to_check.front()->tile_);
            find_goal = true;
         }
         break;
      case CharacterType::SEER:
        if((type == TileType::SEER_BUTTON && priority == PRIORITY::BUTTON) ||
           (type == TileType::CRYSTAL_BALL && priority == PRIORITY::REVEAL))
        {
          character->setTile(tiles_to_check.front()->tile_);
          find_goal = true;
        }
        break;
      case CharacterType::THIEF:
        if((type == TileType::THIEF_BUTTON && priority == PRIORITY::BUTTON) ||
           ((TileType::SECRET_DOOR == type || TileType::VERTICAL_DOOR == type) && priority == PRIORITY::UNLOCK))
        {
          character->setTile(tiles_to_check.front()->tile_);
          find_goal = true;
        }
        break;
      case CharacterType::NONE:
        break;
    }

    if(find_goal == true)
      break;

    current_tiles.push(tiles_to_check.front());
    tiles_to_check.pop();
  }

  deleteQueue(current_tiles, tiles_to_check);
}


void AI::deleteQueue(std::queue<std::shared_ptr<TileComplex>>& first, 
                     std::queue<std::shared_ptr<TileComplex>>& second)
{
  while(first.empty())
    first.pop();

  while(second.empty())
    second.pop();

  for(auto& game_board_row : gameboard_)
    for(auto& tile : game_board_row)
      tile->visited_ = false;
}

void AI::findGoalTile(const std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<Tile> tile = character->getTile().lock();
  int row = tile->getRow();
  int column = tile->getColumn();
  bool find_goal= false;

  std::shared_ptr<TileComplex> first_tile{gameboard_.at(row).at(column)};
  std::queue<std::shared_ptr<TileComplex>> current_tiles;
  std::queue<std::shared_ptr<TileComplex>> tiles_to_check;
  current_tiles.push(first_tile);


  while(!find_goal)
  {
    while(current_tiles.empty())
    {
      checkSidesOfTile(tiles_to_check, current_tiles.front());
      current_tiles.pop();
    }

    checkPriority(character, find_goal, current_tiles, tiles_to_check);
  }


}
