#include "except.h"
#include <stdexcept>
#include <iostream>

namespace except {
void react() {
    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "The standard exception was thrown with message: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Some not standard exception was thrown." << std::endl;
    }
}
}  // namespace except
