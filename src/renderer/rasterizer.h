#pragma once

#include <raster_coordinates.h>
#include <mesh3d.h>
#include <vector>
#include <interpolator.h>

namespace project {
namespace kernel {

struct VertexForRasterizer : RasterPoint2d, InformationToInterpolate {};

VertexForRasterizer PrepareForRasterization(const geometry::Point3d projected_point,
                                            const geometry::Coordinate z_in_camera_view,
                                            const RasterResolution& resolution,
                                            const Vertex& global_vertex);

std::vector<VertexForRasterizer> RasterizeTriangleByXY(VertexForRasterizer a, VertexForRasterizer b,
                                                       VertexForRasterizer c);

namespace detail {

struct HorizontalSegment {
    RasterCoordinate y;
    RasterCoordinate left_x, right_x;
};

class RasterizedFigure {
public:
    void AddRasterSegment(const HorizontalSegment& point);

    const std::vector<HorizontalSegment>& GetAllSegments() const;

    void Merge(const RasterizedFigure& another_figure);

private:
    std::vector<HorizontalSegment> data_;
};

RasterizedFigure BrezAlgo(RasterPoint2d a, RasterPoint2d b);

Factor GetFactorByPointInSegment(RasterCoordinate x, RasterCoordinate begin, RasterCoordinate end);

HorizontalSegment Merge(const HorizontalSegment& segment1, const HorizontalSegment& segment2);

void SortVerticesByY(VertexForRasterizer& a, VertexForRasterizer& b, VertexForRasterizer& c);
void SortVerticesByY(VertexForRasterizer& a, VertexForRasterizer& b);

}  // namespace detail

}  // namespace kernel

}  // namespace project
