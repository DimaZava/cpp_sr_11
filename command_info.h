#include <boost/describe/enum.hpp>
#include <iostream>
#include <vector>

namespace command
{

enum parameter
{
    input_dirs,         // Directories to scan
    exclude_dirs,       // Exclude directories
    depth_level,        // Depth of scanning, number of folders, 0 for topmost scan
    min_file_size,      // Minimal file size to scan, Bytes. 1 byte is default
    file_name_masks,    // Filename masks
    compare_block_size, // Read buffer block size
    algorithm,          // Hash algorithm

    help,               // Print help
    undefined_parameter // default one
};

enum hash_algorihm
{
    undefined_algorithm,
    crc32,
    md5
};
BOOST_DESCRIBE_ENUM(hash_algorihm, crc32, md5)

class base_command
{
  private:
    uint _id = std::rand();

    friend std::ostream &operator<<(std::ostream &os, const base_command &command);
};

class command_info : public base_command
{
  public:
    std::vector<std::string> input_dirs;
    std::vector<std::string> exclude_dirs;
    size_t depth_level;
    size_t min_file_size;
    std::vector<std::string> file_name_masks;
    size_t compare_block_size;
    hash_algorihm algorithm;

    void fill_command(const std::vector<std::string> &args);

    friend std::ostream &operator<<(std::ostream &os, const command_info &command);
};
} // namespace command