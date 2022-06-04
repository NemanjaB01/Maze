//---------------------------------------------------------------------------------------------------------------------
// main.cpp
//
// Contais main function that start the program.
//
// Author: 12038719
//---------------------------------------------------------------------------------------------------------------------
//

#include "Game.hpp"
#include "Exceptions.hpp"
#include "GameParser.hpp"
#include <exception>
#include <iostream>

enum class ReturnValues { OK, MEMORY_PROBLEM, INVALID_CONFIGURATION };

int main(int argc, char* argv[])
{
  try
  {
    GameParser::getInstance().parseRooms(argc, argv);
    MagicMaze::Game::getInstance().run();
  }
  catch(Exceptions::InvalidConfiguration& e)
  {
    std::cout << e.what();
    return static_cast<int>(ReturnValues::INVALID_CONFIGURATION);
  }
  catch(Exceptions::BadAlloc& e)
  {
    std::cout << e.what();
    return static_cast<int>(ReturnValues::MEMORY_PROBLEM);
  }

  return static_cast<int>(ReturnValues::OK);
}
