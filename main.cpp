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
  }
  catch(Exceptions::InvalidConfiguration& e)
  {
    std::cout << e.what() << std::endl;
    return (int)ReturnValues::INVALID_CONFIGURATION;
  }
  catch(Exceptions::BadAlloc& e)
  {
    std::cout << e.what() << std::endl;
    return (int)ReturnValues::MEMORY_PROBLEM;
  }

  return (int)ReturnValues::OK;
}
