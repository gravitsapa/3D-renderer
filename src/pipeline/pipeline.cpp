#include <pipeline.h>

namespace pipeline {

Screen Pipeline::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    Coordinate depth_ = camera.FarPlane - camera.NearPlane;
    Height height = screen.GetHeight();
    Width width = screen.GetWidth();
    z_buffer_.assign(height, std::vector<BufferPoint>(width, {depth_, background_color}));

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

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            screen.GetMatrix()[i][j] = z_buffer_[i][j].col_;
        }
    }
    return screen;
}

Point3d Pipeline::MoveToGlobalCoordinates(const Point3d& point, const Pose& pose) {
    return pose.rot_matrix * point + pose.pos_vector;
}

Point3d Pipeline::MoveToViewerCoordinates(const Point3d& point, const Pose& viewer_pose) {
    return viewer_pose.rot_matrix.inverse() * (point - viewer_pose.pos_vector);
}

// Пока что тупо проецируем на NearPlane. Потом перепишем на перспективную проекцию
Point3d Pipeline::ProjectToCamera(const Point3d& point, const Camera& camera) {
    auto n = camera.NearPlane;
    auto f = camera.FarPlane;
    auto l = -camera.LeftSide;
    auto r = camera.RightSide;
    auto t = camera.TopSide;
    auto b = -camera.BottomSide;
    Matrix4d project_matrix{{2 * n / (r - l), 0, (r + l) / (r - l), 0},
                            {0, 2 * n / (t - b), (t + b) / (t - b), 0},
                            {0, 0, -(f + n) / (f - n), -2 * n * f / (f - n)},
                            {0, 0, -1, 0}};
    Vector4d p = project_matrix * Vector4d{point.x(), point.y(), point.z(), 1};
    auto w = p(3, 0);
    return {p.x() / w, p.y() / w, p.z() / w};
}

Point3d Pipeline::LocalToCamera(const Point3d& point, const Pose& pose, const PosedCamera& camera) {
    return ProjectToCamera(MoveToViewerCoordinates(MoveToGlobalCoordinates(point, pose), camera),
                           camera);
}

Polygon Pipeline::LocalToCamera(const Polygon& polygon, const Pose& pose,
                                const PosedCamera& camera) {
    Polygon res = polygon;
    res.a = LocalToCamera(res.a, pose, camera);
    res.b = LocalToCamera(res.b, pose, camera);
    res.c = LocalToCamera(res.c, pose, camera);
    return res;
}

bool Pipeline::InsideCamera(const Point3d& point) {
    return -1 <= point.x() && point.x() <= 1 && -1 <= point.y() && point.y() <= 1 &&
           -1 <= point.z() && point.z() <= 1;
}

// Пока что берём полигон, если целиком попадает в область видимости.
// Потом напишем нормальный клиппинг
std::vector<Polygon> Pipeline::Clip(const Polygon& polygon) {
    if (!InsideCamera(polygon.a) || !InsideCamera(polygon.b) || !InsideCamera(polygon.c)) {
        return {};
    }
    return {polygon};
}

RasterPoint Pipeline::CameraToScreen(const Point3d point, Height height, Width width) {
    return {static_cast<int>((point.x() + 1) * width / 2),
            static_cast<int>((point.y() + 1) * height / 2), point.z()};
}

// пока что примитивная растеризация
void Pipeline::PushToZBuffer(RasterPoint a, RasterPoint b, RasterPoint c, Color col) {
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
        Coordinate alpha_z = a.z + (c.z - a.z) * alpha;
        int beta_x = second_seg ? b.x + (c.x - b.x) * beta : a.x + (b.x - a.x) * beta;
        Coordinate beta_z = second_seg ? b.z + (c.z - b.z) * beta : a.z + (b.z - a.z) * beta;

        if (alpha_x > beta_x) {
            std::swap(alpha_x, beta_x);
            std::swap(alpha_z, beta_z);
        }

        int y = a.y + h;
        for (int x = alpha_x; x <= beta_x; x++) {
            Coordinate z = alpha_z + (float)(x - alpha_x) / (beta_x - alpha_x) * (beta_z - alpha_z);
            if (z < z_buffer_[y][x].depth_) {
                z_buffer_[y][x].col_ = col;
                z_buffer_[y][x].depth_ = z;
            }
        }
    }
}

}  // namespace pipeline