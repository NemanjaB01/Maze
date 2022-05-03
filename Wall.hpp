#ifndef WALL_HPP
#define WALL_HPP


#include "Tile.hpp"
#include <string>
#include "Room.hpp"

class Wall : public Tile
{

  public:
    Wall() : Tile{TileType::WALL,row_,column_ ,false, false, false}{}
    std::string getTileString()const override;
    Wall(const Wall&) = delete;
    ~Wall() = default;
};


#endif