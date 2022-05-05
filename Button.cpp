#include "Button.hpp"

std::string Button::getTileString()
{
  std::string button;
  switch (letter_)
  {
  case 'S':
    button = "SSSSSSS\nS     S\nSSSSSSS\n";
    if(character_)
    {
      button.at(12) = 'S';
      is_available_ = false;
    }
    break;
  case 'T':
    button = "TTTTTTT\nT     T\nTTTTTTT";
    if(character_)
    {
      button.at(12) = 'T';
      is_available_ = false;
    }
    break;
  case 'F':
    button = "FFFFFFF\nF     F\nFFFFFFF";
    if(character_)
    {
      button.at(12) = 'F';
      is_available_ = false;
    }
  }

  return button;
}
