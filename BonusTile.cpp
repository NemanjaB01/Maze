#include "BonusTile.hpp"


std::string BonusTile::getTileString()
{
  std::string bonus_tile;

  switch(type_)
  {
    case TileType::CRYSTAL_BALL :
      bonus_tile = "  / \\  \n |   | \n  \\ /  \n";
      if(character_)
      {
        bonus_tile.at(12) = character_->getCharacterTypeAsChar();
        is_available_ = false;
      }
      break;
    case TileType::HOURGLASS :
      bonus_tile = " \\   / \n  | |  \n /   \\ \n";
      if(character_)
      {
        bonus_tile.at(12) = character_ ->getCharacterTypeAsChar();
        is_available_ = false;
      }
      break;
  }

  return bonus_tile;
}