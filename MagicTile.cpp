#include "MagicTile.hpp"
#include "Game.hpp"
#include "BasicTile.hpp"

MagicTile::MagicTile(TileType type, char room_id ,int row, int column)
 : Tile{type, room_id, row, column, true}
{
  if (type == TileType::CRYSTAL_BALL || type == TileType::HOURGLASS || type == TileType::LOOT ||
  type == TileType::SEER_BUTTON || type == TileType::TIEF_BUTTON || type == TileType::FIGHTER_BUTTON)
  {
    is_available_ = true;
    is_passable_ = true;
  }
}

std::string MagicTile::getTileString()
{
  std::shared_ptr<Room> room = Game::getInstance().getRoomById(inside_room_id_);
  if (!room->isRevealed())
  {
    return "UUUUUUU\nUUUUUUU\nUUUUUUU\n";
  }

  std::string tile;

  if(type_ == TileType::CRYSTAL_BALL)
    tile = "  / \\  \n |   | \n  \\ /  \n";
  else if(type_ == TileType::HOURGLASS)
    tile = " \\   / \n  | |  \n /   \\ \n";
  else if(type_ == TileType::LOOT)
    tile = "$$$$$$$\n$     $\n$$$$$$$\n";
  else if(type_ == TileType::SEER_BUTTON)
    tile = "SSSSSSS\nS     S\nSSSSSSS\n";
  else if(type_ == TileType::TIEF_BUTTON)
    tile = "TTTTTTT\nT     T\nTTTTTTT\n";
  else if(type_ == TileType::FIGHTER_BUTTON)
    tile = "FFFFFFF\nF     F\nFFFFFFF\n";
  else if(type_ == TileType::MONSTER)
    tile = "       \n   M   \n       \n";
  else if(type_ == TileType::HORIZONTAL_DOOR)
    tile = "       \n███████\n       \n";
  else if(type_ == TileType::VECTICAL_DOOR)
    tile = "   █   \n   █   \n   █   \n";
  else if(type_ == TileType::SECRET_DOOR)
    tile = "███████\n███████\n███████\n";

  if(character_)
    tile.at(TILE_CENTER) = character_->getCharacterTypeAsChar();

  return tile;
}

void MagicTile::magicUsed(std::shared_ptr<Tile>& tile)
{
  Game &g = Game::getInstance();
  std::shared_ptr<Room> room = g.getRoomById(tile->getInsideRoomId());
  if (tile->ifMagic())
    room->setTileToPassage(tile);
}
