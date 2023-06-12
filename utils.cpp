#include "utils.h"
#include <boost/describe.hpp>
#include <boost/lexical_cast.hpp>

[[noreturn]] void utils::enum_helpers::throw_invalid_name(char const *name, char const *type)
{
    throw std::runtime_error(
        std::string("Invalid enumerator name '") + name + "' for enum type '" + type + "'");
}