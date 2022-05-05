#include "Loot.hpp"

std::string Loot::getTileString()
{
  std::string loot{"$$$$$$$\n$     $\n$$$$$$$\n"};

  if(character_)
  {
    loot.at(12) = character_->getCharacterTypeAsChar();
    is_available_ = false;
  }

  return loot;
}