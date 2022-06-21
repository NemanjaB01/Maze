//---------------------------------------------------------------------------------------------------------------------
// Game.hpp
//
// Responsible for total game flow.
//
// Authors: 11837414, 12037057, 12038719
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
  enum class DIRECTIONS { UP, RIGHT, DOWN, LEFT, NONE };

  enum class COMMANDS { HELP, QUIT, MAP, FLIP, MOVE, UNLOCK, FIGHT, SCRY, PLAY, WHOAMI };

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

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// The purpose of this method is to check if neighborly tile type is the same as the one in parameters.
      ///
      /// @param tile_type to compare with neighborly tile
      /// @param row neighbors' row index in room
      /// @param column neighbors' column index in room
      /// @param container to store tiles with corresponding tile type, in order to delete them letter easier
      /// @param room neighbors's room
      //
      void checkTileType(const TileType tile_type, const int row, const int column,
       std::queue<std::shared_ptr<Tile>>& container, std::shared_ptr<Room> room);
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Using this method we want to check if tiles next to a current tile (on which is one of characters at the
      /// moment) are from some certain tile type. This method is used when fight or unlock command are entered.
      ///
      /// @param tile_type to compare with neighborly tile
      /// @param container to store tiles with corresponding tile type, in order to delete them letter easier
      /// @param current_tile on which the corresponding character is
      //
      void checkCorrespondingTiles(const TileType tile_type, std::queue<std::shared_ptr<Tile>>& container,
       std::shared_ptr<Tile> current_tile);
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// This method will be called to check for all neighborly tiles
      ///
      /// @param tile_type to compare with neighborly tile
      /// @param row neighbors' row index in room
      /// @param column neighbors' column index in room
      /// @param container to store tiles with corresponding tile type, in order to delete them letter easier
      /// @param current_room room, where we want to use 'fight' or 'scry' command
      /// @param index index for one of four directions
      //
      void checkNeighborTile(const TileType tile_type, const int row, const int column,
       std::queue<std::shared_ptr<Tile>>& container, std::shared_ptr<Room> current_room, const int index);


      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Default Game class constructor.
      ///
      //
      Game();

    public:
      ///---------------------------------------------------------------------------------------------------------------
      ///
      /// Returns an instance of the Game class (Singleton pattern).
      ///
      /// @return Game instance (Singleto pattern)
      //
      static Game& getInstance() noexcept;
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Via this method we add rooms that are in single row on the game board in member rooms_.
      ///
      /// @param rooms_row_string string that contains room IDs
      //
      void addRoomsInRow(const std::string& rooms_row_string);
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Checks if all characters are on loot tiles.
      ///
      /// @return true, if game is over (with success), otherwise false
      //
      bool endOfGame();

      bool ifAllButtonsVisible() const;

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
      /// Stores next row and column depending on the direction.
      ///
      /// @param next_row
      /// @param next_col
      /// @param dir Direction
      //
      static void changeNextRowCol(int& next_row, int& next_col, const DIRECTIONS& dir);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Checks if all characters are on their button positions.
      ///
      /// @return true, false
      //
      bool checkIfAllCharactersOnButton() const;
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Using this method we want to find corresponding room via room id.
      ///
      /// @param id room id
      ///
      /// @return room, otherwise nullptr
      //
      std::shared_ptr<Room> getRoomById(const char id) const;
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Getter for card placed on top of queue cards_.
      ///
      /// @return one of directions, in which we could move characters
      //
      DIRECTIONS getCurrentDirection() const { return cards_.front(); };
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Getter for member flips_number_.
      ///
      /// @return the member flips_number
      //
      unsigned getFlipsNumber() const { return flips_number_; }
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Based on return value of getCurrentDirection() method, we get that direction represented as string.
      ///
      /// @return current direction represented as string
      //
      static std::string getDirectionAsString(MagicMaze::DIRECTIONS direction);
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Getter for member rooms_.
      ///
      /// @return the member rooms_
      //
      std::vector<std::vector<std::shared_ptr<Room>> > getRooms() const { return rooms_; }
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Getter for member show_map_.
      ///
      /// @return the member show_map_ (true, when map is in active mode)
      //
      bool ifMapActivated() const { return show_map_; }
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Based on the character type, we get one of the corresponding characters
      ///
      /// @param type character type
      ///
      /// @return character
      //
      std::shared_ptr<Character> getCharacter(CharacterType type) const;

      std::array<std::shared_ptr<Character>, 3> getAllCharacters() const { return characters_; };

      //----------------------------------------------------------------------------------------------------------------
      //
      /// Via this method we run a game.
      ///
      //
      void run();
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Calling this method all characters will be placed on starting positions, welcome message and statting
      /// informations will be displayed.
      ///
      //
      void prepareGame();
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Via this method we print game map and displayed it to the user via stdio.
      ///
      //
      void printMap() const;
      //----------------------------------------------------------------------------------------------------------------
      //
      /// If user enters 'flip' command this method is activated and the current direction is changed to the next one.
      ///
      //
      void flip();
      //----------------------------------------------------------------------------------------------------------------
      //
      /// Setter for map.
      ///
      /// @param map_activated
      //
      void setMapActivity(bool map_activated) { show_map_ = map_activated; }
      //----------------------------------------------------------------------------------------------------------------
      //
      /// If user enters 'move' command with appropriate parameters, the character will be moved regarding on them, if
      /// parameters are valid.
      ///
      //
      void move(std::vector<std::string>& input);
      //----------------------------------------------------------------------------------------------------------------
      //
      /// If user enters 'fight' command, fighter character will fight all monsters next to him.
      ///
      //
      void fightMonster();
      //----------------------------------------------------------------------------------------------------------------
      //
      /// If user enters 'scry' command, seer character will scry unrevealed room, if parameters are valid.
      ///
      //
      void scry(std::vector<std::string>& input);
      //----------------------------------------------------------------------------------------------------------------
      //
      /// If user enters 'unlock' command, thief character will unlock all horizontal or vertical doors next to him.
      ///
      //
      void unlock();

      void playInputParsing(const std::vector<std::string>& input, std::shared_ptr<Character>& character);

      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Default destructor as we do not need to free dynamic memory.
      //
      ~Game() noexcept = default;
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// We do not allow initializing one Game object to another one.
      //
      Game(const Game& copy) = delete;
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// We do not allow assigning one Game object to another one.
      //
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
