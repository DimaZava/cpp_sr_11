#include <iostream>

#include "command_info.h"
#include "utils.h"

namespace command
{

std::ostream &operator<<(std::ostream &os, const base_command &command)
{
    os << "_id: " << command._id << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const command_info &command)
{
    os << "input dirs: " << utils::vector_helper::get_stream(command.input_dirs).str() << std::endl;
    os << "exclude dirs: " << utils::vector_helper::get_stream(command.exclude_dirs).str() << std::endl;
    os << "depth level: " << command.depth_level << std::endl;
    os << "min file size: " << command.min_file_size << std::endl;
    os << "file name masks: " << utils::vector_helper::get_stream(command.file_name_masks).str() << std::endl;
    os << "compare block size: " << command.compare_block_size << std::endl;
    os << "hash algorihm: " << utils::enum_helpers::enum_to_string(command.hash_algorihm) << std::endl;
    return os;
}

void command_info::fill_command(const std::vector<std::string> &args)
{
    parameter current_parameter = parameter::undefined_parameter;

    for (auto i = args.begin(); i != args.end(); ++i)
    {
        bool current_parameter_changed = false;
        if (*i == "-h" || *i == "--help")
        {
            current_parameter = parameter::help;
            return;
        }
        else if (*i == "-i" || *i == "--input")
        {
            current_parameter = parameter::input_dirs;
            current_parameter_changed = true;
        }
        else if (*i == "-e" || *i == "--exclude")
        {
            current_parameter = parameter::exclude_dirs;
            current_parameter_changed = true;
        }
        else if (*i == "-d" || *i == "--depth")
        {
            current_parameter = parameter::depth_level;
            current_parameter_changed = true;
        }
        else if (*i == "-s" || *i == "--min_file_size")
        {
            current_parameter = parameter::min_file_size;
            current_parameter_changed = true;
        }
        else if (*i == "-m" || *i == "--file_name_masks")
        {
            current_parameter = parameter::file_name_masks;
            current_parameter_changed = true;
        }
        else if (*i == "-c" || *i == "--compare_block_size")
        {
            current_parameter = parameter::compare_block_size;
            current_parameter_changed = true;
        }
        else if (*i == "-a" || *i == "--hash_algorithm")
        {
            current_parameter = parameter::hash_algorithm;
            current_parameter_changed = true;
        }

        if (current_parameter_changed)
            continue;

        const std::string argument = *i;

        switch (current_parameter)
        {
        case parameter::input_dirs:
            this->input_dirs.push_back(argument);
            break;
        case parameter::exclude_dirs:
            this->exclude_dirs.push_back(argument);
            break;
        case parameter::depth_level:
            try
            {
                size_t depth_level = boost::lexical_cast<size_t>(argument);
                this->depth_level = depth_level;
            }
            catch (boost::bad_lexical_cast const &e)
            {
                std::cout << "NaN error: depth level " << e.what() << std::endl;
            }
            break;
        case parameter::min_file_size:
            try
            {
                size_t min_file_size = boost::lexical_cast<size_t>(argument);
                this->min_file_size = min_file_size;
            }
            catch (boost::bad_lexical_cast const &e)
            {
                std::cout << "NaN error: min_file_size " << e.what() << std::endl;
            }
            break;
        case parameter::file_name_masks:
            this->file_name_masks.push_back(argument);
            break;
        case parameter::compare_block_size:
            try
            {
                size_t compare_block_size = boost::lexical_cast<size_t>(argument);
                this->compare_block_size = compare_block_size;
            }
            catch (boost::bad_lexical_cast const &e)
            {
                std::cout << "NaN error: compare_block_size " << e.what() << std::endl;
            }
            break;
        case parameter::hash_algorithm:
            this->hash_algorihm = utils::enum_helpers::string_to_enum<command::hash_algorihm>(argument.c_str());
            break;
        case parameter::help:
            std::cout << "Some help info" << std::endl;
            break;
        case parameter::undefined_parameter:
            std::string error_message = std::string("Undefined parameter, terminating ") + *i;
            throw std::runtime_error(error_message);
            ;
        }
    }

    std::cout << *this << std::endl;
}
} // namespace command