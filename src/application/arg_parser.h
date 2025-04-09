#pragma once

#include <string>

namespace project {
namespace application {

struct ParsedArguments {
    bool could_parse;
    int width;
    int height;
    std::string scene_name;
};

ParsedArguments FailedToParse();

ParsedArguments ParseArguments(int argc, char **argv);

}  // namespace application
}  // namespace project
