#ifndef ROOM_HPP
#define ROOM_HPP

#include "Tile.hpp"

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <memory>
#include <vector>

class MagicTile;

const std::map<char, TileType> TILE_TYPE_MAP{
  {'W', TileType::WALL}, {'P', TileType::PASSAGE}, {'X', TileType::SECRET_DOOR}, {'M', TileType::MONSTER}, 
  {'G', TileType::HOURGLASS}, {'C', TileType::CRYSTAL_BALL}, {'H', TileType::HORIZONTAL_DOOR}, 
  {'V', TileType::VERTICAL_DOOR}, {'L', TileType::LOOT}, {'T', TileType::THIEF_BUTTON},
  {'F', TileType::FIGHTER_BUTTON}, {'S', TileType::SEER_BUTTON} };

const int NUMBER_LINES_IN_ROOM = 15;

class Room
{
  private:
    const char room_id_;
    std::array<std::array<std::shared_ptr<Tile>, 5>, 5 > room_map_;
    bool revealed_;
    const int row_;
    const int col_;
    int num_of_monsters_;
    std::vector<std::shared_ptr<MagicTile>> secret_doors_;

  public:
    Room(char room_id, const std::string& room_info_str, int row_, int column);
    void reveal() { revealed_ = true; }
    char getRoomId() const { return room_id_; }
    std::array<std::array<std::shared_ptr<Tile>, 5> ,5> getRoomMap() const { return room_map_; }
    bool isRevealed() const { return revealed_; }
    void setTileToPassage(const int& tile_row, const int& tile_column);
    int getRow() const { return row_; }
    int getColumn() const { return col_; }
    int getNumOfMonsters() const { return num_of_monsters_; }
    void decreaseNumMonsters() { num_of_monsters_--; }
    std::vector<std::shared_ptr<MagicTile>> getSecretDoors() const { return secret_doors_; }

    Room(const Room&) = delete;
    Room& operator=(const Room&) = delete;
    ~Room() noexcept = default;

};


#endif
