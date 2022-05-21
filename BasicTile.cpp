#include "BasicTile.hpp"
#include "Game.hpp"
#include "IO.hpp"


BasicTile::BasicTile(TileType type, char room_id, int row, int column) 
  : Tile{type, room_id, row, column, false}
{
  if(type_ == TileType::PASSAGE)
  {
    is_available_ = true;
    is_passable_ = true;
  }
}

std::string BasicTile::getTileString() const
{
  std::shared_ptr<Room> inside_room = MagicMaze::Game::getInstance().getRoomById(inside_room_id_);
  if (!inside_room->isRevealed())
    return IO::UNKNOWN_TILE_STRING;

  std::string basic_tile;
  if(type_ == TileType::WALL)
  {
      basic_tile = IO::WALL_STRING;
      if((row_ == 0) && (column_ == 0))
        basic_tile.replace(31,3, std::string{ inside_room_id_});
  }
  else if(type_ == TileType::PASSAGE)
  {
      basic_tile = IO::PASSAGE_STRING;
      if(character_)
        basic_tile.at(TILE_CENTER) = character_->getCharacterTypeAsChar();
  }

  return basic_tile;
}
