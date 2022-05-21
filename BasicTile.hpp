#ifndef BASICTILE_HPP
#define BASICTILE_HPP

#include <string>
#include "Tile.hpp"
#include "Room.hpp"

class BasicTile : public Tile
{
  public:
    BasicTile(TileType type, char room_id, int row, int column);
    std::string getTileString() const override;

    BasicTile(const BasicTile&) = delete;
    BasicTile operator=(const BasicTile&) = delete;
    ~BasicTile() noexcept override = default;
};


#endif