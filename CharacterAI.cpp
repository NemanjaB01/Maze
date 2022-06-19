#include "CharacterAI.hpp"
#include "Game.hpp"
#include "Room.hpp"
#include "AI.hpp"

CharacterAI::CharacterAI(const std::shared_ptr<Character>& character)
  : Character(character->getCharacterType()), priority_{PRIORITY::NONE}
{
  updateCurrentTile(character);
}

CharacterAI::CharacterAI(const CharacterAI& copy)
 : Character(copy.getCharacterType()), priority_{copy.priority_}
{
  this->current_tile_ = copy.current_tile_.lock();
  this->goal_tile_ = copy.goal_tile_;
}

void CharacterAI::updateCurrentTile(const std::shared_ptr<Character>& character)
{
  std::shared_ptr<Tile> tile{ character->getCurrentile().lock() };
  std::shared_ptr<Room> tile_room{ MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId()) };

  const int row{ tile->getRow() + tile_room->getRow() * 5 };
  const int col{ tile->getColumn() + tile_room->getColumn() * 5 };

  current_tile_ = AI::getInstance().getGameboard().at(row).at(col);
  current_tile_.lock()->setCharacter(AI::getInstance().getCharacterAIById(character->getCharacterTypeAsChar()));
  current_tile_.lock()->setAvailable(false);
}
