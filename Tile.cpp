#include "Tile.hpp"

Tile::Tile(TileType type, int row, int col, bool is_magic)
  : type_{type},
    row_{row},
    column_{col},
    is_available_{false},
    is_passable_{false},
    is_magic_{is_magic}
{}
