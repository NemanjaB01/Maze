//---------------------------------------------------------------------------------------------------------------------
// Room.hpp
//
// Responsible for storing room data.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//


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
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor initializes the members with the parameters values and assigns the value of the room info 
    ///  string to the room map.
    ///
    /// @param char Initial value for the member inside_room_id_
    /// @param int Initial value for the member row_
    /// @param int Initial value for the member column_
    //
    Room(char room_id, const std::string& room_info_str, int row_, int column);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets member revealed_  on true.
    //
    void reveal() { revealed_ = true; }
    
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member room_id_.
    ///
    /// @return room_id_
    //
    char getRoomId() const { return room_id_; }
    
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member room_map_.
    ///
    /// @return room_map_
    //
    std::array<std::array<std::shared_ptr<Tile>, 5> ,5> getRoomMap() const { return room_map_; }
    
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member revealed_.
    ///
    /// @return revealed_
    //
    bool isRevealed() const { return revealed_; }
    
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets forwarded tile to passage and sets character on tile.
    ///
    /// @param tile_row row of tile
    /// @param tile_column column of tile
    /// @param character
    //
    void setTileToPassage(const int& tile_row, const int& tile_column, std::shared_ptr<Character> character);
    
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member row_.
    ///
    /// @return row_
    //
    int getRow() const { return row_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member column_.
    ///
    /// @return  column_
    //
    int getColumn() const { return col_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member num_of_monsters_.
    ///
    /// @return num_of_monsters_
    //
    int getNumOfMonsters() const { return num_of_monsters_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Decrease member num_of_monsters by one.
    //
    void decreaseNumMonsters() { num_of_monsters_--; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member secrect_doors_
    ///
    /// @return secrect_doors_
    //
    std::vector<std::shared_ptr<MagicTile>> getSecretDoors() const { return secret_doors_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy constructor deleted.
    //
    Room(const Room&) = default;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy operator deleted.
    //
    Room& operator=(const Room&) = delete;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default destructor.
    //
    ~Room() noexcept = default;

};


#endif
