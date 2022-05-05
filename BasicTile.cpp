#include "BasicTile.hpp"


BasicTile::BasicTile(TileType type, int row, int column) 
  : Tile{type, row , column, false}
{
  if(type_ == TileType::WALL)
  {
    is_available_ = false;
    is_passable_ = false;
  }
  else if(type_ == TileType::PASSAGE)
  {
    is_available_ = true;
    is_passable_ = true;
  }
}


BasicTile::getTileString()
{
  std::string basic_tile;
  if(type_ == TileType::WALL)
  {
      basic_tile =  "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                    "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                    "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n";

      if((row_ == 0) && (column_ == 0))
      {
        basic_tile.at(12) = in_room_.lock()->getRoomId();
      }
  }
  else if(type_ == TileType::PASSAGE)
  {
      basic_tile = "       \n"
                   "       \n"
                   "       \n";
      if(character_)
      {
        basic_tile.at(12) = character_->getCharacterTypeAsChar();
        is_available_ = false;
      } 
      else if(!is_available_)
        is_available_ = true;
  }

  return basic_tile;
}