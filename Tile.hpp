#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <string>
#include "Character.hpp"
#include <memory>

enum class TileType { WALL, PASSAGE, HORIZONTAL_DOOR, VECTICAL_DOOR, SECRET_DOOR, MONSTER, HOURGLASS,
                      CRYSTAL_BALL, LOOT, TIEF_BUTTON, FIGHTER_BUTTON, SEER_BUTTON };

class Tile
{
  protected:
    std::shared_ptr <Character> character_;
    TileType type_;
    int row_;
    int column_;
    bool is_available_;
    bool is_passable_;

  public:
    Tile(TileType type, int row, int column, bool is_available, bool is_passable);
    virtual std::string getTileString() = 0;
    TileType getTileType() const { return type_; }
    Tile(const Tile&) = delete;
    Tile& operator=(const Tile& other) = delete;
    virtual ~Tile(){}
    int getRow() const { return row_; }
    int getColumn() const { return column_; }
};


#endif
