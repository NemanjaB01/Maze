#include "AI.hpp"
#include "Game.hpp"
#include "Room.hpp"
#include "BasicTile.hpp"
#include "MagicTile.hpp"
#include <vector>
#include <memory>

AI& AI::getInstance()
{
  static AI instance{};
  return instance;
}

TileComplex::TileComplex(int row, int col, const std::shared_ptr<Tile>& tile)
   : row_{row}, col_{col}
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
