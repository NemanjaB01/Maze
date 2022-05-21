#include "Character.hpp"

Character::Character(CharacterType type) noexcept
  : type_{type}, on_button_{false}, on_loot_{false}
{
  switch (type)
  {
  case CharacterType::FIGHTER:
    full_name_ = "Fighter";
    break;
  case CharacterType::THIEF:
    full_name_ = "Thief";
    break;
  case CharacterType::SEER:
    full_name_ = "Seer";
    break;
  }
}

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

