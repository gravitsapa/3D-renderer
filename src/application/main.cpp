#include <application.h>

int main(int argc, char** argv) {
    application::Application app;
    app.Run(argv[1]);
    return 0;
}
