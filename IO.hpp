#ifndef IO_HPP
#define IO_HPP

#include <string>

class IO
{
  public:
    IO() = default;
    IO(const IO&) = delete;
    ~IO() = default;

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
    inline static const std::string WALL_STRING{ "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                                                 "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                                                 "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n" };
    inline static const std::string PASSAGE_STRING{ "       \n       \n       \n" };
    inline static const char* const INVALID_CONFIGURATION_MSG{ "Error: Invalid configuration!\n" };
    inline static const char* const BAD_ALLOC_MSG{ "Error: Not enough memory!\n" };
    inline static std::string FRAME = "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
                                      "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
                                      "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550";
    inline static std::string WELCOME_MSG{ "Welcome to the magical OOP1 Maze!!!" };

    inline static std::string MOVE_BLOCKED_WAY_MSG{ ": \"My way is blocked!\"" };
    inline static std::string MOVE_NOT_ENOUGH_PLACE_MSG{ ": \"There is not enough space on that tile!\"" };
    inline static std::string MOVE_SCARY_ROOM_MSG{ ": \"That room is too scary for me!\"" };
    inline static std::string MOVE_NOT_SPECIFIED_CHARACTER_MSG{ "Who do you want to move?!?" };
    inline static std::string MOVE_NOT_NOT_KNOWN_DIRECTION_MSG{ ": \"I don't understand where I should go!\"" };
    inline static std::string MOVE_WRONG_DISTANCE_MSG{ ": \"I don't understand how far I should go!\"" };
    inline static std::string MOVE_WRONG_DIRECTION_MSG{ ": \"I can't go that way right now!\"" };

    inline static std::string UNLOCK_NO_DOORS_MSG{ ": \"Nothing to unlock here!\"" };
    inline static std::string FIGHT_NO_MOSTERS_MSG{ ": \"Nothing to fight here!\"" };

    inline static std::string SCRY_ALREADY_KNOW_ROOM_MSG{": \"We already know that room...\""};
    inline static std::string SCRY_NO_ROOM_MSG{": \"There is no room I can reveal at this position!\""};
    inline static std::string SCRY_NOT_KNOWN_START_ROOM_MSG{": \"I don't understand which room I should scry!\""};
    inline static std::string SCRY_NO_CRISTAL_BALL_MSG{ ": \"I can't scry without my magic crystal ball!\"" };

    inline static std::string NOT_KNOWN_COMMAND_MSG{"Please enter a KNOWN COMMAND to not confuse your treasure "
                                                     "hunters!"};
    inline static std::string NUMBER_PARAMETERS_MSG{"Please enter a correct NUMBER OF PARAMETERS to not confuse "
                                                     "your treasure hunters!"};

    inline static std::string HELP{ "Commands:\n - help\n    Prints this help text.\n\n"
                                    " - quit\n    Terminates the game.\n\n"
                                    " - map\n    Activates or deactivates the map.\n\n"
                                    " - flip\n    Changes the possible move direction.\n\n"
                                    " - move <CHARACTER> <DIRECTION> <DISTANCE>\n"
                                    "    Moves a character.\n"
                                    "    <CHARACTER>: the character to move, T/F/S\n"
                                    "    <DIRECTION>: the direction to move, up/down/left/right\n"
                                    "    <DISTANCE>: optional, the distance to move\n\n"
                                    " - unlock\n    Unlocks a nearby door.\n\n"
                                    " - fight\n    Fights a nearby monster.\n\n"
                                    " - scry <KNOWN_ROOM> <DIRECTION>\n"
                                    "    Reveals an unknown room.\n"
                                    "    <KNOWN_ROOM>: where to scry from\n"
                                    "    <DIRECTION>: which direction to scry\n" };

};

#endif // IO_HPP