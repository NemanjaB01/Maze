#include "Room.hpp"
#include "Wall.hpp"
/*
#include "CrystalBall.hpp"
#include "HorizontalDoor.hpp"
#include "VertiacalDoor.hpp"
#include "HourGlass.hpp"
#include "Loot.hpp"
#include "Monster.hpp"
#include "Passage.hpp"
#include "SecretDoor.hpp"
#include "Button.hpp"
*/
Room::Room(char room_id, const std::string& room_info_str)
 : room_id_{room_id}, revealed_{false}
{
  int row{0};
  int col{0};

  for (const char& field : room_info_str)
  {
    TileType type{ TILE_TYPE_MAP.at(field) };
    switch(type)
    {
      case TileType::WALL:
        room_map_.at(row).at(col) = std::make_shared<Wall>(room_id_, row, col);
        break;
      case TileType::CRYSTAL_BALL:
        room_map_.at(row).at(col) = std::make_shared<CrystalBall>(row, col);
          break;
      case TileType::HORIZONTAL_DOOR:
        room_map_.at(row).at(col) = std::make_shared<HorizontalDoor>(row, col);
          break;
      case TileType::VECTICAL_DOOR:
        room_map_.at(row).at(col) = std::make_shared<VerticalDoor>(row, col);
          break;
      case TileType::HOURGLASS:
        room_map_.at(row).at(col) = std::make_shared<HourGlass>(row, col);
          break;
      case TileType::LOOT:
        room_map_.at(row).at(col) = std::make_shared<Loot>(row, col);
          break;
      case TileType::MONSTER:
        room_map_.at(row).at(col) = std::make_shared<Monster>(row, col);
          break;
      case TileType::PASSAGE:
        room_map_.at(row).at(col) = std::make_shared<Passage>(row, col);
          break;
      case TileType::SECRET_DOOR:
        room_map_.at(row).at(col) = std::make_shared<SecretDoor>(row, col);
          break;
      case TileType::FIGHTER_BUTTON:
        room_map_.at(row).at(col) = std::make_shared<Button>('F', row, col);
          break;
      case TileType::SEER_BUTTON:
        room_map_.at(row).at(col) = std::make_shared<Button>('S', row, col);
          break;
      case TileType::TIEF_BUTTON:
        room_map_.at(row).at(col) = std::make_shared<Button>('T', row, col);
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