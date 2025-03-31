#include <raster_coordinates.h>
#include <point.h>

namespace project {
namespace kernel {

geometry::Point2d ConvertToCoordinate(const RasterPoint2d& point, const RasterResolution& res) {
    return geometry::Point2d{detail::ConvertToCoordinate(point.x, res.x_max),
                             detail::ConvertToCoordinate(point.y, res.y_max)};
}
RasterPoint2d ConvertToRasterPoint(const geometry::Point2d& point, const RasterResolution& res) {
    return RasterPoint2d{detail::ConvertToRasterCoordinate(point.x(), res.x_max),
                         detail::ConvertToRasterCoordinate(point.y(), res.y_max)};
}

namespace detail {

geometry::Coordinate ConvertToCoordinate(RasterCoordinate coord, RasterCoordinate max_value) {
    return static_cast<geometry::Coordinate>(coord) / max_value * 2 - 1;
}

RasterCoordinate ConvertToRasterCoordinate(geometry::Coordinate coord, RasterCoordinate max_value) {
    return static_cast<RasterCoordinate>((coord + 1) / 2 * max_value);
}
}  // namespace detail

}  // namespace kernel
}  // namespace project
