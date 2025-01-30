#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <polygon.h>
#include <vector>

namespace pipeline {

using namespace scene;

struct RasterPoint {
    int x, y;
    Coordinate z;
};

class Pipeline {
public:
    void SetBackgroundColor(const Color& col);

    void SetWorld(const World* world);

    void SetCamera(const PosedCamera* camera);

    void SetScreenSize(const size_t height, const size_t width);

    Screen Project();

private:
    Color background_color_ = Color::White();
    const World* world_ = nullptr;
    const PosedCamera* camera_ = nullptr;
    size_t height_ = 1, width_ = 1;
    Coordinate depth_ = 0;

    struct BufferPoint {
        Coordinate depth_;
        Color col_;
    };

    std::vector<std::vector<BufferPoint>> z_buffer_;

    void Clear();

    Point3d MoveToGlobalCoordinates(const Point3d& point, const Pose& pose);

    Point3d MoveToViewerCoordinates(const Point3d& point, const Pose& viewer_pose);

    Point3d ProjectToCamera(const Point3d& point, const Camera& camera);

    Point3d LocalToCamera(const Point3d& point, const Pose& pose, const PosedCamera& camera);

    Polygon LocalToCamera(const Polygon& polygon, const Pose& pose, const PosedCamera& camera);

    bool InsideCamera(const Point3d& point);

    std::vector<Polygon> Clip(const Polygon& polygon);

    RasterPoint CameraToScreen(const Point3d point);

    void PushToZBuffer(RasterPoint a, RasterPoint b, RasterPoint c, Color col);
};

}  // namespace pipeline
