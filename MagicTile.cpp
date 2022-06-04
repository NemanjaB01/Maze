#include "MagicTile.hpp"
#include "Game.hpp"
#include "BasicTile.hpp"
#include "IO.hpp"

MagicTile::MagicTile(TileType type, char room_id, int row, int column)
 : Tile{type, room_id, row, column, true}
{
  if (type == TileType::CRYSTAL_BALL || type == TileType::HOURGLASS || type == TileType::LOOT ||
      type == TileType::SEER_BUTTON || type == TileType::THIEF_BUTTON || type == TileType::FIGHTER_BUTTON)
  {
    is_available_ = true;
    is_passable_ = true;
  }
}

std::string MagicTile::getTileString() const
{
  std::shared_ptr<Room> room = MagicMaze::Game::getInstance().getRoomById(inside_room_id_);
  if (!room->isRevealed())
    return IO::UNKNOWN_TILE_STRING;

  std::string tile;

  if (type_ == TileType::CRYSTAL_BALL)
    tile = IO::CRYSTAL_BALL_STRING;
  else if (type_ == TileType::HOURGLASS)
    tile = IO::HOURGLASS_STRING;
  else if (type_ == TileType::LOOT)
    tile = IO::LOOT_STRING;
  else if (type_ == TileType::SEER_BUTTON)
    tile = IO::SEER_BUTTON_STRING;
  else if (type_ == TileType::THIEF_BUTTON)
    tile = IO::THIEF_BUTTON_STRING;
  else if (type_ == TileType::FIGHTER_BUTTON)
    tile = IO::FIGHTER_BUTTON_STRING;
  else if (type_ == TileType::MONSTER)
    tile = IO::MONSTER_STRING;
  else if (type_ == TileType::HORIZONTAL_DOOR)
    tile = IO::HORIZONTAL_DOOR_STRING;
  else if (type_ == TileType::VERTICAL_DOOR)
    tile = IO::VERTICAL_DOOR_STRING;
  else if (type_ == TileType::SECRET_DOOR)
    tile = IO::SECRET_DOOR_STRING;

  if (character_)
    tile.at(TILE_CENTER) = character_->getCharacterTypeAsChar();


  return tile;
}

bool MagicTile::magicUsed() const
{
  if (type_ != TileType::LOOT)
    return true;

  return false;
}
