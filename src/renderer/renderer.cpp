#include <renderer.h>

namespace project {
namespace kernel {

Screen Renderer::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    Height height = screen.GetHeight();
    Width width = screen.GetWidth();
    z_buffer_.assign(height, std::vector<BufferPoint>(width, {camera.GetDepth(), background_color}));

    for (auto& object : world.GetObjects()) {
        for (auto& polygon : object.GetPolygons()) {
            Polygon polygon_in_camera_space = LocalToCamera(polygon, object, camera);
            auto clipped_polygons = Clip(polygon_in_camera_space);
            for (auto& clipped_polygon : clipped_polygons) {
                PushToZBuffer(CameraToScreen(clipped_polygon.a, height, width),
                              CameraToScreen(clipped_polygon.b, height, width),
                              CameraToScreen(clipped_polygon.c, height, width),
                              clipped_polygon.color);
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            screen.SetPixel(Height(i), Width(j), z_buffer_[i][j].col);
        }
    }
    return screen;
}

geometry::Point3d Renderer::MoveToGlobalCoordinates(const geometry::Point3d& point,
                                                    const geometry::Pose& pose) {
    return pose.rot_matrix * point + pose.pos_vector;
}

geometry::Point3d Renderer::MoveToViewerCoordinates(const geometry::Point3d& point,
                                                    const geometry::Pose& viewer_pose) {
    return viewer_pose.rot_matrix.inverse() * (point - viewer_pose.pos_vector);
}

geometry::Point3d Renderer::LocalToCamera(const geometry::Point3d& point,
                                          const geometry::Pose& pose, const PosedCamera& camera) {
    return camera.ProjectPointOnMe(
        MoveToViewerCoordinates(MoveToGlobalCoordinates(point, pose), camera));
}

Polygon Renderer::LocalToCamera(const Polygon& polygon, const geometry::Pose& pose,
                                const PosedCamera& camera) {
    Polygon res = polygon;
    res.a = LocalToCamera(res.a, pose, camera);
    res.b = LocalToCamera(res.b, pose, camera);
    res.c = LocalToCamera(res.c, pose, camera);
    return res;
}

bool Renderer::InsideCamera(const geometry::Point3d& point) {
    return -1 <= point.x() && point.x() <= 1 && -1 <= point.y() && point.y() <= 1 &&
           -1 <= point.z() && point.z() <= 1;
}

// Пока что берём полигон, если целиком попадает в область видимости.
// Потом напишем нормальный клиппинг
std::vector<Polygon> Renderer::Clip(const Polygon& polygon) {
    if (!InsideCamera(polygon.a) || !InsideCamera(polygon.b) || !InsideCamera(polygon.c)) {
        return {};
    }
    return {polygon};
}

RasterPoint Renderer::CameraToScreen(const geometry::Point3d point, Height height, Width width) {
    return {static_cast<int>((point.x() + 1) * width / 2),
            static_cast<int>((point.y() + 1) * height / 2), point.z()};
}

// пока что примитивная растеризация
void Renderer::PushToZBuffer(RasterPoint a, RasterPoint b, RasterPoint c, Color col) {
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
            if (z < z_buffer_[y][x].depth) {
                z_buffer_[y][x].col = col;
                z_buffer_[y][x].depth = z;
            }
        }
    }
}

}  // namespace kernel
}  // namespace project
