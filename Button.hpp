#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "MagicTile.hpp"

class Button : public MagicTile
{
  private:
    const char letter_;

  public:
    Button(TileType type, int row, int column, const char letter)
     : MagicTile{type, row, column, true, true}, letter_{letter} {}
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    ~Button() override = default;

    std::string getTileString() override;
};

#endif