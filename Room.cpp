#include "Room.hpp"
#include "MagicTile.hpp"
#include "BasicTile.hpp"


Room::Room(char room_id, const std::string& room_info_str,  int row_, int column)
 : room_id_{room_id}, revealed_{false}, row_{row_}, col_{column}
{
  int row{0};
  int col{0};

  for (const char& field : room_info_str)
  {
    TileType type{ TILE_TYPE_MAP.at(field) };
    switch(type)
    {
      case TileType::WALL:
        room_map_.at(row).at(col) = std::make_shared<BasicTile>(type, room_id, row, col);
        break;
      case TileType::PASSAGE:
        room_map_.at(row).at(col) = std::make_shared<BasicTile>(type, room_id, row, col);
        break;
      case TileType::CRYSTAL_BALL:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::HOURGLASS:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::HORIZONTAL_DOOR:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::VECTICAL_DOOR:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::SECRET_DOOR:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::LOOT:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::MONSTER:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::FIGHTER_BUTTON:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::SEER_BUTTON:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
      case TileType::TIEF_BUTTON:
        room_map_.at(row).at(col) = std::make_shared<MagicTile>(type, room_id, row, col);
        break;
    }
    col += 1;
    if (col >= 5)
    {
      row += 1;
      col = 0;
    }
  }
}


void Room::setTileToPassage(const std::shared_ptr<Tile>& t)
{
  const int t_row{t->getRow()};
  const int t_col{t->getColumn()};

  if (t->getTileType() == room_map_.at(t_row).at(t_col)->getTileType())
  {
    room_map_.at(t_row).at(t_col) = std::make_shared<BasicTile>(TileType::PASSAGE, room_id_, t_row, t_col);
  }
}

