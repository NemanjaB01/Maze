//---------------------------------------------------------------------------------------------------------------------
// MagicTile.hpp
//
// In this class, magic fields (all tile types except wall and passage) and interaction with them are defined,
// ie their role in the game.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef MAGICTILE_HPP
#define MAGICTILE_HPP

#include "Tile.hpp"
#include "Room.hpp"

#include <string>

class MagicTile : public Tile
{
  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor initializes the members with the parameters' values.
    /// @param CharacterType Initial value for the member type_
    /// @param char Initial value for the member inside_room_id_
    /// @param int Initial value for the member row_ (row index in particular room)
    /// @param int Initial value for the member column_ (column index in particular room)
    //
    MagicTile(TileType type, char room_id ,int row, int column);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// We do not allow initializing one MagicTile object to another one.
    //
    MagicTile(const MagicTile&) = delete;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// We do not allow assigning one MagicTile object to another one.
    //
    MagicTile& operator=(const MagicTile&) = delete;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default destructor as we do not need to free dynamic memory.
    //
    ~MagicTile() noexcept override = default;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Depending on the type of tile, the corresponding tile will be printed.
    ///
    /// @return std::string the corresponding tile as string
    //
    std::string getTileString() const override;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if the magic could be used.
    ///
    /// @return true - if the magic is used, false - if unpossible to use magic
    //
    bool magicUsed() const;
};

#endif