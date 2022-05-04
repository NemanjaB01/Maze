#include "Passage.hpp"


std::string Passage::getTileString()
{
  std::string passage = "       \n"
                        "       \n"
                        "       \n";

  if(character_)
  {
    passage.at(12) = character_->getCharacterTypeAsChar();
    is_available_ = false;
  }
  else if(!is_available_)
    is_available_ = true;

  return passage;
}