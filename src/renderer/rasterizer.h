#pragma once

#include <raster_coordinates.h>
#include <mesh3d.h>
#include <vector>
#include <interpolator.h>

namespace project {
namespace kernel {

struct VertexForRasterizer : RasterPoint2d, InformationToInterpolate {};

VertexForRasterizer PrepareForRasterization(const geometry::Point3d& projected_point,
                                            const geometry::Coordinate& z_in_camera_view,
                                            const RasterResolution& resolution,
                                            const Vertex& global_vertex);

std::vector<VertexForRasterizer> RasterizeTriangleByXY(VertexForRasterizer a, VertexForRasterizer b,
                                                       VertexForRasterizer c);

namespace detail {
void SortVerticesByY(VertexForRasterizer& a, VertexForRasterizer& b, VertexForRasterizer& c);
void SortVerticesByY(VertexForRasterizer& a, VertexForRasterizer& b);

}  // namespace detail

}  // namespace kernel

}  // namespace project
