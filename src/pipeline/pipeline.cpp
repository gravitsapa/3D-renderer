#include <pipeline.h>

namespace pipeline {

void Pipeline::SetBackgroundColor(const Color& col) {
    background_color_ = col;
}

void Pipeline::SetWorld(const World* world) {
    world_ = world;
}

void Pipeline::SetCamera(const PosedCamera* camera) {
    camera_ = camera;
    if (camera) {
        depth_ = camera->cam_.FarPlane - camera->cam_.NearPlane;
    } else {
        depth_ = 0;
    }
}

void Pipeline::SetScreenSize(const size_t height, const size_t width) {
    height_ = height;
    width_ = width;
}

Screen Pipeline::Project() {
    Clear();
    if (world_ && camera_) {
        for (auto& object : world_->GetObjects()) {
            for (auto& polygon : object.obj_.GetPolygons()) {
                Polygon polygon_in_camera_space = LocalToCamera(polygon, object.pose_, *camera_);
                auto clipped_polygons = Clip(polygon_in_camera_space);
                for (auto& clipped_polygon : clipped_polygons) {
                    PushToZBuffer(CameraToScreen(clipped_polygon.a),
                                  CameraToScreen(clipped_polygon.b),
                                  CameraToScreen(clipped_polygon.c), clipped_polygon.color);
                }
            }
        }
    }
    Screen screen;
    screen.SetSize(height_, width_);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            screen.GetMatrix()[i][j] = z_buffer_[i][j].col_;
        }
    }
    return screen;
}

void Pipeline::Clear() {
    z_buffer_.assign(height_, std::vector<BufferPoint>(width_, {depth_, background_color_}));
}

Point3d Pipeline::MoveToGlobalCoordinates(const Point3d& point, const Pose& pose) {
    return pose.rot_.rot_matrix * point + pose.pos_.pos_vector;
}

Point3d Pipeline::MoveToViewerCoordinates(const Point3d& point, const Pose& viewer_pose) {
    return viewer_pose.rot_.rot_matrix.inverse() * (point - viewer_pose.pos_.pos_vector);
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
    return ProjectToCamera(
        MoveToViewerCoordinates(MoveToGlobalCoordinates(point, pose), camera.pose_), camera.cam_);
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

RasterPoint Pipeline::CameraToScreen(const Point3d point) {
    return {static_cast<int>((point.x() + 1) * width_ / 2),
            static_cast<int>((point.y() + 1) * height_ / 2), point.z()};
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