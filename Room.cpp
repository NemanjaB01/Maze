#include "Room.hpp"
#include "MagicTile.hpp"
#include "BasicTile.hpp"


Room::Room(char room_id, const std::string& room_info_str,  int row_, int column)
 : room_id_{room_id}, revealed_{false}, row_{row_}, col_{column}, num_of_monsters_{0}
{
  int row{0};
  int col{0};
  for (const char& field : room_info_str)
  {
    TileType type{ TILE_TYPE_MAP.at(field) };

    if (type == TileType::PASSAGE || type == TileType::WALL)
      room_map_.at(row).at(col) = std::make_shared<BasicTile>(type, room_id, row, col);
    else
      room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);

    col += 1;
    if (col >= 5)
    {
      row += 1;
      col = 0;
    }
  }
}


void Room::setTileToPassage(std::shared_ptr<Tile>& t)
{
  const int t_row{t->getRow()};
  const int t_col{t->getColumn()};

  if (t->getTileType() == room_map_.at(t_row).at(t_col)->getTileType())
  {
    t = std::make_shared<BasicTile>(TileType::PASSAGE, room_id_, t_row, t_col);
    room_map_.at(t_row).at(t_col) = t;
  }
}

