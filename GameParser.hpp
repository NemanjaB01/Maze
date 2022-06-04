//---------------------------------------------------------------------------------------------------------------------
// GameParser.hpp
//
// Responsible for taking and validation the input by user, before and during the game.
//
// Author: 12038719
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef GAME_PARSER_HPP
#define GAME_PARSER_HPP

#include <memory>
#include "Room.hpp"
#include <vector>

#include "Game.hpp"

class GameParser
{
  private:
    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if the string, which contains room description, contains only uppercase letters. Throws if not valid.
    ///
    /// @param rooms_row_string Contains room description of tiles
    ///
    //
    void checkIfLetters(const std::string& rooms_row_string) const;

    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if in one row there are more that 5 rooms. Throws if more than 5.
    ///
    /// @param rooms_row_string Contains room description of tiles
    ///
    //
    void checkRowLength(const std::string& rooms_row_string) const;

    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if there is only one starting room. Throws if not.
    ///
    /// @param all_rooms
    ///
    //
    void containsOneStartingRoom(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms) const;

    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if every room is unique. Throws if not.
    ///
    /// @param all_rooms
    ///
    //
    void ifEveryRoomUnique(const std::vector<std::vector<std::shared_ptr<Room>> >& all_rooms,
                           const std::shared_ptr<Room>& room) const;

    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Checks the first parameter of user's input. If the parameter corresponds to one of the menu options,
    ///  the command is stored. Otherwise throws an error message.
    ///
    /// @param command_as_string Whole user input
    /// @param command stores the command based on the first parameter
    ///
    //
    void checkFirstParameter(const std::string& command_as_string, MagicMaze::COMMANDS& command) const;

    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if the number of parameters satify the specification, depending on the command. Throws an error 
    /// message, if not.
    ///
    /// @param container Whole user input
    /// @param command Command entered by user
    ///
    //
    void checkSizeOfInputParameters(const std::vector<std::string>& container,
                                    const MagicMaze::COMMANDS& command) const;

    //---------------------------------------------------------------------------------------------------------------
    ///
    /// Default GameParser class constructor.
    ///
    //
    GameParser() = default;

  public:
    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns GameParser instance.
    ///
    /// @return Class instance
    //
    static GameParser& getInstance() noexcept;

    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Parses rooms input before the start of the game. Throws if invalid configuration.
    ///
    /// @param argc Number of command-line arguments
    /// @param argv Command-line arguments
    ///
    //
    void parseRooms(int argc, const char* const argv[]);

    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Parses and checks if the user input is valid.
    ///
    /// @param container User input
    /// @param command
    /// @param if_eof Signalizes if eof occured
    ///
    //
    void parseInput(std::vector<std::string>& container, MagicMaze::COMMANDS& command, bool& if_eof);

    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if the direction, that the user entered, is valid.
    ///
    /// @param direction user input
    /// @param direction_type stores direction chosen by user
    ///
    /// @return True if direction is valid, otherwise false
    //
    bool checkDirection(const std::string& direction, MagicMaze::DIRECTIONS& direction_type) const noexcept;

    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Default IO class destructor.
    ///
    //
    ~GameParser() noexcept = default;
    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted copy constructor.
    ///
    //
    GameParser(const GameParser&) = delete;
    //---------------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted copy operator.
    ///
    //
    GameParser operator=(const GameParser&) = delete;
};

#endif // GAME_PARSER_HPP