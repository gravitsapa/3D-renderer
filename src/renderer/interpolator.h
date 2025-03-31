#pragma once

#include <point.h>
#include <texture.h>
#include <raster_coordinates.h>

namespace project {
namespace kernel {
    
struct InformationToInterpolate {
    geometry::Coordinate z_coord_in_camera_view;
    geometry::Coordinate z_coord_inv;
    geometry::Vector3d normal_div_z;
    geometry::Point3d global_point_div_z;
    TextureCoordinates tex_coord_div_z;
};

using Factor = double;
struct VertexWeights {
    Factor a;
    Factor b;
    Factor c;
};

InformationToInterpolate GetWeightedInformation(const VertexWeights& weights,
                                                const InformationToInterpolate& a,
                                                const InformationToInterpolate& b,
                                                const InformationToInterpolate& c);

// class WeightsFinder {
// public:
//     WeightsFinder(const RasterPoint2d& a, const RasterPoint2d& b, const RasterPoint2d& c);

//     VertexWeights FindWeights(const RasterPoint2d& p, RasterCoordinate left_x, RasterCoordinate
//     right_x);
// private:
//     RasterPoint2d a_;
//     RasterPoint2d b_;
//     RasterPoint2d c_;
// };

}  // namespace kernel
}  // namespace project
