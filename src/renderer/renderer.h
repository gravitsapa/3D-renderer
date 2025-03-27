#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <polygon.h>
#include <vector>
#include <orientation.h>
#include <table.h>

namespace project {
namespace kernel {

using RasterCoordinate = int;

struct RasterPoint {
    RasterCoordinate x;
    RasterCoordinate y;
    geometry::Coordinate z;
};

struct BufferPoint {
    geometry::Coordinate depth;
    Color col;
};

class ZBuffer {
public:
    ZBuffer(Height height, Width width, geometry::Coordinate depth);

    bool TryToAddNewPixel(const RasterPoint& point);

private:
    structures::Table<geometry::Coordinate> buffer_;
};

class Renderer {
public:
    Screen Project(const World& world, const PosedCamera& camera, Screen&& screen,
                   const Color& background_color = Color::White());

private:
    geometry::Point3d MoveToGlobalCoordinates(const geometry::Point3d& point,
                                              const geometry::Pose& pose);
    geometry::Point3d MoveToViewerCoordinates(const geometry::Point3d& point,
                                              const geometry::Pose& viewer_pose);
    geometry::Point3d ConvertCooridnatesPipeline(const geometry::Point3d& point,
                                                 const geometry::Pose& pose,
                                                 const PosedCamera& camera);
    Triangle ConvertCooridnatesPipeline(const Triangle& polygon, const geometry::Pose& pose,
                                        const PosedCamera& camera);
    RasterPoint CameraToScreen(const geometry::Point3d point, Height height, Width width);

    bool InsideCamera(const geometry::Point3d& point);
    std::vector<Triangle> Clip(const Triangle& polygon);

    void RasterizeWorld(const World& world, ZBuffer& buffer, Screen& screen, const PosedCamera& camera);
    void RasterizeObject(const PosedObject& world, ZBuffer& buffer, Screen& screen, const PosedCamera& camera);
    void RasterizeTriangle(const Triangle& triangle, const geometry::Pose& pose, ZBuffer& buffer, Screen& screen, const PosedCamera& camera);
    void RasterizeTriangle(RasterPoint a, RasterPoint b, RasterPoint c, Color col, ZBuffer& buffer,
                           Screen& screen);
    bool TryToAddNewRasterPoint(RasterPoint point, Color col, ZBuffer& buffer, Screen& screen);
};

}  // namespace kernel
}  // namespace project
