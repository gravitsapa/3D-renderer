#include <renderer.h>
#include <iostream>

namespace project {
namespace kernel {

ZBuffer::ZBuffer(Height height, Width width, RasterCoordinate depth)
    : buffer_(height, width, BufferPoint{depth, std::nullopt}) {
}

bool ZBuffer::TryToAddVertex(const RasterPoint3d& raster_point, const ColoredVertex& vertex) {
    auto& buf_point = buffer_.Get(raster_point.y, raster_point.x);
    if (buf_point.depth <= raster_point.z) {
        return false;
    }

    buf_point = BufferPoint{raster_point.z, vertex};
    return true;
}

std::optional<ColoredVertex> ZBuffer::GetVertex(Height y, Width x) const {
    assert(0 <= y && y < buffer_.GetHeight() && 0 <= x && x < buffer_.GetWidth());
    return buffer_.Get(y, x).vertex;
}

Vertex MoveFromLocalToGlobalCoordinates(const Vertex& local_vertex, const geometry::Pose& pose) {
    return Vertex{.point = pose.rot_matrix * local_vertex.point + pose.pos_vector,
                  .normal = local_vertex.normal,
                  .text_coord = local_vertex.text_coord};
}

Vertex MoveFromGlobalToViewerCoordinates(const Vertex& global_vertex,
                                         const geometry::Pose& viewer_pose) {
    return Vertex{
        .point = viewer_pose.rot_matrix.inverse() * (global_vertex.point - viewer_pose.pos_vector),
        .normal = global_vertex.normal,
        .text_coord = global_vertex.text_coord};
}

Face MoveFromLocalToGlobalCoordinates(const Face& local_face, const geometry::Pose& pose) {
    return Face{MoveFromLocalToGlobalCoordinates(local_face.a, pose),
                MoveFromLocalToGlobalCoordinates(local_face.b, pose),
                MoveFromLocalToGlobalCoordinates(local_face.c, pose)};
}

Face MoveFromGlobalToViewerCoordinates(const Face& global_face, const geometry::Pose& viewer_pose) {
    return Face{MoveFromGlobalToViewerCoordinates(global_face.a, viewer_pose),
                MoveFromGlobalToViewerCoordinates(global_face.b, viewer_pose),
                MoveFromGlobalToViewerCoordinates(global_face.c, viewer_pose)};
}

Screen Renderer::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    ZBuffer buffer(screen.GetHeight(), screen.GetWidth(), raster_depth_max + 1);

    RasterizeWorld(world, buffer, screen, camera);

    for (int y = 0; y < screen.GetHeight(); ++y) {
        for (int x = 0; x < screen.GetWidth(); ++x) {
            auto opt_vertex_from_buffer = buffer.GetVertex(Height(y), Width(x));
            if (opt_vertex_from_buffer.has_value()) {
                screen.SetPixel(Height(y), Width(x), opt_vertex_from_buffer.value().col);
            } else {
                screen.SetPixel(Height(y), Width(x), background_color);
            }
        }
    }

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
    for (auto& face : object.mesh.GetAllFaces()) {
        RasterizeGlobalVertex(face, object.texture, object, buffer, screen, camera);
    }
}

void Renderer::RasterizeGlobalVertex(const Face& face, const Texture& texture,
                                     const geometry::Pose& pose, ZBuffer& buffer, Screen& screen,
                                     const PosedCamera& camera) {
    Face global_face = MoveFromLocalToGlobalCoordinates(face, pose);
    Face face_as_viewer_see = MoveFromGlobalToViewerCoordinates(global_face, camera);
    Face face_in_camera_space = camera.ProjectFaceOnMe(face_as_viewer_see);

    // пока что тупо выкидываем непопадающие треугольники
    // да, это ужасно. Потом перепишем
    if (face_in_camera_space.a.point.x() <= -1 || face_in_camera_space.a.point.x() >= 1 ||
        face_in_camera_space.a.point.y() <= -1 || face_in_camera_space.a.point.y() >= 1 ||
        face_in_camera_space.a.point.z() <= -1 || face_in_camera_space.a.point.z() >= 1 ||
        face_in_camera_space.b.point.x() <= -1 || face_in_camera_space.b.point.x() >= 1 ||
        face_in_camera_space.b.point.y() <= -1 || face_in_camera_space.b.point.y() >= 1 ||
        face_in_camera_space.b.point.z() <= -1 || face_in_camera_space.b.point.z() >= 1 ||
        face_in_camera_space.c.point.x() <= -1 || face_in_camera_space.c.point.x() >= 1 ||
        face_in_camera_space.c.point.y() <= -1 || face_in_camera_space.c.point.y() >= 1 ||
        face_in_camera_space.c.point.z() <= -1 || face_in_camera_space.c.point.z() >= 1) {
        return;
    }

    RasterizeFace(face_in_camera_space, texture, buffer, screen);
}

void SortVertexesByY(Vertex& a, Vertex& b) {
    if (a.point.y() > b.point.y()) {
        std::swap(a, b);
    }
}

Face SortVertexesInFaceByY(Face face) {
    SortVertexesByY(face.a, face.b);
    SortVertexesByY(face.b, face.c);
    SortVertexesByY(face.a, face.b);
    return face;
}

RasterCoordinate ConvertToRasterCoordinate(geometry::Coordinate coord, RasterCoordinate max_value) {
    return static_cast<RasterCoordinate>((coord + 1) / 2 * max_value);
}
RasterPoint3d ConvertToRasterPoint(const geometry::Point3d& point, const RasterResolution& res) {
    return RasterPoint3d{ConvertToRasterCoordinate(point.x(), res.x_max),
                         ConvertToRasterCoordinate(point.y(), res.y_max),
                         ConvertToRasterCoordinate(point.z(), res.z_max)};
}

geometry::Coordinate ConvertToCoordinate(RasterCoordinate coord, RasterCoordinate max_value) {
    return static_cast<geometry::Coordinate>(coord) / max_value * 2 - 1;
}
geometry::Point3d ConvertToCoordinate(const RasterPoint3d& point, const RasterResolution& res) {
    return geometry::Point3d{ConvertToCoordinate(point.x, res.x_max),
                             ConvertToCoordinate(point.y, res.y_max),
                             ConvertToCoordinate(point.z, res.z_max)};
}

// пока что примитивная растеризация
void Renderer::RasterizeFace(const Face& face, const Texture& texture, ZBuffer& buffer,
                             Screen& screen) {
    Face sorted_face = SortVertexesInFaceByY(face);

    RasterResolution res{screen.GetWidth() - 1, screen.GetHeight() - 1, raster_depth_max};
    RasterPoint3d ra = ConvertToRasterPoint(sorted_face.a.point, res);
    RasterPoint3d rb = ConvertToRasterPoint(sorted_face.b.point, res);
    RasterPoint3d rc = ConvertToRasterPoint(sorted_face.c.point, res);

    if (ra.y == rc.y) {
        return;
    }

    // Color col_here = Color::Random();

    RasterCoordinate height = rc.y - ra.y;
    for (RasterCoordinate h = 0; h < height; ++h) {
        bool second_seg = h > rb.y - ra.y || rb.y == ra.y;
        RasterCoordinate seg_height = second_seg ? rc.y - rb.y : rb.y - ra.y;

        Factor alpha = (Factor)(h) / height;
        Factor beta = (Factor)(second_seg ? h - rb.y + ra.y : h) / seg_height;

        RasterCoordinate alpha_x = ra.x + (rc.x - ra.x) * alpha;
        Vertex alpha_vertex = WeightedSum(sorted_face.a, sorted_face.c, alpha);
        RasterCoordinate beta_x;
        Vertex beta_vertex;
        if (second_seg) {
            beta_x = rb.x + (rc.x - rb.x) * beta;
            beta_vertex = WeightedSum(sorted_face.b, sorted_face.c, beta);
        } else {
            beta_x = ra.x + (rb.x - ra.x) * beta;
            beta_vertex = WeightedSum(sorted_face.a, sorted_face.b, beta);
        }

        if (alpha_x > beta_x) {
            std::swap(alpha_x, beta_x);
            std::swap(alpha_vertex, beta_vertex);
        }

        RasterCoordinate y = ra.y + h;
        for (RasterCoordinate x = alpha_x; x <= beta_x; x++) {
            Factor gamma = 0;
            if (beta_x > alpha_x) {
                gamma = (Factor)(x - alpha_x) / (beta_x - alpha_x);
            }

            Vertex real_vertex = WeightedSum(alpha_vertex, beta_vertex, gamma);
            ColoredVertex col_vertex = CreateColoredVertex(real_vertex, texture);
            // ColoredVertex col_vertex = ColoredVertex{
            //     .point = real_vertex.point, .normal = real_vertex.normal, .col = col_here};

            buffer.TryToAddVertex(
                RasterPoint3d{x, y, ConvertToRasterCoordinate(col_vertex.point.z(), res.z_max)},
                col_vertex);
        }
    }
}

}  // namespace kernel
}  // namespace project
