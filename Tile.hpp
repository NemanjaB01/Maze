#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <string>
#include "Character.hpp"
#include <memory>

class Room;

enum class TileType { WALL, PASSAGE, HORIZONTAL_DOOR, VECTICAL_DOOR, SECRET_DOOR, MONSTER, HOURGLASS,
                      CRYSTAL_BALL, LOOT, TIEF_BUTTON, FIGHTER_BUTTON, SEER_BUTTON };

class Tile
{
  protected:
    std::shared_ptr<Character> character_;
    std::weak_ptr<Room> in_room_;
    TileType type_;
    int row_;
    int column_;
    bool is_available_;
    bool is_passable_;
    bool is_magic_;

  public:
    Tile(TileType type, int row, int column, bool is_available, bool is_passable, bool is_magic);
    virtual std::string getTileString() = 0;

    TileType getTileType() const { return type_; }
    std::weak_ptr<Room> getInRoom() const { return in_room_; }
    int getRow() const { return row_; }
    int getColumn() const { return column_; }
    bool ifMagic() const { return is_magic_; }
    bool ifAvailable() const { return is_available_; }
    bool ifPassable() const { return is_passable_; }

    Tile(const Tile&) = delete;
    Tile& operator=(const Tile& other) = delete;
    virtual ~Tile(){}
    void setCharacter(const std::shared_ptr<Character>& character) { character_ = character; }
    void setInRoom(const std::shared_ptr<Room>& room) { in_room_ = room; }
};

#endif
