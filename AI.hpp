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
};

class AI
{
  private:
    AI() = default;
    std::vector<std::vector<TileComplex>> gameboard_;
    std::array<std::shared_ptr<CharacterAI>, 3> characters_;

  public:
    static AI& getInstance();
    void play();

    AI(AI&) = delete;
    AI& operator=(AI&) = delete;
};

#endif