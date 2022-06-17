#include "Room.hpp"
#include "MagicTile.hpp"
#include "BasicTile.hpp"


Room::Room(char room_id, const std::string& room_info_str,  int row_, int column)
 : room_id_{room_id}, revealed_{false}, row_{row_}, col_{column}, num_of_monsters_{0}, contains_button_{false}
{
  int row{0};
  int col{0};
  for (const char& field : room_info_str)
  {
    TileType type{ TILE_TYPE_MAP.at(field) };

    if (type == TileType::PASSAGE || type == TileType::WALL)
      room_map_.at(row).at(col) = std::make_shared<BasicTile>(type, room_id, row, col);
    else
    {
      room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);

      if (type == TileType::MONSTER)
        num_of_monsters_++;

      else if (type == TileType::SECRET_DOOR)
        secret_doors_.push_back(std::dynamic_pointer_cast<MagicTile>(room_map_.at(row).at(col)));

      else if (type == TileType::SEER_BUTTON || type == TileType::FIGHTER_BUTTON || type == TileType::THIEF_BUTTON)
        contains_button_ = true;
    }

    col += 1;
    if (col >= 5)
    {
      row += 1;
      col = 0;
    }
  }
}


void Room::setTileToPassage(const int& tile_row, const int& tile_column, std::shared_ptr<Character> character)
{
  std::shared_ptr<Tile> new_tile{ std::make_shared<BasicTile>(TileType::PASSAGE, room_id_, tile_row, tile_column) };
  room_map_.at(tile_row).at(tile_column) = new_tile;
  if (character)
  {
    character->setCurrentTile(new_tile);
    new_tile->setCharacter(character);
  }
}
