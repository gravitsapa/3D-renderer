#include <interpolator.h>
#include <texture.h>
#include <cassert>

namespace project {
namespace kernel {

InformationToInterpolate GetWeightedInformation(const VertexWeights& weights,
                                                const InformationToInterpolate& a,
                                                const InformationToInterpolate& b,
                                                const InformationToInterpolate& c) {
    return InformationToInterpolate{
        weights.a * a.z_coord_in_camera_view + weights.b * b.z_coord_in_camera_view +
            weights.c * c.z_coord_in_camera_view,
        weights.a * a.z_coord_inv + weights.b * b.z_coord_inv + weights.c * c.z_coord_inv,
        weights.a * a.normal_div_z + weights.b * b.normal_div_z + weights.c * c.normal_div_z,
        weights.a * a.global_point_div_z + weights.b * b.global_point_div_z +
            weights.c * c.global_point_div_z,
        weights.a * a.tex_coord_div_z + weights.b * b.tex_coord_div_z +
            weights.c * c.tex_coord_div_z};
}

// WeightsFinder::WeightsFinder(const RasterPoint2d& a, const RasterPoint2d& b, const RasterPoint2d&
// c)
//     : a_(a), b_(b), c_(c) {
//     assert(a.y <= b.y && b.y <= c.y);
// }

// VertexWeights FindWeights(const RasterPoint2d& p) {
//     assert(a_.y <= p.y && p.y <= c_.y);

// }

}  // namespace kernel
}  // namespace project
