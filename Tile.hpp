#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <string>
#include "Character.hpp"
#include <memory>


class Tile
{
  protected:
    std::shared_ptr <Character> charachter_;
    int row_;
    int column_;
    bool isFieldAvailable_;
    bool isFieldPassable_;
    bool isFieldChangeable_;
    char initial_;

  public:
    virtual std::string getTileString() = 0;
    Tile(const Tile&) = delete;
    virtual ~Tile(){}
    Tile(char initial,bool isFieldAvailable, bool isFieldPassable, bool isFieldChangeable) :
    initial_{initial},
    isFieldAvailable_{isFieldAvailable},
    isFieldPassable_{isFieldPassable},
    isFieldChangeable_{isFieldChangeable}{}

};


#endif