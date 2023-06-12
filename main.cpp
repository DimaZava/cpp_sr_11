#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
// #include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "command_info.h"

int main(int argc, char **argv)
{
    const char *debug_args[] = {"app_path", "-i", "tmpdir", "-e", "exctmpdir", "-d", "3",    "-s",
                                "512",      "-m", "test",   "-c", "16",        "-a", "crc32"};
    // std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<std::string> args(debug_args + 1, debug_args + std::size(debug_args));

    // factory thing just to demonstrate boost usage
    std::map<std::string, boost::function<command::base_command *()>> factories;
    factories["command_info"] = boost::factory<command::command_info *>();
    command::command_info *command_info = static_cast<class command::command_info *>(factories["command_info"]());

    command_info->fill_command(args);

    return 0;
}

