#include "Tile.hpp"

Tile::Tile(TileType type, char room_id, int row, int col, bool is_magic)
  : type_{type},
    inside_room_id_{room_id},
    row_{row},
    column_{col},
    is_available_{false},
    is_passable_{false},
    is_magic_{is_magic}
{}

bool Tile::ifContainsCharacter() const
{
  return character_ ? true : false;
}


std::string Tile::getLineOfTile(int index, bool is_revealed)
{
  std::string line;

  switch (index)
  {
    case 1:
      if((type_ == TileType::WALL || type_ == TileType::SECRET_DOOR)&& is_revealed == true)
        line = getTileString().substr(0,21);
      else if(type_ == TileType::VERTICAL_DOOR && is_revealed == true)
        line = getTileString().substr(0,9);
      else 
        line = getTileString().substr(0,7);
      break;
    case 2:
      if(type_ == TileType::WALL && is_revealed == true && row_ == 0 && column_ == 0)
        line = getTileString().substr(22,19);
      else if((type_ == TileType::WALL || type_ == TileType::SECRET_DOOR)&& is_revealed == true)
        line = getTileString().substr(22,21);
      else if(type_ == TileType::VERTICAL_DOOR && is_revealed == true)
        line = getTileString().substr(10,9);
      else if(type_ == TileType::HORIZONTAL_DOOR && is_revealed == true)
        line = getTileString().substr(8,21);
      else
        line = getTileString().substr(8,7);
      break;
    case 3:
      if(type_ == TileType::WALL && is_revealed == true && row_ == 0 && column_ == 0)
        line = getTileString().substr(42,21);
      else if((type_ == TileType::WALL || type_ == TileType::SECRET_DOOR)&& is_revealed == true)
        line = getTileString().substr(44,21);
      else if(type_ == TileType::VERTICAL_DOOR && is_revealed == true)
        line = getTileString().substr(20,9);
      else if(type_ == TileType::HORIZONTAL_DOOR && is_revealed == true)
        line = getTileString().substr(30,7);
      else
        line = getTileString().substr(16,7);
      break;
  }

  return line;
}

void Tile::setCharacter(const std::shared_ptr<Character>& character)
{
  character_ = character;
  if (character)
    is_available_ = false;
  else
    is_available_ = true;
}
