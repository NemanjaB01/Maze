#ifndef ROOM_HPP
#define ROOM_HPP

#include "Tile.hpp"

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <memory>

const std::map<char, TileType> TILE_TYPE_MAP{
  {'W', TileType::WALL}, {'P', TileType::PASSAGE}, {'X', TileType::SECRET_DOOR}, {'M', TileType::MONSTER}, 
  {'G', TileType::HOURGLASS}, {'C', TileType::CRYSTAL_BALL}, {'H', TileType::HORIZONTAL_DOOR}, 
  {'V', TileType::VECTICAL_DOOR}, {'L', TileType::LOOT}, {'T', TileType::TIEF_BUTTON},
  {'F', TileType::FIGHTER_BUTTON}, {'S', TileType::SEER_BUTTON} };

class Room
{
  private:
    const char room_id_;
    std::array<std::array<std::shared_ptr<Tile>, 5>, 5 > room_map_;
    bool revealed_;
    const int row_;
    const int col_;

  public:
    Room(char room_id, const std::string& room_info_str, int row_, int column);
    void setRevealed(const bool revealed) { revealed_ = revealed; }
    char getRoomId() const { return room_id_; }
    std::array<std::array<std::shared_ptr<Tile>, 5> ,5> getRoomMap() { return room_map_; }
    bool isRevealed() const { return revealed_; }
    void setTileToPassage(const std::shared_ptr<Tile>& tile);
    int getRow() const { return row_; }
    int getColumn() const { return col_; }
    std::string getLineOfRoom(int index, std::string tile, TileType type, int row, int column);

    Room(const Room&) = delete;
    Room& operator=(const Room&) = delete;
    ~Room() = default;

};


#endif
