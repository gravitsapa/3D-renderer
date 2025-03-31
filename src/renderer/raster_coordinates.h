#pragma once
#include <point.h>

namespace project {
namespace kernel {

using RasterCoordinate = long long;

struct RasterPoint2d {
    RasterCoordinate x;
    RasterCoordinate y;
};

struct RasterPoint3d {
    RasterCoordinate x;
    RasterCoordinate y;
    RasterCoordinate z;
};

struct RasterResolution {
    RasterCoordinate x_max;
    RasterCoordinate y_max;
};

RasterPoint2d ConvertToRasterPoint(const geometry::Point2d& point, const RasterResolution& res);
geometry::Point2d ConvertToCoordinate(const RasterPoint2d& point, const RasterResolution& res);

namespace detail {

geometry::Coordinate ConvertToCoordinate(RasterCoordinate coord, RasterCoordinate max_value);
RasterCoordinate ConvertToRasterCoordinate(geometry::Coordinate coord, RasterCoordinate max_value);

}  // namespace detail

}  // namespace kernel
}  // namespace project
