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
  enum class DIRECTIONS_TYPES { UP, RIGHT, DOWN, LEFT };

  enum class COMMANDS { HELP, QUIT, MAP, FLIP, MOVE, UNLOCK, FIGHT, SCRY };

  class Game
  {
    private:
      std::vector<std::vector<std::shared_ptr<Room>> > rooms_;
      std::array<std::shared_ptr<Character>, 3> characters_;
      std::queue<DIRECTIONS_TYPES> cards_;
      unsigned flips_number_;
      bool show_map_;

      void printHorizontalFrame() const;
      void shuffleCards();

      void placeCharactersOnStartingPositions();
      void stopCharacterOnTile(std::shared_ptr<Tile>& first_tile, std::shared_ptr<Room>& current_room,
                              std::shared_ptr<Tile>& current_tile, std::shared_ptr<Character>& moving_character);
      void useHourglass(std::shared_ptr<Tile>& tile);
      void moveInputParsing(std::vector<std::string>& input, std::shared_ptr<Character>& character_to_move,
                            int& distance);
      void changeNextRowCol(int& next_row, int& next_col);
      void getTilesOnTheWay(std::queue<std::shared_ptr<Tile>>& tiles_on_way,
                            const std::shared_ptr<Character>& character, const int& distance);
      void checkIfNewRoomsNeedToBeRevealed(const std::shared_ptr<Tile>& current_tile,
                                          const std::shared_ptr<Room> current_room);
      void scryInputParsing(std::vector<std::string>& input, std::shared_ptr<Room>& room_to_scry,
                            DIRECTIONS_TYPES& direction, std::shared_ptr<Character> character);
      DIRECTIONS_TYPES checkDirection(std::string direction, std::shared_ptr<Character>& character);

      Game();
    public:
      static Game& getInstance() noexcept;
      void addRoom(const std::string& rooms_row_string);

      std::shared_ptr<Room> getRoomById(const char id) const;
      DIRECTIONS_TYPES getCurrentDirection() const { return cards_.front(); };
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
      void checkCorrespondingTileType(const TileType tile_type, std::queue<std::shared_ptr<Tile>>& container,
       std::shared_ptr<Tile> current_tile);

      ~Game() noexcept = default;
      Game(const Game& copy) = delete;
      Game& operator=(const Game& game) = delete;
  };

}
#endif // GAME_HPP
