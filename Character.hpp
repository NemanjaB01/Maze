#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <memory>

enum class CharacterType { FIGHTER, THIEF, SEER };

class Tile;

class Character
{
  private:
    CharacterType type_;
    bool on_button_;
    std::weak_ptr<Tile> current_tile_;
    bool on_loot_;

  public:
    Character() = default;
    Character(const Character&) = delete;
    Character(CharacterType type) : type_{type} ,on_button_{false}, on_loot_{false} {}
    Character operator=(const Character&) = delete;
    ~Character() = default;

    CharacterType getCharacterType() const { return type_; }
    void setCharacterType(const CharacterType& type) { type_ = type; }
    char getCharacterTypeAsChar() const;
    bool ifOnButton() const { return on_button_; }
    void setOnButton(bool on_button) { on_button_ = on_button; }

    std::weak_ptr<Tile>& getCurrentile() { return current_tile_; }
    void setCurrentTIle( std::shared_ptr<Tile> current_tile) { current_tile_ = current_tile; }
    void setOnLoot(bool on_loot) { on_loot_ = on_loot; }
    bool isOnLoot() { return on_loot_; }
};

#endif