#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <vector>
#include <orientation.h>
#include <table.h>
#include <object.h>
#include <optional>

namespace project {
namespace kernel {

using RasterCoordinate = long long;
constexpr RasterCoordinate raster_depth_max = 1'000'000'000'000;

struct RasterPoint3d {
    RasterCoordinate x;
    RasterCoordinate y;
    RasterCoordinate z;
};

struct BufferPoint {
    RasterCoordinate depth;
    std::optional<ColoredVertex> vertex;
};

Vertex MoveFromLocalToGlobalCoordinates(const Vertex& local_face, const geometry::Pose& pose);

Vertex MoveFromGlobalToViewerCoordinates(const Vertex& global_face,
                                         const geometry::Pose& viewer_pose);

Face MoveFromLocalToGlobalCoordinates(const Face& local_face, const geometry::Pose& pose);

Face MoveFromGlobalToViewerCoordinates(const Face& global_face, const geometry::Pose& viewer_pose);

struct RasterResolution {
    RasterCoordinate x_max;
    RasterCoordinate y_max;
    RasterCoordinate z_max;
};

RasterCoordinate ConvertToRasterCoordinate(geometry::Coordinate coord, RasterCoordinate max_value);
RasterPoint3d ConvertToRasterPoint(const geometry::Point3d& point, const RasterResolution& res);

geometry::Coordinate ConvertToCoordinate(RasterCoordinate coord, RasterCoordinate max_value);
geometry::Point3d ConvertToCoordinate(const RasterPoint3d& point, const RasterResolution& res);

class ZBuffer {
public:
    ZBuffer(Height height, Width width, RasterCoordinate depth);

    bool TryToAddVertex(const RasterPoint3d& raster_point, const ColoredVertex& vertex);
    std::optional<ColoredVertex>& GetVertex(Height y, Width x);
    std::optional<ColoredVertex> GetVertex(Height y, Width x) const;

    Height GetHeight();
    Width GetWidth();
private:
    structures::Table<BufferPoint> buffer_;
};

class Renderer {
public:
    Screen Project(const World& world, const PosedCamera& camera, Screen&& screen,
                   const Color& background_color = Color::White());

private:
    void RasterizeWorld(const World& world, ZBuffer& buffer, Screen& screen,
                        const PosedCamera& camera);
    void RasterizeObject(const PosedObject& world, ZBuffer& buffer, Screen& screen,
                         const PosedCamera& camera);
    void RasterizeGlobalVertex(const Face& face, const Texture& texture, const geometry::Pose& pose,
                               ZBuffer& buffer, Screen& screen, const PosedCamera& camera);
    void RasterizeFace(Face face, Face global_face, const Texture& texture, ZBuffer& buffer, Screen& screen);

    void GetColorOfEachPixelByLights(const Lights& lights, ZBuffer& buffer);
    void PrintAllPixelsFromBufferToScreen(const ZBuffer& buffer, Screen& screen);
};

}  // namespace kernel
}  // namespace project
