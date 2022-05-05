#ifndef DOOR_HPP
#define DOOR_HPP

#include "MagicTile.hpp"

#include <string>

class Door : public MagicTile
{
  bool is_secret_;

  public:
    Door(TileType type, int row, int column, bool is_secret);
    Door(const Door&) = delete;
    Door& operator=(const Door&) = delete;
    ~Door() override = default;

    std::string getTileString() override;
    bool IsSecret() const { return is_secret_; }
};

#endif