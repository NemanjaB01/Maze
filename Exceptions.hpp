#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <exception>

namespace Exceptions
{
  class InvalidConfiguration : public std::exception
  {
    public:
      const char* what() const noexcept override { return "Error: Invalid configuration!\n"; }
  };

  class BadAlloc : public std::bad_alloc
  {
    public:
      const char* what() const noexcept override { return "Error: Not enough memory!\n"; }
  };
}

#endif // EXCEPTIONS