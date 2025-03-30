#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <vector>
#include <orientation.h>
#include <table.h>
#include <object.h>
#include <optional>
#include <raster_coordinates.h>
#include <zbuffer.h>

namespace project {
namespace kernel {

constexpr RasterCoordinate raster_depth_max = 1'000'000'000'000;

using Factor = double;
Vertex WeightedSum(const Vertex& a, const Vertex& b, Factor alpha);

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
    void RasterizeFace(Face face, Face global_face, const Texture& texture, ZBuffer& buffer,
                       Screen& screen);

    void GetColorOfEachPixelByLights(const Lights& lights, ZBuffer& buffer);
    void PrintAllPixelsFromBufferToScreen(const ZBuffer& buffer, Screen& screen);
};

namespace detail {
Vertex MoveFromLocalToGlobalCoordinates(const Vertex& local_face, const geometry::Pose& pose);

Vertex MoveFromGlobalToViewerCoordinates(const Vertex& global_face,
                                         const geometry::Pose& viewer_pose);

Face MoveFromLocalToGlobalCoordinates(const Face& local_face, const geometry::Pose& pose);

Face MoveFromGlobalToViewerCoordinates(const Face& global_face, const geometry::Pose& viewer_pose);

};  // namespace detail

}  // namespace kernel
}  // namespace project
