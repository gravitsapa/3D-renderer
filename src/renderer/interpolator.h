#pragma once

#include <point.h>
#include <texture.h>

namespace project {
namespace kernel {

using Factor = double;
struct VertexWeights {
    Factor a;
    Factor b;
    Factor c;
};

class WeightsFinder {
public:
    WeightsFinder(const geometry::Point2d& a, const geometry::Point2d& b,
                  const geometry::Point2d& c);

    VertexWeights FindBarycentricCoordinates(const geometry::Point2d& p);
    bool IsDegenerate();

private:
    static constexpr geometry::Coordinate epsilon_ = 1e-5;
    bool degenerate_ = false;
    geometry::Matrix2d ab_ac_matrix_inv_;
    geometry::Point2d a_;
};

geometry::Coordinate InterpolateCoordinate(const VertexWeights& weights,
                                           const geometry::Coordinate& a,
                                           const geometry::Coordinate& b,
                                           const geometry::Coordinate& c);

geometry::Vector3d InterpolateNormals(const VertexWeights& weights, const geometry::Vector3d& a,
                                      const geometry::Vector3d& b, const geometry::Vector3d& c);

geometry::Point3d InterpolatePoints(const VertexWeights& weights, const geometry::Point3d& a,
                                    const geometry::Point3d& b, const geometry::Point3d& c);

TextureCoordinates InterpolateTextureCoordinates(const VertexWeights& weights,
                                                 const TextureCoordinates& a,
                                                 const TextureCoordinates& b,
                                                 const TextureCoordinates& c);

}  // namespace kernel
}  // namespace project
