#include <boost/mp11.hpp>
#include <boost/describe/enumerators.hpp>
#include <iostream>
#include <vector>

#include <boost/describe.hpp>
#include <boost/lexical_cast.hpp>

namespace utils
{
    namespace enum_helpers
    {
        [[noreturn]] void throw_invalid_name(char const *name, char const *type);

        template <typename E>
        E string_to_enum(const char* name)
        {
            bool found = false;
            E r = {};

            auto conversion_closure = [&](auto D)
            {
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
        char const* enum_to_string(E e)
        {
            char const *r = "(unnamed)";

            auto format_closure = [&](auto D)
            {
                if (e == D.value)
                r = D.name;
            };

            boost::mp11::mp_for_each<boost::describe::describe_enumerators<E>>(format_closure);
            return r;
        }
    };
    
    namespace vector_helper
    {
        template <typename T>
        std::ostringstream get_stream(const std::vector<T> &vector)
        {
            std::ostringstream output;
            output << "[" << *vector.begin();
            for (auto it = vector.begin() + 1; it != vector.end(); ++it)
            {
                output << ", " << *it;
            }
            output << "]";
            return output;
        };
    };
};