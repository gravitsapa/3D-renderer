#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <polygon.h>
#include <vector>
#include <orientation.h>

namespace pipeline {

using namespace scene;

struct RasterPoint {
    int x, y;
    Coordinate z;
};

struct BufferPoint {
    Coordinate depth_;
    Color col_;
};

class Pipeline {
public:
    Screen Project(const World& world, const PosedCamera& camera, Screen&& screen,
                   const Color& background_color = Color::White());

private:
    std::vector<std::vector<BufferPoint>> z_buffer_;

    void Clear();

    Point3d MoveToGlobalCoordinates(const Point3d& point, const Pose& pose);

    Point3d MoveToViewerCoordinates(const Point3d& point, const Pose& viewer_pose);

    Point3d ProjectToCamera(const Point3d& point, const Camera& camera);

    Point3d LocalToCamera(const Point3d& point, const Pose& pose, const PosedCamera& camera);

    Polygon LocalToCamera(const Polygon& polygon, const Pose& pose, const PosedCamera& camera);

    bool InsideCamera(const Point3d& point);

    std::vector<Polygon> Clip(const Polygon& polygon);

    RasterPoint CameraToScreen(const Point3d point, Height height, Width width);

    void PushToZBuffer(RasterPoint a, RasterPoint b, RasterPoint c, Color col);
};

}  // namespace pipeline
