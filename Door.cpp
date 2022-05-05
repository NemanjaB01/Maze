#include "Door.hpp"

Door::Door(TileType type, int row, int column, bool is_secret)
 : MagicTile(type, row, column, false, false)
{
  type == TileType::SECRET_DOOR ? is_secret = true : is_secret = false;
}

std::string Door::getTileString()
{
  std::string door;

  switch (type_)
  {
  case TileType::HORIZONTAL_DOOR:
    door = "       \n███████\n       \n";
    break;
  case TileType::VECTICAL_DOOR:
    door = "   █   \n   █   \n   █   \n";
    break;
  case TileType::SECRET_DOOR:
    door = "███████\n███████\n███████\n";
    break;
  }

  return door;
}