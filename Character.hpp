#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

enum class CharacterType { FIGHTER, THIEF, SEER };

class Character
{
  private:
    CharacterType type_;

  public:
    Character(const Character&) = delete;
    Character(CharacterType type) : type_{type} {}
    Character operator=(const Character&) = delete;
    ~Character() = default;

    CharacterType getCharacterType() const { return type_; }
    void setCharacterType(const CharacterType& type) { type_ = type; }
    char getCharacterTypeAsChar() const;
};

#endif