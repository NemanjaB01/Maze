#include "PassageChar.hpp"
#include "Character.hpp"

std::string PassageChar::getTileString() const
{
  Character character;
  char c = character.getCharacterTypeAsChar();
  std::string passage_char = "       \n   ";
  passage_char.push_back(c);
  passage_char += "   \n       \n";

  return passage_char;
}