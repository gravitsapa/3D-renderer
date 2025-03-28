#include <application.h>
#include <except.h>

int main(int argc, char** argv) {
    try {
        project::application::Application app(project::kernel::Height(400), project::kernel::Width(600));
        app.Run();
    } catch (...) {
        project::except::react();
    }
    return 0;
}
