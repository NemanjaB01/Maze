#include "Character.hpp"

char Character::getCharacterTypeAsChar() const
{
  if (type_ == CharacterType::FIGHTER)
    return 'F';
  else if (type_ == CharacterType::SEER)
    return 'S';
  else if (type_ == CharacterType::THIEF)
    return 'T';
  return ' ';
}

std::string Character::getFullName()
{
  std::string full_name;
  if(type_ == CharacterType::FIGHTER)
    full_name = "Fighter";
  else if (type_ == CharacterType::SEER)
    full_name = "Seer";
  else
    full_name = "Thief";

  return full_name;
}
