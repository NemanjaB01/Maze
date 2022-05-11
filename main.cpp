#include "Game.hpp"
#include "Exceptions.hpp"
#include <exception>
#include <iostream>

enum class ReturnValues { OK, MEMORY_PROBLEM, INVALID_CONFIGURATION };

int main(int argc, char* argv[])
{
  try
  {
    Game::getInstance().parse(argc, argv);
    Game::getInstance().startTheGame();
  }
  catch(Exceptions::InvalidConfiguration& e)
  {
    std::cout << e.what();
    return (int)ReturnValues::INVALID_CONFIGURATION;
  }
  catch(Exceptions::BadAlloc& e)
  {
    std::cout << e.what();
    return (int)ReturnValues::MEMORY_PROBLEM;
  }
  catch(Exceptions::EndOfFile& e)
  {
    return (int)ReturnValues::OK;
  }

  return (int)ReturnValues::OK;
}
