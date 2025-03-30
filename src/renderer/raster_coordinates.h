#pragma once
#include <point.h>

namespace project {
namespace kernel {

using RasterCoordinate = long long;

struct RasterPoint3d {
    RasterCoordinate x;
    RasterCoordinate y;
    RasterCoordinate z;
};

struct RasterResolution {
    RasterCoordinate x_max;
    RasterCoordinate y_max;
    RasterCoordinate z_max;
};

RasterPoint3d ConvertToRasterPoint(const geometry::Point3d& point, const RasterResolution& res);
geometry::Point3d ConvertToCoordinate(const RasterPoint3d& point, const RasterResolution& res);

namespace detail {

geometry::Coordinate ConvertToCoordinate(RasterCoordinate coord, RasterCoordinate max_value);
RasterCoordinate ConvertToRasterCoordinate(geometry::Coordinate coord, RasterCoordinate max_value);

}  // namespace detail

}  // namespace kernel
}  // namespace project
