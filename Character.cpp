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
    case CharacterType::NONE:
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

CharacterType Character::getCharacterTypeFromChar(const std::string& character_str)
{
  if (character_str == "F")
    return CharacterType::FIGHTER;
  else if (character_str == "S")
    return CharacterType::SEER;
  else if (character_str == "T")
    return CharacterType::THIEF;
  else
    return CharacterType::NONE;
}
