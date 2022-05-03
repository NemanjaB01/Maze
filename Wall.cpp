#include "Wall.hpp"

std::string Wall::getTileString() const
{
  std::string wall;
  Room room_id;
  char c = room_id.getRoomId();
  if((c = 'S') && (row_ == 0) && (column_ == 0))
  {
    wall ="\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n\u2588\u2588\u2588";
    wall.push_back(c);
    wall += "\u2588\u2588\u2588\n\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n";
  }
  else
  {
    wall ="\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
          "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
          "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n";
  }

  return wall;
}