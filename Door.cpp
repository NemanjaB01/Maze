#include "Door.hpp"

Door::Door(TileType type, int row, int column)
 : MagicTile(type, row, column, false, false) {}

std::string Door::getTileString()
{
  std::string door;

  if (type_ == TileType::HORIZONTAL_DOOR)
    door = "       \n███████\n       \n";

  else if (type_ == TileType::VECTICAL_DOOR)
    door = "   █   \n   █   \n   █   \n";

  else if (type_ == TileType::SECRET_DOOR)
    door = "███████\n███████\n███████\n";

  return door;
}