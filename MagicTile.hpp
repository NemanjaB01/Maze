#ifndef MAGICTILE_HPP
#define MAGICTILE_HPP

#include "Tile.hpp"
#include "Room.hpp"

#include <string>

class MagicTile : public Tile
{
  public:
    MagicTile(TileType type, char room_id ,int row, int column);

    MagicTile(const MagicTile&) = delete;
    MagicTile& operator=(const MagicTile&) = delete;
    ~MagicTile() = default;

    std::string getTileString() override;
    static void magicUsed(std::shared_ptr<Tile>& tile);
};

#endif