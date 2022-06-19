#ifndef CharacterAI_HPP
#define CharacterAI_HPP

#include "Character.hpp"
#include "Tile.hpp"
#include <memory>

enum class PRIORITY { LOOT, FIGHT, LEAVE_TILE, REVEAL, BUTTON , UNLOCK, NONE };

class CharacterAI : public Character
{
  private:
    std::weak_ptr<Tile> goal_tile_;
    PRIORITY priority_;

  public:
    CharacterAI(const std::shared_ptr<Character>& character);

    PRIORITY getPriority() const { return priority_; }
    std::shared_ptr<Tile> getGoalTile() const { return goal_tile_.lock(); };
    bool hasGoal() const { return goal_tile_.expired() ? false : true; }

    void updateCurrentTile(const std::shared_ptr<Character>& character);
    void setGoalTile(const std::shared_ptr<Tile>& goal_tile) { this->goal_tile_ = goal_tile; }
    void setPriority(PRIORITY priority) { priority_ = priority; }

    ~CharacterAI() noexcept override = default;
    CharacterAI(const CharacterAI& copy);
    CharacterAI() = delete;
};

#endif
