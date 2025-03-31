#pragma once

#include <raster_coordinates.h>
#include <mesh3d.h>
#include <vector>

namespace project {
namespace kernel {

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
RasterizedFigure RasterizeTriangleByXY(const Face& face, const RasterResolution& resolution);

namespace detail {

HorizontalSegment Merge(const HorizontalSegment& segment1, const HorizontalSegment& segment2);

void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b, RasterPoint2d& c);
void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b);

}  // namespace detail

}  // namespace kernel

}  // namespace project
