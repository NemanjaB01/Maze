#ifndef MAGICTILE_HPP
#define MAGICTILE_HPP

#include "Tile.hpp"
#include "Passage.hpp"
#include "Room.hpp"

#include <string>

class MagicTile : public Tile
{
  protected:
    bool magic_used_;

  public:
    MagicTile(TileType type, int row, int col, bool is_available, bool is_passable)
     : Tile{type, row, col, is_available, is_passable}, magic_used_{false} {}
    virtual ~MagicTile() = default;

    virtual std::string getTileString() = 0;
    virtual void magicUsed();
    bool isAvailable();
};

#endif