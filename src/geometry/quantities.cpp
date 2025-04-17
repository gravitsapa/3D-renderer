#include <quantities.h>

namespace project {
namespace geometry {

bool Equal(Coordinate lhs, Coordinate rhs) {
    return std::abs(lhs - rhs) < eps;
}

bool GreaterOrEqual(Coordinate lhs, Coordinate rhs) {
    return lhs > rhs - eps;
}

bool Greater(Coordinate lhs, Coordinate rhs) {
    return lhs > rhs + eps;
}

bool LessOrEqual(Coordinate lhs, Coordinate rhs) {
    return lhs < rhs + eps;
}

bool Less(Coordinate lhs, Coordinate rhs) {
    return lhs < rhs - eps;
}

}  // namespace geometry
}  // namespace project
