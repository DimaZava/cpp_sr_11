#include <cstddef>
#include <fstream>
#include <iostream>
#include <zlib.h>

#include "command_info.h"
#include "utils.h"

namespace command {

std::ostream& operator<<(std::ostream& os, const base_command& command)
{
    os << "_id: " << command._id << "\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const command_info& command)
{
    os << "input dirs: " << utils::set_helper::get_stream(command.input_dirs).str() << "\n";
    os << "exclude dirs: " << utils::set_helper::get_stream(command.exclude_dirs).str() << "\n";
    os << "depth level: " << command.depth_level << "\n";
    os << "min file size: " << command.min_file_size << "\n";
    os << "file name masks: " << utils::set_helper::get_stream(command.file_name_masks).str() << "\n";
    os << "compare block size: " << command.compare_block_size << "\n";
    os << "hash algorihm: " << utils::enum_helpers::enum_to_string(command.algorithm) << "\n";
    return os;
}

void command_info::fill_command(const std::vector<std::string> args)
{
    parameter current_parameter = parameter::undefined_parameter;

    for (const auto& argument : args)
    {
        bool current_parameter_changed = false;
        if (argument == "-h" || argument == "--help")
        {
            // current_parameter = parameter::help;
            return;
        }
        else if (argument == "-i" || argument == "--input")
        {
            current_parameter = parameter::input_dirs;
            current_parameter_changed = true;
        }
        else if (argument == "-e" || argument == "--exclude")
        {
            current_parameter = parameter::exclude_dirs;
            current_parameter_changed = true;
        }
        else if (argument == "-d" || argument == "--depth")
        {
            current_parameter = parameter::depth_level;
            current_parameter_changed = true;
        }
        else if (argument == "-s" || argument == "--min_file_size")
        {
            current_parameter = parameter::min_file_size;
            current_parameter_changed = true;
        }
        else if (argument == "-m" || argument == "--file_name_masks")
        {
            current_parameter = parameter::file_name_masks;
            current_parameter_changed = true;
        }
        else if (argument == "-c" || argument == "--compare_block_size")
        {
            current_parameter = parameter::compare_block_size;
            current_parameter_changed = true;
        }
        else if (argument == "-a" || argument == "--hash_algorithm")
        {
            current_parameter = parameter::algorithm;
            current_parameter_changed = true;
        }

        if (current_parameter_changed)
            continue;

        switch (current_parameter)
        {
            case parameter::input_dirs:
                this->input_dirs.insert(argument);
                break;
            case parameter::exclude_dirs:
                this->exclude_dirs.insert(argument);
                break;
            case parameter::depth_level:
                try
                {
                    size_t depth_level = boost::lexical_cast<size_t>(argument);
                    this->depth_level = depth_level;
                }
                catch (boost::bad_lexical_cast const& e)
                {
                    std::cout << "NaN error: depth level " << e.what() << "\n";
                }
                break;
            case parameter::min_file_size:
                try
                {
                    size_t min_file_size = boost::lexical_cast<size_t>(argument);
                    this->min_file_size = min_file_size;
                }
                catch (boost::bad_lexical_cast const& e)
                {
                    std::cout << "NaN error: min_file_size " << e.what() << "\n";
                }
                break;
            case parameter::file_name_masks:
                this->file_name_masks.insert(argument);
                break;
            case parameter::compare_block_size:
                try
                {
                    size_t compare_block_size = boost::lexical_cast<size_t>(argument);
                    this->compare_block_size = compare_block_size;
                }
                catch (boost::bad_lexical_cast const& e)
                {
                    std::cout << "NaN error: compare_block_size " << e.what() << "\n";
                }
                break;
            case parameter::algorithm:
                this->algorithm = utils::enum_helpers::string_to_enum<command::hash_algorihm>(argument.c_str());
                break;
            case parameter::help:
                std::cout << "Some help info"
                          << "\n";
                break;
            case parameter::undefined_parameter:
                std::string error_message = std::string("Undefined parameter, terminating ") + argument;
                throw std::runtime_error(error_message);
        }
    }

    std::cout << *this << "\n";
}

void command_info::process()
{
    std::vector<std::string> collected_files;
    for (const auto& directory : this->input_dirs)
    {
        collect_files(directory, collected_files);
    }

    std::set<std::filesystem::path> duplicates;

    auto it = collected_files.begin();
    while (it != collected_files.end())
    {
        auto file_string_path = *it;

        if (duplicates.find(file_string_path) != duplicates.end())
        {
            ++it;
            continue;
        }

        std::set<std::string> files_to_compare = {it + 1, collected_files.end()};
        this->iterate_files(file_string_path, files_to_compare, duplicates);
        ++it;
    }
    std::cout << "Found duplicates:\n" << utils::set_helper::get_stream(duplicates).str() << "\n";
}

void command_info::collect_files(std::string directory, std::vector<std::string>& collected_files)
{
    if (this->depth_level < current_depth_level)
    {
        return;
    }
    ++current_depth_level;

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        std::cout << entry.path() << " ";
        if (entry.is_directory())
        {
            std::cout << "directory\n";
            if (this->exclude_dirs.count(entry.path()) == 0)
            {
                collect_files(entry.path(), collected_files);
            }
        }
        else
        {
            auto path_string = entry.path().filename().string();
            auto file_mask_compare_closure = [&path_string](std::string current_path) {
                return path_string.find(current_path) != std::string::npos;
            };

            if (auto it = std::find_if(file_name_masks.begin(), file_name_masks.end(), file_mask_compare_closure);
                it != file_name_masks.end())
            {
                std::cout << "file\n";
                collected_files.push_back(entry.path());
            }
            else
            {
                std::cout << "ignoring file\n";
            }
        }
    }
}

void command_info::iterate_files(
    std::string file_string_path,
    std::set<std::string> files_to_compare,
    std::set<std::filesystem::path>& duplicates)
{
    auto file_path = std::filesystem::path(file_string_path.c_str());
    auto file_size = std::filesystem::file_size(file_path);
    auto file_input = std::ifstream(file_path, std::ios::binary);

    for (const auto& file_to_compare : files_to_compare)
    {
        auto file_to_compare_path = std::filesystem::path(file_to_compare.c_str());
        auto file_to_compare_size = std::filesystem::file_size(file_to_compare_path);
        if (file_size != file_to_compare_size)
            continue;

        auto file_to_compare_input = std::ifstream(file_to_compare_path, std::ios::binary);

        if (this->compare_files(file_input, file_to_compare_input))
        {
            duplicates.insert(file_to_compare_path);
        }
        file_to_compare_input.close();
    }
    file_input.close();
}

/// @brief Compares files
/// @param lhs First file std::ifstream object to
/// compare
/// @param rhs Second file std::ifstream object to
/// compare
/// @return Returns true if files are equal,
/// othewise - false
bool command_info::compare_files(std::ifstream& lhs, std::ifstream& rhs)
{
    auto lhs_buf = std::vector<char>(compare_block_size);
    auto rhs_buf = std::vector<char>(compare_block_size);

    std::streamoff compare_block_stream_size = static_cast<std::streamoff>(compare_block_size);

    bool is_found_equal = false;
    while (true)
    {
        lhs.read(lhs_buf.data(), compare_block_stream_size);
        rhs.read(rhs_buf.data(), compare_block_stream_size);

        if (lhs.gcount() == 0 && rhs.gcount() == 0)
        {
            is_found_equal = true;
            break;
        }
        else if (!compare_filechunks(lhs_buf.data(), lhs_buf.size(), rhs_buf.data(), rhs_buf.size()))
        {
            is_found_equal = false;
            break;
        }
        lhs_buf.clear();
        rhs_buf.clear();
        lhs_buf.resize(compare_block_size);
        rhs_buf.resize(compare_block_size);
    }
    return is_found_equal;
}

bool command_info::compare_filechunks(char* lhs, size_t lhs_size, char* rhs, size_t rhs_size) const
{
    bool is_equal = false;
    switch (algorithm)
    {
        case hash_algorihm::crc32:
        {
            auto lhs_hash = utils::hash_helpers::get_crc32(lhs, sizeof(std::byte) * lhs_size);
            auto rhs_hash = utils::hash_helpers::get_crc32(rhs, sizeof(std::byte) * rhs_size);
            is_equal = lhs_hash == rhs_hash;
            break;
        }
        case hash_algorihm::md5:
        {
            auto lhs_hash = utils::hash_helpers::get_md5(lhs, sizeof(std::byte) * lhs_size);
            auto rhs_hash = utils::hash_helpers::get_md5(rhs, sizeof(std::byte) * rhs_size);
            is_equal = lhs_hash == rhs_hash;
            break;
        }
        case hash_algorihm::undefined_algorithm:
        {
            // unreachable case
            std::string error_message = std::string("Undefined algroithm, terminating ");
            throw std::runtime_error(error_message);
            break;
        }
    }
    return is_equal;
}

} // namespace command