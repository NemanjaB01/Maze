#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <string>
#include "Character.hpp"
#include <memory>

class Room;

const unsigned TILE_CENTER = 11;

enum class TileType { WALL, PASSAGE, HORIZONTAL_DOOR, VERTICAL_DOOR, SECRET_DOOR, MONSTER, HOURGLASS,
                      CRYSTAL_BALL, LOOT, THIEF_BUTTON, FIGHTER_BUTTON, SEER_BUTTON };

class Tile
{
  protected:
    std::shared_ptr<Character> character_;
    TileType type_;
    char inside_room_id_;
    int row_;
    int column_;
    bool is_available_;
    bool is_passable_;
    bool is_magic_;

  public:
    Tile(TileType type, char room_id ,int row, int column, bool is_magic);
    virtual std::string getTileString() const = 0;

    TileType getTileType() const { return type_; }
    char getInsideRoomId() const { return inside_room_id_; }
    int getRow() const { return row_; }
    int getColumn() const { return column_; }
    bool ifMagic() const { return is_magic_; }
    bool ifAvailable() const { return is_available_; }
    bool ifPassable() const { return is_passable_; }

    Tile(const Tile&) = delete;
    Tile& operator=(const Tile& other) = delete;
    virtual ~Tile() noexcept = default;

    void setCharacter(const std::shared_ptr<Character>& character);
    void setInRoom(const char room_id) { inside_room_id_ = room_id; }
    void setAvailable(bool available) { is_available_ = available; }
    bool ifContainsCharacter() const;
    std::string getLineOfTile(int index, bool is_room_revealed);
};

#endif
