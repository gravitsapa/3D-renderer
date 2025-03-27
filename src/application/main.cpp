#include <application.h>
#include <except.h>

int main(int argc, char** argv) {
    try {
        project::Application app(argv[1], renderer::Height(400), renderer::Width(600));
        app.Run();
    } catch (...) {
        except::react();
    }
    return 0;
}
