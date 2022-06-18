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

enum class CUT_TYPE{ HORIZONTAL, VERTICAL, BOTH, NONE};

struct CutPoints
{
  int x_right_;
  int x_left_;
  int y_top_;
  int y_bottom_;
  CutPoints() : x_right_{0}, x_left_{0}, y_top_{0}, y_bottom_{0} {}
};

class AI
{
  private:
    AI() = default;
    std::vector<std::vector<std::shared_ptr<Tile> >> gameboard_;
    std::array<std::shared_ptr<CharacterAI>, 3> characters_;

    void determineHighPriorities();
    void giveGoalsToCharacters();
    bool ifGoalCorrespondsToPriority(const std::shared_ptr<CharacterAI>& character);
    bool ifTileReveals(const std::shared_ptr<Tile>& tile);

    bool checkIfCutsInPossibleDirection(const CUT_TYPE& cut, const int& total_iterations);
    bool checkTilesWayForAvailability(const std::shared_ptr<CharacterAI>& character, CUT_TYPE cut,
        bool& currently_best_way);
    bool checkIfPossibleCut(const std::shared_ptr<CharacterAI>& character, CUT_TYPE& cut, bool& currently_best_way);
    void leaveTileIfPossible(std::shared_ptr<CharacterAI>& character);
    void countFreeSpaceInCertainDirection(const std::shared_ptr<CharacterAI>& character,
        MagicMaze::DIRECTIONS direction, int& free_space);
    void checkForBestLeave(std::shared_ptr<CharacterAI>& character, int& other_free_spaces, const int& free_space);

    void getMaxPoint(int& max_cut_points, const std::shared_ptr<Tile>& goal_tile, const MagicMaze::DIRECTIONS& dir);
    void getMaxCutPoints(CutPoints& max_cut_points, const std::shared_ptr<Tile>& goal_tile);
    bool checkIfCuts(CUT_TYPE& cut, const std::shared_ptr<CharacterAI>& character);

    void findGoalTile(const std::shared_ptr<CharacterAI>& character);
    void collectNeighborTiles(std::queue<std::shared_ptr<Tile>>& tiles, std::vector<std::vector<bool>>& visited);
    void checkPriority(const std::shared_ptr<CharacterAI> character, bool& goal_found,
                       const std::shared_ptr<Tile>& current_tile);

    void printCommand(const std::vector<std::string>& command_input) const noexcept;
    void playNextMove(std::shared_ptr<CharacterAI>& character);
    void callMove(std::shared_ptr<CharacterAI>& character, CUT_TYPE& cut, std::shared_ptr<Tile>& q_finder);

  public:
    static AI& getInstance();
    void copyGameboard();
    void copySpecificTile(std::shared_ptr<Tile>& tile);
    void copySpecificTile(std::shared_ptr<Tile>& tile, const std::shared_ptr<Character>& character);
    void play();

    std::vector<std::vector<std::shared_ptr<Tile> >> getGameboard() const { return gameboard_; }
    std::shared_ptr<CharacterAI> getCharacterAIById(const char& id);

    AI(AI&) = delete;
    AI& operator=(const AI&) = delete;
};

#endif