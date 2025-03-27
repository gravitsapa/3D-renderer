#include <renderer.h>

namespace project {
namespace kernel {

ZBuffer::ZBuffer(Height height, Width width, geometry::Coordinate depth)
    : buffer_(height, width, depth) {
}

bool ZBuffer::TryToAddNewPixel(const RasterPoint& point) {
    auto& depth = buffer_.Get(point.y, point.x);
    if (point.z >= depth) {
        return false;
    }

    depth = point.z;
    return true;
}

geometry::Point3d Renderer::MoveToGlobalCoordinates(const geometry::Point3d& point,
                                                    const geometry::Pose& pose) {
    return pose.rot_matrix * point + pose.pos_vector;
}

geometry::Point3d Renderer::MoveToViewerCoordinates(const geometry::Point3d& point,
                                                    const geometry::Pose& viewer_pose) {
    return viewer_pose.rot_matrix.inverse() * (point - viewer_pose.pos_vector);
}

geometry::Point3d Renderer::ConvertCooridnatesPipeline(const geometry::Point3d& point,
                                                       const geometry::Pose& pose,
                                                       const PosedCamera& camera) {
    return camera.ProjectPointOnMe(
        MoveToViewerCoordinates(MoveToGlobalCoordinates(point, pose), camera));
}

Triangle Renderer::ConvertCooridnatesPipeline(const Triangle& polygon, const geometry::Pose& pose,
                                              const PosedCamera& camera) {
    Triangle res = polygon;
    res.a = ConvertCooridnatesPipeline(res.a, pose, camera);
    res.b = ConvertCooridnatesPipeline(res.b, pose, camera);
    res.c = ConvertCooridnatesPipeline(res.c, pose, camera);
    return res;
}

bool Renderer::InsideCamera(const geometry::Point3d& point) {
    return -1 <= point.x() && point.x() <= 1 && -1 <= point.y() && point.y() <= 1 &&
           -1 <= point.z() && point.z() <= 1;
}

// Пока что берём полигон, если целиком попадает в область видимости.
// Потом напишем нормальный клиппинг
std::vector<Triangle> Renderer::Clip(const Triangle& polygon) {
    if (!InsideCamera(polygon.a) || !InsideCamera(polygon.b) || !InsideCamera(polygon.c)) {
        return {};
    }
    return {polygon};
}

RasterPoint Renderer::CameraToScreen(const geometry::Point3d point, Height height, Width width) {
    return {static_cast<int>((point.x() + 1) * width / 2),
            static_cast<int>((point.y() + 1) * height / 2), point.z()};
}

Screen Renderer::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    ZBuffer buffer(screen.GetHeight(), screen.GetWidth(), camera.GetDepth());
    screen.Fill(background_color);

    RasterizeWorld(world, buffer, screen, camera);

    return screen;
}

void Renderer::RasterizeWorld(const World& world, ZBuffer& buffer, Screen& screen,
                              const PosedCamera& camera) {
    for (auto& object : world.GetObjects()) {
        RasterizeObject(object, buffer, screen, camera);
    }
}

void Renderer::RasterizeObject(const PosedObject& object, ZBuffer& buffer, Screen& screen,
                               const PosedCamera& camera) {
    for (auto& triangle : object.GetPolygons()) {
        RasterizeTriangle(triangle, object, buffer, screen, camera);
    }
}

void Renderer::RasterizeTriangle(const Triangle& triangle, const geometry::Pose& pose, ZBuffer& buffer, Screen& screen,
                                 const PosedCamera& camera) {
    Triangle triangle_in_camera_space = ConvertCooridnatesPipeline(triangle, pose, camera);
    auto clipped_polygons = Clip(triangle_in_camera_space);
    for (auto& clipped_polygon : clipped_polygons) {
        auto height = screen.GetHeight();
        auto width = screen.GetWidth();
        RasterizeTriangle(CameraToScreen(clipped_polygon.a, height, width),
                          CameraToScreen(clipped_polygon.b, height, width),
                          CameraToScreen(clipped_polygon.c, height, width), clipped_polygon.color,
                          buffer, screen);
    }
}

// пока что примитивная растеризация
void Renderer::RasterizeTriangle(RasterPoint a, RasterPoint b, RasterPoint c, Color col,
                                 ZBuffer& buffer, Screen& screen) {
    if (a.y > b.y)
        std::swap(a, b);
    if (b.y > c.y)
        std::swap(b, c);
    if (a.y > b.y)
        std::swap(a, b);

    if (a.y == c.y)
        return;

    int height = c.y - a.y;
    for (int h = 0; h < height; ++h) {
        bool second_seg = h > b.y - a.y || b.y == a.y;
        int seg_height = second_seg ? c.y - b.y : b.y - a.y;

        float alpha = (float)(h) / height;
        float beta = (float)(second_seg ? h - b.y + a.y : h) / seg_height;

        int alpha_x = a.x + (c.x - a.x) * alpha;
        geometry::Coordinate alpha_z = a.z + (c.z - a.z) * alpha;
        int beta_x = second_seg ? b.x + (c.x - b.x) * beta : a.x + (b.x - a.x) * beta;
        geometry::Coordinate beta_z =
            second_seg ? b.z + (c.z - b.z) * beta : a.z + (b.z - a.z) * beta;

        if (alpha_x > beta_x) {
            std::swap(alpha_x, beta_x);
            std::swap(alpha_z, beta_z);
        }

        int y = a.y + h;
        for (int x = alpha_x; x <= beta_x; x++) {
            geometry::Coordinate z =
                alpha_z + (float)(x - alpha_x) / (beta_x - alpha_x) * (beta_z - alpha_z);
            TryToAddNewRasterPoint(RasterPoint{x, y, z}, col, buffer, screen);
        }
    }
}

bool Renderer::TryToAddNewRasterPoint(RasterPoint point, Color col, ZBuffer& buffer,
                                      Screen& screen) {
    if (!buffer.TryToAddNewPixel(point)) {
        return false;
    }

    screen.SetPixel(Height(point.y), Width(point.x), col);
    return true;
}

}  // namespace kernel
}  // namespace project
