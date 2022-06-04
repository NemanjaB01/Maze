//---------------------------------------------------------------------------------------------------------------------
// Exceptions.hpp
//
// Contains own exception classes.
//
// Author: 11837414, 12037057, 12038719
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include "IO.hpp"

namespace Exceptions
{
  class InvalidConfiguration : public std::exception
  {
    public:
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Overrides what method of the exception class.
      ///
      /// @return Invalid configuration error message
      //
      const char* what() const noexcept override { return IO::INVALID_CONFIGURATION_MSG; }
  };

  class BadAlloc : public std::bad_alloc
  {
    public:
      //----------------------------------------------------------------------------------------------------------------
      ///
      /// Overrides what method of the bad_alloc class.
      ///
      /// @return Bad alloc error message
      //
      const char* what() const noexcept override { return IO::BAD_ALLOC_MSG; }
  };
}

#endif // EXCEPTIONS_HPP
