#ifndef LOOT_HPP
#define LOOT_HPP

#include "MagicTile.hpp"

#include <string>

class Loot : public MagicTile
{
  public:
    Loot() : MagicTile{TileType::LOOT, row_, column_, true, true} {}
    ~Loot() = default;
    std::string getTileString() override { return "$$$$$$$\n$     $\n$$$$$$$\n"; }
};

#endif