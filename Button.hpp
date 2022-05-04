#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "MagicTile.hpp"

class Button : public MagicTile
{
  private:
    char letter_;

  public:
    Button(char letter, int row, int column)
     : MagicTile{row_, column_, true, true}, letter_{letter} {}
    std::string getTileString() override;
    void setLetter(char letter) { letter_ = letter; }
    char getLetter() const { return letter_; }
    TileType getButtonType();
};

#endif