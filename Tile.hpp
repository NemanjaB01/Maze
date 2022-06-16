//---------------------------------------------------------------------------------------------------------------------
// Tile.hpp
//
// Responsible for storing tile data.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//


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
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor initializes the members with the parameters values and set is_available_ and is_passable_ 
    /// on false.
    ///
    /// @param TileType Initial value for the member type_
    /// @param char Initial value for the member inside_room_id_
    /// @param int Initial value for the member row_
    /// @param int Initial value for the member column_
    /// @param bool Initial value for the member is_magic_
    //
    Tile(TileType type, char room_id,int row, int column, bool is_magic);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks tiletype and depending on it finds corresponding string.
    //
    /// @return tile as string
    //
    virtual std::string getTileString() const = 0;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member type_.
    ///
    /// @return type_
    //
    TileType getTileType() const { return type_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member inside_room_id_.
    ///
    /// @return inside_room_id_
    //
    char getInsideRoomId() const { return inside_room_id_; }

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
    /// @return column_
    //
    int getColumn() const { return column_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member is_magic_.
    ///
    /// @return is_magic__
    //
    bool ifMagic() const { return is_magic_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member is_available_.
    ///
    /// @return is_available_ 
    //
    bool ifAvailable() const { return is_available_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member is_passable_.
    ///
    /// @return is_passable_
    //
    bool ifPassable() const { return is_passable_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy constructor deleted.
    //
    Tile(const Tile&) = default;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy operator deleted.
    //
    Tile& operator=(const Tile& other) = delete;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default virtual destructor.
    //
    virtual ~Tile() noexcept = default;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for member character_.
    ///
    /// @param character - the character to be set on tile
    //
    void setCharacter(const std::shared_ptr<Character>& character);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for member room_id_.
    ///
    /// @param room_id - the room_id to be assigned
    //
    void setInRoom(const char room_id) { inside_room_id_ = room_id; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for member is_available_.
    ///
    /// @param available - boolean value to be assigned 
    //
    void setAvailable(bool available) { is_available_ = available; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if tile contains character.
    ///
    /// @return true or false
    //
    bool ifContainsCharacter() const;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks the row of Tile and whether the room is revealed to find valid line.
    //
    /// @param index row of Tile
    /// @param is_room_revealed boolean value to be checked
    ///
    /// @return line of Tile
    //
    std::string getLineOfTile(int index, bool is_room_revealed);
};

#endif
