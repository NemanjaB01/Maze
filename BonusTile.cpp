#include "BonusTile.hpp"


std::string BonusTile::getTileString()
{
  std::string bonus_tile;

  if (type_ == TileType::CRYSTAL_BALL)
  {
    bonus_tile = "  / \\  \n |   | \n  \\ /  \n";
    if(character_)
    {
      bonus_tile.at(12) = character_->getCharacterTypeAsChar();
      is_available_ =  false;
    }
    else if (!is_available_)
      is_available_ = true;
  }
  
  if (type_ == TileType::HOURGLASS)
  {
    bonus_tile = " \\   / \n  | |  \n /   \\ \n";
    if(character_)
    {
      bonus_tile.at(12) = character_ ->getCharacterTypeAsChar();
      is_available_ =  false;
    }
    else if (!is_available_)
      is_available_ = true;
  }

  return bonus_tile;
}