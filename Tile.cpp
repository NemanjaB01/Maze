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
