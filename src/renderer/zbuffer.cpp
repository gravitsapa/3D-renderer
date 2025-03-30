#include <zbuffer.h>

namespace project {
namespace kernel {

ZBuffer::ZBuffer(Height height, Width width, RasterCoordinate depth)
    : buffer_(height, width, BufferPoint{depth, std::nullopt}) {
}

bool ZBuffer::TryToAddVertex(const RasterPoint3d& raster_point,
                             const PixelOriginInformation& vertex) {
    auto& buf_point = buffer_.Get(raster_point.y, raster_point.x);
    if (buf_point.depth <= raster_point.z) {
        return false;
    }

    buf_point = BufferPoint{raster_point.z, vertex};
    return true;
}

Height ZBuffer::GetHeight() {
    return Height(buffer_.GetHeight());
}

Width ZBuffer::GetWidth() {
    return Width(buffer_.GetWidth());
}

std::optional<PixelOriginInformation>& ZBuffer::GetVertex(Height y, Width x) {
    assert(0 <= y && y < buffer_.GetHeight() && 0 <= x && x < buffer_.GetWidth());
    return buffer_.Get(y, x).vertex;
}

std::optional<PixelOriginInformation> ZBuffer::GetVertex(Height y, Width x) const {
    assert(0 <= y && y < buffer_.GetHeight() && 0 <= x && x < buffer_.GetWidth());
    return buffer_.Get(y, x).vertex;
}

}  // namespace kernel
}  // namespace project
