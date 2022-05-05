#ifndef BONUSTILE_HPP
#define BONUSTILE_HPP

#include <string>
#include "MagicTile.hpp"

class BonusTile : public MagicTile
{
  private:
    const char letter_;

  public:
    BonusTile(TileType tiletype, int row, int column, const char letter) : MagicTile{type, row, column, true, true}
    , letter_{letter} {}
    BonusTile operator=(const BonusTile&) = delete;
    std::string getTileString() override;
    BonusTile(const BonusTile&) = delete;
    ~BonusTile() override= default;


};


#endif