#include "Tile.hpp"

Tile::Tile(TileType type, int row, int col, bool is_available, bool is_passable, bool is_changeable)
  : character_{nullptr},
    type_{type},
    row_{row},
    column_{col},
    is_available_{is_available},
    is_passable_{is_passable},
    is_changeable_{is_changeable}
{}