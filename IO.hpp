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
};

#endif // IO_HPP