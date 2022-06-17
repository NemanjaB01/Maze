#ifndef CharacterAI_HPP
#define CharacterAI_HPP

#include "Character.hpp"
#include "Tile.hpp"
#include <memory>

enum class PRIORITY { LOOT, FIGHT, LEAVE_TILE, REVEAL, BUTTON , UNLOCK };

class CharacterAI
{
  private:
    std::shared_ptr<Character> character_;
    std::weak_ptr<Tile> goal_tile_;
    PRIORITY priority_;

  public:
    CharacterAI(const std::shared_ptr<Character>& character)
     : character_{character}, priority_{PRIORITY::LOOT} {}

    std::shared_ptr<Character> getCharacter() const { return character_; }
    std::weak_ptr<Tile> getTile() const { return goal_tile_; }
    PRIORITY getPriority() const { return priority_; }

    void setTile(const std::shared_ptr<Tile>& tile) { goal_tile_ = tile; }
    void setPriority(PRIORITY priority) { priority_ = priority; }
};

#endif
