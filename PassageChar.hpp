#ifndef PASSAGECHAR_HPP
#define PASSAGECHAR_HPP

#include"Tile.hpp"
#include<string>
#include "Character.hpp"


class PassageChar : public Tile
{
  public:
    PassageChar() : Tile{TileType::PASSAGE,row_,column_,false, true, true}{}
    std::string getTileString()const override;
    PassageChar(const PassageChar&) = delete;
    ~PassageChar() = default;
};



#endif