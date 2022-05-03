#include "Character.hpp"

char Character::getCharacterTypeAsChar() const
{
  if (type_ == CharacterType::FIGHTER)
    return 'f';
  else if (type_ == CharacterType::SEER)
    return 's';
  else if (type_ == CharacterType::THIEF)
    return 't';
}

