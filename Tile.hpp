#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <string>
#include "Character.hpp"


class Tile
{
  private:
    Character* charachter_;

  public:
    virtual std::string getTileString() = 0;
    Tile(const Tile&) = delete;
    virtual ~Tile(){}

};


#endif