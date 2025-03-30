#pragma once

#include <color.h>
#include <point.h>
#include <raster_coordinates.h>
#include <orientation.h>
#include <table.h>

namespace project {
namespace kernel {

struct PixelOriginInformation {
    geometry::Point3d point;
    geometry::Vector3d normal;
    Color col;
};

struct BufferPoint {
    RasterCoordinate depth;
    std::optional<PixelOriginInformation> vertex;
};

class ZBuffer {
public:
    ZBuffer(Height height, Width width, RasterCoordinate depth);

    bool TryToAddVertex(const RasterPoint3d& raster_point, const PixelOriginInformation& vertex);
    std::optional<PixelOriginInformation>& GetVertex(Height y, Width x);
    std::optional<PixelOriginInformation> GetVertex(Height y, Width x) const;

    Height GetHeight();
    Width GetWidth();

private:
    structures::Table<BufferPoint> buffer_;
};

}  // namespace kernel
}  // namespace project
