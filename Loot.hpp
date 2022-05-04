#ifndef LOOT_HPP
#define LOOT_HPP

#include "MagicTile.hpp"

#include <string>

class Loot : public MagicTile
{
  public:
    Loot(int row, int column) : MagicTile{TileType::LOOT, row, column, true, true} {}
    ~Loot() override = default;
    std::string getTileString() override;
    void magicUsed();
};

#endif