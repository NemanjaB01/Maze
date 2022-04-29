///---------------------------------------------------------------------------------------------------------------------
/// RoomInfo.hpp
///
/// Authors: Tutors
///---------------------------------------------------------------------------------------------------------------------

#ifndef ROOM_INFO_HPP
#define ROOM_INFO_HPP

#include <string>
#include <array>

namespace Oop
{
  ///-------------------------------------------------------------------------------------------------------------------
  /// RoomInfo class
  /// Provides Information on the configuration of the individual Rooms
  ///
  class RoomInfo final
  {
    public:
      ///---------------------------------------------------------------------------------------------------------------
      /// Returns an instance of the RoomInfo class (Singleton pattern).
      /// Example: RoomInfo::getInstance().getRoomInfo('A')
      ///
      static RoomInfo& getInstance();

      ///---------------------------------------------------------------------------------------------------------------
      /// Deleted copy constructor.
      ///
      RoomInfo(RoomInfo const&) = delete;

      ///-----------------------------------------------------------------------------------------------------------------
      /// Deleted assignment operator.
      ///
      RoomInfo& operator=(RoomInfo const&) = delete;

      ///---------------------------------------------------------------------------------------------------------------
      /// Returns the descriptive room_info_string of the room with the provided room_id.
      std::string getRoomInfoString(char room_id);

    private:
      ///---------------------------------------------------------------------------------------------------------------
      /// Private constructor. Access to class possible only through getInstance.
      ///
      RoomInfo();

      ///---------------------------------------------------------------------------------------------------------------
      /// The provided room_info_strings
      ///
      const std::array<std::string, 26> room_info_array = {
          "WPMWWPPWPVPPGPWWWWPWSPPMW", // A
          "WWPWWWPPWWWPWPPWPPPWWWCWW", // B
          "WHWWWPPPPWPWWPPPPPWWWWPTW", // C
          "WPPPPPPWWWPPWFWWPPPWWWWWW", // D
          "WWPWWPHPVPPGPCPWPXPWWWPWW", // E
          "WWWWWWPPPWPFMPPPPPPPWWPWW", // F
          "", // G
          "", // H
          "", // I
          "", // J
          "", // K
          "WWXWWWLPLWXPPPXWLPLWWWXWW", // L (loot chamber)
          "", // M
          "", // N
          "", // O
          "", // P
          "", // Q
          "", // R
          "WWPWWWPPPWPPPPPWPPPWWWPWW", // S (start room)
          "", // T
          "", // U
          "", // V
          "", // W
          "", // X
          "", // Y
          ""  // Z
      };
  };
}

#endif