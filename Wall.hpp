#ifndef WALL_HPP
#define WALL_HPP


#include "Tile.hpp"
#include <string>
#include "Room.hpp"

class Wall : public Tile
{
  private:
    const char room_id_;
  public:
    Wall(char room_id) : Tile{TileType::WALL,row_,column_ ,false, false, false},room_id_{room_id}{}
    std::string getTileString() override;
    Wall(const Wall&) = delete;
    ~Wall() = default;
};


#endif