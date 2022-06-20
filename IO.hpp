//---------------------------------------------------------------------------------------------------------------------
// IO.hpp
//
// Responsible for input-output things, by storing common strings, like error messages, tile strings,
// game board string etc.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef IO_HPP
#define IO_HPP

#include <string>

class IO
{
  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default IO class constructor.
    ///
    //
    IO() = default;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted copy constructor.
    ///
    //
    IO(const IO&) = delete;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Default IO class destructor.
    ///
    //
    ~IO() = default;
    inline static const std::string WHOAMI{ "Banja Luka" };

    inline static const std::string CRYSTAL_BALL_STRING{ "  / \\  \n |   | \n  \\ /  \n" };
    inline static const std::string HOURGLASS_STRING{ " \\   / \n  | |  \n /   \\ \n" };
    inline static const std::string LOOT_STRING{ "$$$$$$$\n$     $\n$$$$$$$\n" };
    inline static const std::string SEER_BUTTON_STRING{ "SSSSSSS\nS     S\nSSSSSSS\n" };
    inline static const std::string THIEF_BUTTON_STRING{ "TTTTTTT\nT     T\nTTTTTTT\n" };
    inline static const std::string FIGHTER_BUTTON_STRING{ "FFFFFFF\nF     F\nFFFFFFF\n" };
    inline static const std::string MONSTER_STRING{ "       \n   M   \n       \n" };
    inline static const std::string HORIZONTAL_DOOR_STRING{ "       \n███████\n       \n" };
    inline static const std::string VERTICAL_DOOR_STRING{ "   █   \n   █   \n   █   \n" };
    inline static const std::string SECRET_DOOR_STRING{ "███████\n███████\n███████\n" };
    inline static const std::string UNKNOWN_TILE_STRING{ "UUUUUUU\nUUUUUUU\nUUUUUUU\n" };
    inline static const std::string PASSAGE_STRING{ "       \n       \n       \n" };
    inline static const std::string WELCOME_MSG{ "Welcome to the magical OOP1 Maze!!!" };

    inline static const std::string MOVE_BLOCKED_WAY_MSG{ ": \"My way is blocked!\"" };
    inline static const std::string MOVE_NOT_ENOUGH_PLACE_MSG{ ": \"There is not enough space on that tile!\"" };
    inline static const std::string MOVE_SCARY_ROOM_MSG{ ": \"That room is too scary for me!\"" };
    inline static const std::string MOVE_NOT_SPECIFIED_CHARACTER_MSG{ "Who do you want to move?!?" };
    inline static const std::string MOVE_NOT_NOT_KNOWN_DIRECTION_MSG{ ": \"I don't understand where I should go!\"" };
    inline static const std::string MOVE_WRONG_DISTANCE_MSG{ ": \"I don't understand how far I should go!\"" };
    inline static const std::string MOVE_WRONG_DIRECTION_MSG{ ": \"I can't go that way right now!\"" };

    inline static const std::string UNLOCK_NO_DOORS_MSG{ ": \"Nothing to unlock here!\"" };
    inline static const std::string FIGHT_NO_MOSTERS_MSG{ ": \"Nothing to fight here!\"" };

    inline static const std::string SCRY_ALREADY_KNOW_ROOM_MSG{": \"We already know that room...\""};
    inline static const std::string SCRY_NO_ROOM_MSG{": \"There is no room I can reveal at this position!\""};
    inline static const std::string SCRY_NO_CRISTAL_BALL_MSG{ ": \"I can't scry without my magic crystal ball!\"" };

    inline static const std::string Y_FRAME{"\u2551"};
    inline static const std::string DOUBLE_X_Y_FRAME{"\u256C"};

    inline static const char* const INVALID_CONFIGURATION_MSG{ "Error: Invalid configuration!\n" };
    inline static const char* const BAD_ALLOC_MSG{ "Error: Not enough memory!\n" };

    static const std::string SCRY_NOT_KNOWN_START_ROOM_MSG;
    static const std::string HORIZONTAL_FRAME;
    static const std::string NOT_KNOWN_COMMAND_MSG;
    static const std::string NUMBER_PARAMETERS_MSG;
    static const std::string WALL_STRING;
    static const std::string HELP;

};

#endif // IO_HPP
