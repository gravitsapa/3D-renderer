#include <interpolator.h>
#include <texture.h>
#include <cassert>

namespace project {
namespace kernel {

InformationToInterpolate operator+(const InformationToInterpolate& lhs,
                                   const InformationToInterpolate& rhs) {
    return {lhs.z_coord_in_camera_view + rhs.z_coord_in_camera_view,
            lhs.z_coord_inv + rhs.z_coord_inv, lhs.normal_div_z + rhs.normal_div_z,
            lhs.global_point_div_z + rhs.global_point_div_z,
            lhs.tex_coord_div_z + rhs.tex_coord_div_z};
}

InformationToInterpolate operator-(const InformationToInterpolate& lhs,
                                   const InformationToInterpolate& rhs) {
    return {lhs.z_coord_in_camera_view - rhs.z_coord_in_camera_view,
            lhs.z_coord_inv - rhs.z_coord_inv, lhs.normal_div_z - rhs.normal_div_z,
            lhs.global_point_div_z - rhs.global_point_div_z,
            lhs.tex_coord_div_z - rhs.tex_coord_div_z};
}

InformationToInterpolate operator*(const InformationToInterpolate& lhs, double rhs) {
    return {lhs.z_coord_in_camera_view * rhs, lhs.z_coord_inv * rhs, lhs.normal_div_z * rhs,
            lhs.global_point_div_z * rhs, lhs.tex_coord_div_z * rhs};
}

InformationToInterpolate ZeroInformation() {
    return {0, 0, geometry::Vector3d::Zero(), geometry::Point3d::Zero(),
            TextureCoordinates::Zero()};
}

}  // namespace kernel
}  // namespace project
