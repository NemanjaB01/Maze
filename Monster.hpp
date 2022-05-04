#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "MagicTile.hpp"

class Monster : public MagicTile
{
  public:
    Monster(int row, int col) : MagicTile(TileType::MONSTER, row, col, false, false) {}
    ~Monster() override = default;
    Monster& operator=(const Monster&) = delete;
    Monster(const Monster&) = delete;

    std::string getTileString() override { return "       \n   M   \n       \n"; };
    void magicUsed() override;
};

#endif // MONSTER_HPP
