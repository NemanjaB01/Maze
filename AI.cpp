#include "AI.hpp"

AI& AI::getInstance()
{
  static AI instance{};
  return instance;
}

void AI::play()
{}