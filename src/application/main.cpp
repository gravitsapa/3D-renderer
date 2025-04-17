#include <application.h>
#include <except.h>
#include <arg_parser.h>
#include <user_manual.h>

int main(int argc, char** argv) {
    try {
        auto parsed_args = project::application::ParseArguments(argc, argv);
        if (parsed_args.could_parse) {
            project::application::Application app(project::kernel::Height(parsed_args.height),
                                                  project::kernel::Width(parsed_args.width),
                                                  parsed_args.scene_name);
            app.Run();
        } else {
            project::application::PrintUserManual();
        }
    } catch (...) {
        project::except::react();
    }
    return 0;
}
