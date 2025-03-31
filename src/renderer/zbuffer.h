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
    geometry::Coordinate depth;
    std::optional<PixelOriginInformation> vertex;
};

class ZBuffer {
public:
    ZBuffer(Height height, Width width);

    bool TryToAddVertex(const RasterPoint2d& raster_point, const geometry::Coordinate& z_coord,
                        const PixelOriginInformation& vertex);

    std::optional<PixelOriginInformation>& GetVertex(Height y, Width x);
    std::optional<PixelOriginInformation> GetVertex(Height y, Width x) const;

    Height GetHeight();
    Width GetWidth();

private:
    structures::Table<BufferPoint> buffer_;
};

}  // namespace kernel
}  // namespace project
