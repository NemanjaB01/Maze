#include "BasicTile.hpp"
#include "Game.hpp"


BasicTile::BasicTile(TileType type, char room_id, int row, int column) 
  : Tile{type, room_id, row, column, false}
{
  if(type_ == TileType::PASSAGE)
  {
    is_available_ = true;
    is_passable_ = true;
  }
}

std::string BasicTile::getTileString()
{
  std::shared_ptr<Room> inside_room = Game::getInstance().getRoomById(inside_room_id_);
  if (!inside_room->isRevealed())
    return "UUUUUUU\nUUUUUUU\nUUUUUUU\n";

  std::string basic_tile;
  if(type_ == TileType::WALL)
  {
      basic_tile =  "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                    "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                    "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n";

      if((row_ == 0) && (column_ == 0))
      {
        std::string string;
        string += inside_room_id_;
        basic_tile.replace(31,3,string);
      }
  }
  else if(type_ == TileType::PASSAGE)
  {
      basic_tile = "       \n"
                   "       \n"
                   "       \n";
      if(character_)
        basic_tile.at(TILE_CENTER) = character_->getCharacterTypeAsChar();
  }

  return basic_tile;
}
