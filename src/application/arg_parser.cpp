#include <arg_parser.h>

namespace project {
namespace application {

ParsedArguments FailedToParse() {
    return ParsedArguments{.could_parse = false, .width = 0, .height = 0, .scene_name = ""};
}

ParsedArguments ParseArguments(int argc, char **argv) {
    if (argc != 4) {
        return FailedToParse();
    }

    ParsedArguments parsed_arguments;
    parsed_arguments.could_parse = true;

    try {
        parsed_arguments.width = atoi(argv[1]);
        parsed_arguments.height = atoi(argv[2]);
        parsed_arguments.scene_name = argv[3];
    } catch (...) {
        return FailedToParse();
    }

    if (parsed_arguments.width <= 0 || parsed_arguments.height <= 0) {
        return FailedToParse();
    }

    return parsed_arguments;
}

}  // namespace application
}  // namespace project
