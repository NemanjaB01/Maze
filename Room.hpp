#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <string>
#include <array>
#include "Tile.hpp"

class Room
{
  private:
    char room_id_;
    std::array<std::array<Tile*,5> ,5> room_map_;
    bool revealed_;

  public:
    Room(char room_id,const std::string room_info_str);
    void setRevealed(bool revealed){revealed_ = revealed;}
    char getRoomId(){return room_id_;}
    std::array<std::array<Tile*,5>,5> getRoomMap(){return room_map_;}
    bool isRevealed(){return revealed_;}
    Room(const Room&) = delete;
    ~Room() = default;

};


#endif 