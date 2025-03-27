#include <application.h>
#include <except.h>

int main(int argc, char** argv) {
    try {
        project::Application app;
        app.Run(argv[1]);
    } catch (...) {
        except::react();
    }
    return 0;
}
