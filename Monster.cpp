#include "Monster.hpp"
#include "Tile.hpp"

void Monster::magicUsed()
{
  if (character_->getCharacterType() == CharacterType::FIGHTER)
    magic_used_ = true;
}
