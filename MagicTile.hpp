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
     : Tile{type_, row_, column_, is_available_, is_passable_}, magic_used_{false} {}
    ~MagicTile() override = default;

    virtual std::string getTileString() = 0;
    virtual void magicUsed();
};

#endif