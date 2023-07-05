#include "utils.h"
#include <boost/describe.hpp>
#include <boost/lexical_cast.hpp>

namespace utils::enum_helpers
{
[[noreturn]] void throw_invalid_name(char const *name, char const *type)
{
    throw std::runtime_error(std::string("Invalid enumerator name '") + name + "' for enum type '" + type + "'");
}
}; // namespace utils::enum_helpers