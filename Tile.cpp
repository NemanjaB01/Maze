#include "Tile.hpp"

Tile::Tile(TileType type, int row, int col, bool is_available, bool is_passable, bool is_magic)
  : type_{type},
    row_{row},
    column_{col},
    is_available_{is_available},
    is_passable_{is_passable},
    is_magic_{is_magic}
{}
