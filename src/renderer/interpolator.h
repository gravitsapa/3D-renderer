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

}  // namespace kernel
}  // namespace project
