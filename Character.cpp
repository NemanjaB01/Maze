#include "Character.hpp"

char Character::getCharacterTypeAsChar() const
{
  if (type_ == CharacterType::FIGHTER)
    return 'F';
  else if (type_ == CharacterType::SEER)
    return 'S';
  else if (type_ == CharacterType::THIEF)
    return 'T';
}

