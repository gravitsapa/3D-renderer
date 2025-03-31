#pragma once

#include <raster_coordinates.h>
#include <mesh3d.h>
#include <vector>

namespace project {
namespace kernel {

class RasterizedFigure {
public:
    void AddRasterPoint(const RasterPoint2d& point);

    const std::vector<RasterPoint2d>& GetAllPoints() const;

private:
    std::vector<RasterPoint2d> data_;
};

RasterizedFigure RasterizeTriangleByXY(const Face& face, const RasterResolution& resolution);

namespace detail {
void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b, RasterPoint2d& c);
void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b);

}  // namespace detail

}  // namespace kernel

}  // namespace project
