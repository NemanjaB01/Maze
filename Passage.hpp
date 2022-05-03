#ifndef PASSAGE_HPP
#define PASSAGE_HPP


#include "Tile.hpp"
#include <string>

class Passage : public Tile
{
  public:
    Passage() : Tile{TileType::PASSAGE,row_,column_,true, true, true}{}
    std::string getTileString() override;
    Passage(const Passage&) = delete;
    ~Passage() = default;
};


#endif