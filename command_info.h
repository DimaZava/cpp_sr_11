#include <boost/describe/enum.hpp>
#include <filesystem>
#include <iostream>
#include <set>
#include <vector>
#include <random>
#include <limits>

namespace command
{

enum class parameter
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
public:
    base_command()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        constexpr decltype(_id) min = std::numeric_limits<decltype(_id)>::min();
        constexpr decltype(_id) max = std::numeric_limits<decltype(_id)>::max();

        std::uniform_int_distribution<uint> dist(min, max);
        _id = dist(gen);
    }

private:
    uint _id;

    friend std::ostream &operator<<(std::ostream &os, const base_command &command);
};

class command_info : public base_command
{
  public:
      std::set<std::string> input_dirs;
      std::set<std::string> exclude_dirs;
      size_t depth_level;
      size_t min_file_size;
      std::set<std::string> file_name_masks;
      size_t compare_block_size;
      hash_algorihm algorithm;

      void fill_command(const std::vector<std::string> args);
      void process();

      friend std::ostream &operator<<(std::ostream &os, const command_info &command);

  private:
      size_t current_depth_level = 0;

      void collect_files(std::string directory, std::vector<std::string> &collected_files);
      void iterate_files(std::string file_string_path, std::set<std::string> files_to_compare, std::set<std::filesystem::path> &duplicates);
      bool compare_files(std::ifstream &lhs, std::ifstream &rhs);
      bool compare_filechunks(std::byte *lhs, size_t lhs_size, std::byte *rhs, size_t rhs_size) const;
};
} // namespace command