#include <user_manual.h>

#include <iostream>

namespace project {
namespace application {
void PrintUserManual() {
    std::cout << detail::UserManual() << std::endl;
}

namespace detail {
std::string UserManual() {
    return "You must enter arguments: width height scene_name\nExample: ./3D-renderer 1920 1080 "
           "coffee\n\nPossible scene list: coffee, house, chess\n";
}

}  // namespace detail
}  // namespace application
}  // namespace project
