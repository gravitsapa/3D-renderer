#pragma once

#include <world.h>
#include <color.h>
#include <screen.h>
#include <polygon.h>
#include <vector>

class Pipeline {
public:
    void SetBackgroundColor(const Color& col) {
        background_color_ = col;
    }

    void SetWorld(const World* world) {
        world_ = world;
    }

    void SetCamera(const PosedCamera* camera) {
        camera_ = camera;
        if (camera) {
            depth_ = camera->cam_.FarPlane - camera->cam_.NearPlane;
        } else {
            depth_ = 0;
        }
    }

    void SetScreenSize(const size_t height, const size_t width) {
        height_ = height;
        width_ = width;
    }

    Screen Project() {
        Clear();
        if (world_ && camera_) {
            for (auto& object : world_->GetObjects()) {
                for (auto& polygon : object.obj_.GetPolygons()) {
                    Polygon polygon_in_camera_space =
                        LocalToCamera(polygon, object.pose_, *camera_);
                    auto clipped_polygons = Clip(polygon_in_camera_space);
                    for (auto& clipped_polygon : clipped_polygons) {
                        PushToZBuffer(CameraToScreen(clipped_polygon.a_),
                                      CameraToScreen(clipped_polygon.b_),
                                      CameraToScreen(clipped_polygon.c_), clipped_polygon.color_);
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

    void Clear() {
        z_buffer_.assign(height_, std::vector<BufferPoint>(width_, {depth_, background_color_}));
    }

    Point3d MoveToGlobalCoordinates(const Point3d& point, const Pose& pose) {
        return pose.rot_.rot_matrix_ * point + pose.pos_.pos_vector_;
    }

    Point3d MoveToViewerCoordinates(const Point3d& point, const Pose& viewer_pose) {
        return viewer_pose.rot_.rot_matrix_.inverse() * (point - viewer_pose.pos_.pos_vector_);
    }

    // Пока что тупо проецируем на NearPlane. Потом перепишем на перспективную проекцию
    Point3d ProjectToCamera(const Point3d& point, const Camera& camera) {
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

    Point3d LocalToCamera(const Point3d& point, const Pose& pose, const PosedCamera& camera) {
        return ProjectToCamera(
            MoveToViewerCoordinates(MoveToGlobalCoordinates(point, pose), camera.pose_),
            camera.cam_);
    }

    Polygon LocalToCamera(const Polygon& polygon, const Pose& pose, const PosedCamera& camera) {
        Polygon res = polygon;
        res.a_ = LocalToCamera(res.a_, pose, camera);
        res.b_ = LocalToCamera(res.b_, pose, camera);
        res.c_ = LocalToCamera(res.c_, pose, camera);
        return res;
    }

    bool InsideCamera(const Point3d& point) {
        return -1 <= point.x() && point.x() <= 1 && -1 <= point.y() && point.y() <= 1 &&
               -1 <= point.z() && point.z() <= 1;
    }

    // Пока что берём полигон, если целиком попадает в область видимости.
    // Потом напишем нормальный клиппинг
    std::vector<Polygon> Clip(const Polygon& polygon) {
        if (!InsideCamera(polygon.a_) || !InsideCamera(polygon.b_) || !InsideCamera(polygon.c_)) {
            return {};
        }
        return {polygon};
    }

    struct RasterPoint {
        int x, y;
        Coordinate z;
    };

    RasterPoint CameraToScreen(const Point3d point) {
        return {(point.x() + 1) * width_ / 2, (point.y() + 1) * height_ / 2, point.z()};
    }

    // пока что примитивная растеризация
    void PushToZBuffer(RasterPoint a, RasterPoint b, RasterPoint c, Color col) {
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
                Coordinate z =
                    alpha_z + (float)(x - alpha_x) / (beta_x - alpha_x) * (beta_z - alpha_z);
                if (z < z_buffer_[y][x].depth_) {
                    z_buffer_[y][x].col_ = col;
                    z_buffer_[y][x].depth_ = z;
                }
            }
        }
    }
};
