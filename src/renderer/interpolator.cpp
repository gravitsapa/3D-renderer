#include <interpolator.h>
#include <texture.h>

namespace project {
namespace kernel {

WeightsFinder::WeightsFinder(const geometry::Point2d& a, const geometry::Point2d& b,
                             const geometry::Point2d& c)
    : a_(a) {
    if (std::abs((b - a).cross(c - a)) < epsilon_) {
        degenerate_ = true;
    }
    geometry::Matrix2d ab_ac_matrix;
    ab_ac_matrix.col(0) = b - a;
    ab_ac_matrix.col(1) = c - a;
    ab_ac_matrix_inv_ = ab_ac_matrix.inverse();
}

bool WeightsFinder::IsDegenerate() {
    return degenerate_;
}

VertexWeights WeightsFinder::FindBarycentricCoordinates(const geometry::Point2d& p) {
    // if (degenerate_) return {1, 0, 0};
    geometry::Vector2d solution = ab_ac_matrix_inv_ * (p - a_);
    return VertexWeights{.a = 1 - solution.x() - solution.y(), solution.x(), solution.y()};
}

geometry::Coordinate InterpolateCoordinate(const VertexWeights& weights,
                                           const geometry::Coordinate& a,
                                           const geometry::Coordinate& b,
                                           const geometry::Coordinate& c) {
    return a * weights.a + b * weights.b + c * weights.c;
}

geometry::Vector3d InterpolateNormals(const VertexWeights& weights, const geometry::Vector3d& a,
                                      const geometry::Vector3d& b, const geometry::Vector3d& c) {
    return geometry::Normalized(a) * weights.a + geometry::Normalized(b) * weights.b +
           geometry::Normalized(c) * weights.c;
}

geometry::Point3d InterpolatePoints(const VertexWeights& weights, const geometry::Point3d& a,
                                    const geometry::Point3d& b, const geometry::Point3d& c) {
    return a * weights.a + b * weights.b + c * weights.c;
}

TextureCoordinates InterpolateTextureCoordinates(const VertexWeights& weights,
                                                 const TextureCoordinates& a,
                                                 const TextureCoordinates& b,
                                                 const TextureCoordinates& c) {
    return TextureCoordinates{.h = a.h * weights.a + b.h * weights.b + c.h * weights.c,
                              .w = a.w * weights.a + b.w * weights.b + c.w * weights.c};
}

}  // namespace kernel
}  // namespace project
