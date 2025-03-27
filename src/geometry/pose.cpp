#include <pose.h>

namespace geometry {

Rotation Rotation::ByAngles(const Coordinate& alpha, const Coordinate& beta,
                            const Coordinate& gamma) {
    Coordinate cos_alpha = std::cos(alpha);
    Coordinate sin_alpha = std::sin(alpha);
    Coordinate cos_beta = std::cos(beta);
    Coordinate sin_beta = std::sin(beta);
    Coordinate cos_gamma = std::cos(gamma);
    Coordinate sin_gamma = std::sin(gamma);

    return {
        Matrix3d{{cos_alpha * cos_beta, cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma,
                  cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma},
                 {sin_alpha * cos_beta, sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma,
                  sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma},
                 {-sin_beta, cos_beta * sin_gamma, cos_beta * cos_gamma}}};
}

Pose::Pose(const Position& pos, const Rotation& rot) : Position(pos), Rotation(rot) {
}

}  // namespace geometry
