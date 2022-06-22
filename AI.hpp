#ifndef AI_HPP
#define AI_HPP

#include <memory>
#include <queue>
#include <vector>
#include <array>
#include "Character.hpp"
#include "Tile.hpp"
#include "CharacterAI.hpp"
#include "Game.hpp"
#include <map>

enum class CUT_TYPE{ HORIZONTAL, VERTICAL, BOTH, NONE};

struct CutPoints
{
  int x_right_;
  int x_left_;
  int y_top_;
  int y_bottom_;
  CutPoints() : x_right_{0}, x_left_{0}, y_top_{0}, y_bottom_{0} {}
};

const std::array<int, 4> ROW_VALUES { -1, 0, 1, 0}; // UP  RIGHT  DOWN  LEFT
const std::array<int, 4> COL_VALUES { 0, 1, 0, -1};

class AI
{
  private:
    AI() : buttons_used_{false} {};
    std::vector<std::vector<std::shared_ptr<Tile> >> gameboard_;
    std::array<std::shared_ptr<CharacterAI>, 3> characters_;
    bool buttons_used_;


    void determineHighPriorities();
    void giveGoalToCharacter(std::shared_ptr<CharacterAI>& character);
    bool ifGoalCorrespondsToPriority(std::shared_ptr<CharacterAI>& character);
    bool ifTileReveals(const std::shared_ptr<Tile>& tile);

    bool checkIfCutsInPossibleDirection(const CUT_TYPE& cut, const int& total_iterations);
    bool checkTilesWayForAvailability(const std::shared_ptr<CharacterAI>& character, CUT_TYPE cut,
         bool& currently_best_way, int& distance);
    bool checkIfPossibleCut(const std::shared_ptr<CharacterAI>& character, CUT_TYPE& cut, bool& currently_best_way,
          int& distance);
    void countFreeSpaceInCertainDirection(const std::shared_ptr<CharacterAI>& character,
        MagicMaze::DIRECTIONS direction, int& free_space);

    void getMaxPoint(int& max_cut_points, const std::shared_ptr<Tile>& goal_tile, const MagicMaze::DIRECTIONS& dir);
    void getMaxCutPoints(CutPoints& max_cut_points, const std::shared_ptr<Tile>& goal_tile);
    bool checkIfCuts(CUT_TYPE& cut, const std::shared_ptr<CharacterAI>& character);

    void findGoalTile(const std::shared_ptr<CharacterAI>& character);
    void collectNeighborTiles(std::queue<std::shared_ptr<Tile>>& tiles, std::vector<std::vector<bool>>& visited,
         CharacterType character_type);
    void checkPriority(std::shared_ptr<CharacterAI> character, bool& goal_found,
                       const std::shared_ptr<Tile>& current_tile);

    void printCommand(const std::vector<std::string>& command_input) const noexcept;
    bool playNextMove(std::shared_ptr<CharacterAI>& character);
    void callMove(std::shared_ptr<CharacterAI>& character, const int& distance);
    void optimizePriority(std::shared_ptr<CharacterAI>& character, const std::shared_ptr<Tile>& test_tile);
    void optimizeGoals(std::shared_ptr<CharacterAI>& character, const std::shared_ptr<Tile>& test_tile);
    bool checkIfInBetterPosition(std::shared_ptr<CharacterAI>& current_character,
                std::shared_ptr<CharacterAI>& other_character, const std::shared_ptr<Tile>& test_tile);

    bool ifDirectHit(const std::shared_ptr<CharacterAI>& character);
    bool callScry();
    void getScryGoalRoom(std::shared_ptr<Room>& goal_room);
    void getScryFromRoomId(const int& goal_row, const int& goal_col, std::vector<std::string>& scry_input);
    void invertDirection(MagicMaze::DIRECTIONS& direction);
    bool checkIfPowerCouldBeUsed(std::shared_ptr<CharacterAI>& character);

    bool ifCharacterAllowsToCollectTile(CharacterType character_type, const std::shared_ptr<Tile>& tile);
    bool useCertainPower(std::shared_ptr<CharacterAI>& character, MagicMaze::COMMANDS command);
    void checkifMonsterBlocksGoalRoom(std::shared_ptr<CharacterAI>& character);
    void runCharacter(std::shared_ptr<CharacterAI>& character);

    bool ifDoorBlocksWay(const std::shared_ptr<Tile>& tile, const std::shared_ptr<Room>& room);
    bool ifDoorAtRoomsEdgeAccessable(const std::shared_ptr<Tile>& tile, const std::shared_ptr<Room>& room);

    bool checkIfCharactersBlockingWays(std::shared_ptr<CharacterAI>& character);
    void decideWhoLeavesTile(std::pair<std::shared_ptr<CharacterAI>, std::shared_ptr<CharacterAI>>& characters,
        bool& decision_made);

    void callFigherToFight();
    bool ifDoorAtRoomEdge(const std::shared_ptr<Room>& room, const std::shared_ptr<Tile>& tile);
    void checkIfCharacterPaused();

    void optimizePower(std::shared_ptr<CharacterAI>& character);
    void callThief();

  public:
    static AI& getInstance();
    void copyGameboard();
    void copySpecificTile(std::shared_ptr<Tile>& tile);
    void copySpecificTile(std::shared_ptr<Tile>& tile, const std::shared_ptr<Character>& character);
    void play();
    void callCommand(MagicMaze::COMMANDS command);

    std::vector<std::vector<std::shared_ptr<Tile> >> getGameboard() const { return gameboard_; }
    std::shared_ptr<CharacterAI> getCharacterAIById(const char& id);

    AI(AI&) = delete;
    AI& operator=(const AI&) = delete;
};

#endif