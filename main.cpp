#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
// #include <boost/lexical_cast.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "command_info.h"
#include "utils.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    // const char *debug_args[] = {
    //     "app_path",
    //     "-i",
    //     "/Users/dimazava/Developer/cpp_sr_11/include_dir",
    //     "/Users/dimazava/Developer/cpp_sr_11/include_dir2",
    //     "-e",
    //     "/Users/dimazava/Developer/cpp_sr_11/include_dir/include_dir_lvl1/exclude_dir",
    //     "-d",
    //     "1",
    //     "-s",
    //     "16",
    //     "-m",
    //     "_allow",
    //     "-c",
    //     "16",
    //     "-a",
    //     "crc32"};
    // std::vector<std::string> args(debug_args + 1, debug_args + std::size(debug_args));

    std::vector<std::string> args(argv + 1, argv + argc);

    // factory thing just to demonstrate boost usage
    // std::map<std::string, boost::function<command::base_command *()>> factories;
    // actories["command_info"] = boost::factory<command::command_info *>();
    // command::command_info *command_info = static_cast<class command::command_info *>(factories["command_info"]());

    command::command_info *command_info = new command::command_info{};
    command_info->fill_command(args);
    command_info->process();

    return 0;
}
