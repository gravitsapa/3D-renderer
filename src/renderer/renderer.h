#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <polygon.h>
#include <vector>
#include <orientation.h>

namespace project {
namespace kernel {

struct RasterPoint {
    int x;
    int y;
    geometry::Coordinate z;
};

struct BufferPoint {
    geometry::Coordinate depth_;
    Color col_;
};

class Renderer {
public:
    Screen Project(const World& world, const PosedCamera& camera, Screen&& screen,
                   const Color& background_color = Color::White());

private:
    std::vector<std::vector<BufferPoint>> z_buffer_;

    geometry::Point3d MoveToGlobalCoordinates(const geometry::Point3d& point,
                                              const geometry::Pose& pose);

    geometry::Point3d MoveToViewerCoordinates(const geometry::Point3d& point,
                                              const geometry::Pose& viewer_pose);

    geometry::Point3d ProjectToCamera(const geometry::Point3d& point, const Camera& camera);

    geometry::Point3d LocalToCamera(const geometry::Point3d& point, const geometry::Pose& pose,
                                    const PosedCamera& camera);

    Polygon LocalToCamera(const Polygon& polygon, const geometry::Pose& pose,
                          const PosedCamera& camera);

    bool InsideCamera(const geometry::Point3d& point);

    std::vector<Polygon> Clip(const Polygon& polygon);

    RasterPoint CameraToScreen(const geometry::Point3d point, Height height, Width width);

    void PushToZBuffer(RasterPoint a, RasterPoint b, RasterPoint c, Color col);
};

}  // namespace kernel
}  // namespace project
