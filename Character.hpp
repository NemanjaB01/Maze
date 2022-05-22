#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <memory>
#include <map>

enum class CharacterType { FIGHTER, THIEF, SEER };

class Tile;

class Character
{
  private:
    CharacterType type_;
    std::string full_name_;
    std::weak_ptr<Tile> current_tile_;
    bool on_button_;
    bool on_loot_;

  public:
    Character(const Character&) = delete;
    Character(CharacterType type) noexcept;
    Character operator=(const Character&) = delete;
    virtual ~Character() noexcept = default;

    CharacterType getCharacterType() const { return type_; }
    char getCharacterTypeAsChar() const;
    bool ifOnButton() const { return on_button_; }
    std::string getFullName() const { return full_name_; };
    std::weak_ptr<Tile>& getCurrentile() { return current_tile_; }
    bool isOnLoot() const { return on_loot_; }

    void setOnButton(bool on_button) { on_button_ = on_button; }
    void setCurrentTile( std::shared_ptr<Tile> current_tile) { current_tile_ = current_tile; }
    void setOnLoot(bool on_loot) { on_loot_ = on_loot; }
};

#endif