#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include "IO.hpp"

namespace Exceptions
{
  class InvalidConfiguration : public std::exception
  {
    public:
      const char* what() const noexcept override { return IO::INVALID_CONFIGURATION_MSG; }
  };

  class BadAlloc : public std::bad_alloc
  {
    public:
      const char* what() const noexcept override { return IO::BAD_ALLOC_MSG; }
  };
}

#endif // EXCEPTIONS_HPP