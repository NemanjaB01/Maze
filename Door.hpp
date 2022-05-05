#ifndef DOOR_HPP
#define DOOR_HPP

#include "MagicTile.hpp"

#include <string>

class Door : public MagicTile
{
  public:
    Door(TileType type, int row, int column);
    Door(const Door&) = delete;
    Door& operator=(const Door&) = delete;
    ~Door() override = default;

    std::string getTileString() override;
};

#endif