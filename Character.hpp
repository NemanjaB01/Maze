//---------------------------------------------------------------------------------------------------------------------
// Character.hpp
//
// Characters (fighter, thief and seer), their role in the game and interaction with other elements of the game are
// implemented in this class.
//
// Authors: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <memory>
#include <map>

enum class CharacterType { FIGHTER, THIEF, SEER, NONE };

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
    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// We do not allow initializing one Character object to another one.
    //
    Character(const Character&) = default;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor initializes the members with the parameters' values.
    /// @param CharacterType Initial value for the member type_
    //
    Character(CharacterType type) noexcept;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// We do not allow assigning one Character object to another one.
    //
    Character operator=(const Character&) = delete;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default destructor as we do not need to free dynamic memory.
    //
    virtual ~Character() noexcept = default;

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member type_.
    ///
    /// @return CharacterType The member type_
    //
    CharacterType getCharacterType() const { return type_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Depending on the member type_, we get the appearance of that character type as single letter.
    ///
    /// @return char capital letter representing a given character
    //
    char getCharacterTypeAsChar() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Depending on the capital letter of the given character, we get the corresponding character type.
    ///
    /// @param character_str the capital letter
    ///
    /// @return CharacterType the corresponding character type
    //
    static CharacterType getCharacterTypeFromChar(const std::string& character_str);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member on_button_.
    ///
    /// @return bool The member on_button_.
    //
    bool ifOnButton() const { return on_button_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member full_name_.
    ///
    /// @return std::string The member full_name_
    //
    std::string getFullName() const { return full_name_; };
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member current_tile_.
    ///
    /// @return std::weak_ptr<Tile>& The member current_tile_
    //
    std::weak_ptr<Tile>& getCurrentile() { return current_tile_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for member on_loot_.
    ///
    /// @return bool The member on_loot_
    //
    bool isOnLoot() const { return on_loot_; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for member on_button_.
    ///
    /// @param on_button
    ///
    //
    void setOnButton(bool on_button) { on_button_ = on_button; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for member current_tile_.
    ///
    /// @param current_tile
    ///
    //
    void setCurrentTile(std::shared_ptr<Tile> current_tile) { current_tile_ = current_tile; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for member on_loot_.
    ///
    /// @param on_loot
    ///
    //
    void setOnLoot(bool on_loot) { on_loot_ = on_loot; }
};

#endif