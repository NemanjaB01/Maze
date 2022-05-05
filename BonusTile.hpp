#ifndef BONUSTILE_HPP
#define BONUSTILE_HPP

#include <string>
#include "MagicTile.hpp"

class BonusTile : public MagicTile
{
  private:
    TileType type_;

  public:
    BonusTile(TileType type, int row, int column) : MagicTile{type, row, column, true, true} , type_{type} {}
    BonusTile operator=(const BonusTile&) = delete;
    std::string getTileString() override;
    BonusTile(const BonusTile&) = delete;
    ~BonusTile() override= default;


};


#endif