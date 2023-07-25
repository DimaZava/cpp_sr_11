#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
// #include <boost/lexical_cast.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <chrono>

#include "command_info.h"
#include "utils.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::vector<std::string> args(debug_args + 1, debug_args + std::size(debug_args));

    // std::vector<std::string> args(argv + 1, argv + argc);

    // factory thing just to demonstrate boost usage
    // std::map<std::string, boost::function<command::base_command *()>> factories;
    // actories["command_info"] = boost::factory<command::command_info *>();
    // command::command_info *command_info = static_cast<class command::command_info *>(factories["command_info"]());

    command::command_info *command_info = new command::command_info{};
    command_info->fill_command(args);

    const auto start = std::chrono::steady_clock::now();
    command_info->process();
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << std::endl;

    return 0;
}
