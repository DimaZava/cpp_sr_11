#include "utils.h"
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>
#include <boost/describe.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <iterator>

namespace utils::hash_helpers {
uint32_t get_crc32(const void *buffer, size_t byte_count)
{
    boost::crc_32_type result;
    result.process_bytes(buffer, byte_count);
    return result.checksum();
}

std::string get_md5(const void *buffer, size_t byte_count)
{
    using boost::uuids::detail::md5;
    md5 hash;
    md5::digest_type digest;

    hash.process_bytes(buffer, byte_count);
    hash.get_digest(digest);

    const auto intDigest = reinterpret_cast<const unsigned int *>(&digest);
    std::string result;
    boost::algorithm::hex(intDigest, intDigest + (sizeof(md5::digest_type) / sizeof(int)), std::back_inserter(result));
    return result;
}
}; // namespace utils::hash_helpers

namespace utils::enum_helpers
{
[[noreturn]] void throw_invalid_name(char const *name, char const *type)
{
    throw std::runtime_error(std::string("Invalid enumerator name '") + name + "' for enum type '" + type + "'");
}
}; // namespace utils::enum_helpers