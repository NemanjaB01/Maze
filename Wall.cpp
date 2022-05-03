#include "Wall.hpp"

std::string Wall::getTileString()
{
  std::string wall =  "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n\u2588\u2588\u2588\n"
                      "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n\u2588\u2588\u2588\n"
                      "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n\u2588\u2588\u2588\n";

  if((row_ == 0) && (column_ == 0))
  {
    wall.at(12) = room_id_;
  }


  return wall;
}