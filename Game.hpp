//---------------------------------------------------------------------------------------------------------------------
// Game.hpp
//
// Responsible for total game flow.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <array>
#include <queue>
#include <sstream>

#include "Room.hpp"
#include "Character.hpp"


namespace MagicMaze
{
  enum class DIRECTIONS { UP, RIGHT, DOWN, LEFT };

  enum class COMMANDS { HELP, QUIT, MAP, FLIP, MOVE, UNLOCK, FIGHT, SCRY };

  const int CHARACTERS_NUMBER = 3;

  class Game
  {
    private:
      std::vector<std::vector<std::shared_ptr<Room>>> rooms_;
      std::array<std::shared_ptr<Character>, 3> characters_;
      std::queue<DIRECTIONS> cards_;
      unsigned flips_number_;
      bool show_map_;

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Prints gameboard's horizontal frame.
      ///
      //
      void printHorizontalFrame() const noexcept;

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Shuffles cards, that represent direction, before the start of the game.
      ///
      //
      void shuffleCards();

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Places the characters on their starting positions, before the start of the game.
      ///
      //
      void placeCharactersOnStartingPositions();

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Places character after the moving on the tile. If tile is magic and provides some feature, helper funtions
      /// are called to use it.
      ///
      /// @param moving_character
      /// @param first_tile Tile from that the player moved
      /// @param current_tile Tile where the player finished his move
      //
      void stopCharacterOnTile(std::shared_ptr<Character>& moving_character, std::shared_ptr<Tile>& first_tile,
                               std::shared_ptr<Tile>& current_tile);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Checks if character stopped on his own button. if yes setter for character button is called.
      ///
      /// @param character
      /// @param tile
      //
      void ifCharacterStoppedOnButton(const std::shared_ptr<Character>& character, const std::shared_ptr<Tile>& tile);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Uses hourglass power, by reducing the flips by maximum 5, and minimum 0. Converts hourglass into 
      /// normal pasage.
      ///
      /// @param tile hourglass
      /// @param character character that moved to hourglass tile
      //
      void useHourglass(std::shared_ptr<Tile>& tile, std::shared_ptr<Character>& character);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Parses input for the move command. If valid parameters are stored. Otherwise an error message is thrown.
      ///
      /// @param input User input
      /// @param character_to_move
      /// @param distance
      //
      void moveInputParsing(std::vector<std::string>& input, std::shared_ptr<Character>& character_to_move,
                            int& distance);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Stores next row and column depending on the direction.
      ///
      /// @param next_row
      /// @param next_col
      /// @param dir Direction
      //
      void changeNextRowCol(int& next_row, int& next_col, const DIRECTIONS& dir) const;

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Stores tiles that are on the character's way to get to the wanted position. Throws if the tile does not
      /// exist.
      ///
      /// @param tiles_on_way
      /// @param character
      /// @param distance Distance to move
      //
      void getTilesOnTheWay(std::queue<std::shared_ptr<Tile>>& tiles_on_way,
                            const std::shared_ptr<Character>& character, const int& distance);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Reveals if new rooms need to be reveled, by checking the current player's position.
      ///
      /// @param tile Tile on that the character stopped
      //
      void checkIfNewRoomsNeedToBeRevealed(const std::shared_ptr<Tile>& current_tile);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Parses input for the scry command. If valid the parameters are stored, otherwise a corresponding error
      /// message is thrown.
      ///
      /// @param input
      /// @param room_to_scry
      /// @param character
      //
      void scryInputParsing(std::vector<std::string>& input, std::shared_ptr<Room>& room_to_scry,
                            std::shared_ptr<Character> character) const;

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Checks if all characters are on their button positions.
      ///
      /// @return true, false
      //
      bool checkIfAllCharactersOnButton() const;

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Removes all secret doors of all rooms, by converting them into normal passages.
      ///
      //
      void removeAllSecretDoors();

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Converts all buttons into normal passages.
      ///
      //
      void setAllButtonsToPassage();

      void checkCorrespondingTileType(const TileType tile_type, std::queue<std::shared_ptr<Tile>>& container,
       std::shared_ptr<Tile> current_tile);
      void checkNeighborsTile(const TileType tile_type, const int row, const int column,
       std::queue<std::shared_ptr<Tile>>& container, std::shared_ptr<Room> neighbour);
      void checkCorrespondingTiles(const TileType tile_type, std::queue<std::shared_ptr<Tile>>& container,
       std::shared_ptr<Tile> current_tile);
      void checkTileType(const TileType tile_type, const int row, const int column,
       std::queue<std::shared_ptr<Tile>>& container, std::shared_ptr<Room> current_room, const int index);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Returns neighbor's room based on row and column.
      ///
      /// @param neighbor_room_row neighbor's room row
      /// @param neighbor_room_column neighbor's room column
      ///
      /// @return neighbor room if exists, otherwise nullptr
      //
      std::shared_ptr<Room> getNeighborsRoom(int neighbor_room_row, int neighbor_room_column);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Checks if all characters are on loot tiles.
      ///
      /// @return true, if game is over (with success), otherwise false
      //
      bool endOfGame();

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Default Game class constructor.
      ///
      //
      Game();

    public:
      static Game& getInstance() noexcept;
      void addRoom(const std::string& rooms_row_string);

      std::shared_ptr<Room> getRoomById(const char id) const;
      DIRECTIONS getCurrentDirection() const { return cards_.front(); };
      unsigned getFlipsNumber() const { return flips_number_; }
      std::string getPossibleMoveAsString() const;
      std::vector<std::vector<std::shared_ptr<Room>> > getRooms() const { return rooms_; }
      bool ifMapActivated() const { return show_map_; }
      std::shared_ptr<Character> getCharacter(CharacterType type) const;

      void run();
      void prepareGame();
      void printMap() const;
      void flip();
      void setMapActivity(bool map_activated) { show_map_ = map_activated; }
      void move(std::vector<std::string>& input);
      void fightMonster();
      void scry(std::vector<std::string>& input);
      void unlock();


      ~Game() noexcept = default;
      Game(const Game& copy) = delete;
      Game& operator=(const Game& game) = delete;
  };

  //--------------------------------------------------------------------------------------------------------------------
  ///
  /// Overloads std::ostream operator for printing the whole gameboard.
  ///
  /// @param out
  /// @param game
  ///
  /// @return std::ostream
  //
  std::ostream& operator<<(std::ostream& out, const Game& game);
}
#endif // GAME_HPP
