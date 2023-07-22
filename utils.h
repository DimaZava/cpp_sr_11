#include <boost/describe/enumerators.hpp>
#include <boost/mp11.hpp>
#include <iostream>
#include <set>
#include <vector>

#include <boost/describe.hpp>
#include <boost/lexical_cast.hpp>

namespace utils {
namespace hash_helpers {
uint32_t get_crc32(const void *buffer, size_t byte_count);
std::string get_md5(const void *buffer, size_t byte_count);
} // namespace hash_helpers

namespace enum_helpers {
[[noreturn]] void throw_invalid_name(char const *name, char const *type);

template <typename E>
E string_to_enum(const char *name)
{
    bool found = false;
    E r = {};

    auto conversion_closure = [&](auto D) {
        if (!found && std::strcmp(D.name, name) == 0)
        {
            found = true;
            r = D.value;
        }
    };
    boost::mp11::mp_for_each<boost::describe::describe_enumerators<E>>(conversion_closure);

    if (found)
    {
        return r;
    }
    else
    {
        throw_invalid_name(name, typeid(E).name());
    }
}

template <typename E>
char const *enum_to_string(E e)
{
    char const *r = "(unnamed)";

    auto format_closure = [&](auto D) {
        if (e == D.value)
            r = D.name;
    };

    boost::mp11::mp_for_each<boost::describe::describe_enumerators<E>>(format_closure);
    return r;
}
}; // namespace enum_helpers

namespace vector_helper {
template <typename T>
std::ostringstream get_stream(const std::vector<T> &vector, std::string split_str = ", ")
{
    std::ostringstream output;
    output << "[" << *vector.begin();
    for (auto it = vector.begin() + 1; it != vector.end(); ++it)
    {
        output << split_str << *it;
    }
    output << "]";
    return output;
};
}; // namespace vector_helper

namespace set_helper {
template <typename T>
std::ostringstream get_stream(const std::set<T> &set, std::string split_str = ", ")
{
    std::ostringstream output;
    auto it = set.begin();
    output << "[" << *it;
    ++it;
    for (; it != set.end(); ++it)
    {
        output << split_str << *it;
    }
    output << "]";
    return output;
};
}; // namespace set_helper
}; // namespace utils