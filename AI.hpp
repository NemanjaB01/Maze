#ifndef AI_HPP
#define AI_HPP

#include <vector>
#include <array>
#include "Tile.hpp"
#include "CharacterAI.hpp"

struct TileComplex
{
  const int row_;
  const int col_;
  std::shared_ptr<Tile> tile_;
  TileComplex(int row, int col, const std::shared_ptr<Tile>& tile);
};

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
    std::vector<std::vector<std::shared_ptr<TileComplex> >> gameboard_;
    std::array<std::shared_ptr<CharacterAI>, 3> characters_;

  public:
    static AI& getInstance();
    void copyGameboard();
    void copySpecificTile(std::shared_ptr<Tile>& tile);
    void play();

    AI(AI&) = delete;
    AI& operator=(AI&) = delete;
};

#endif