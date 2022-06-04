#include "IO.hpp"
#include <string>

const std::string IO::HORIZONTAL_FRAME{"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
                                       "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
                                       "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"};

const std::string IO::WALL_STRING{"\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                                  "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"
                                  "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n"};

const std::string IO::NOT_KNOWN_COMMAND_MSG{"Please enter a KNOWN COMMAND to not confuse your treasure hunters!"};

const std::string IO::NUMBER_PARAMETERS_MSG{"Please enter a correct NUMBER OF PARAMETERS to not confuse "
                                        "your treasure hunters!"};
const std::string IO::HELP{ "Commands:\n - help\n    Prints this help text.\n\n"
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

const std::string IO::SCRY_NOT_KNOWN_START_ROOM_MSG{": \"I don't understand which room I should scry!\""};
