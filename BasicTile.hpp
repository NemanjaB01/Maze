//---------------------------------------------------------------------------------------------------------------------
// BasicTile.hpp
//
// Responsible for storing Basictile data.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//


#ifndef BASICTILE_HPP
#define BASICTILE_HPP

#include <string>
#include "Tile.hpp"
#include "Room.hpp"

class BasicTile : public Tile
{
  public:
   //-------------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor initializes the members with the parameters values.
    /// If TileType is passage, sets is_available_ and is_passable on true.
    /// @param type Initial value for the member type_
    /// @param int Initial value for the member row_
    /// @param int Initial value for the member column_
    //
    BasicTile(TileType type, char room_id, int row, int column);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks tiletype and depending on it finds corresponding string.
    ///
    /// @return tile as string
    //
    std::string getTileString() const override;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy constructor deleted.
    //
    BasicTile(const BasicTile&) = default;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy operator deleted.
    //
    BasicTile operator=(const BasicTile&) = delete;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default destructor.
    //
    ~BasicTile() noexcept override = default;
};


#endif