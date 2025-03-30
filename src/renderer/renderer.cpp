#include <renderer.h>
#include <iostream>
#include <algorithm>

namespace project {
namespace kernel {

// нужно дописать алгоритм для нормали
Vertex WeightedSum(const Vertex& a, const Vertex& b, Factor alpha) {
    return Vertex{
        .point = (a.point * (1 - alpha) + b.point * alpha),
        .normal = a.normal,
        .text_coord = TextureCoordinates{a.text_coord.h * (1 - alpha) + b.text_coord.h * alpha,
                                         a.text_coord.w * (1 - alpha) + b.text_coord.w * alpha}};
}

Screen Renderer::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    ZBuffer buffer(screen.GetHeight(), screen.GetWidth(), raster_depth_max + 1);
    screen.Fill(background_color);

    RasterizeWorld(world, buffer, screen, camera);
    GetColorOfEachPixelByLights(world.GetLights(), buffer);
    PrintAllPixelsFromBufferToScreen(buffer, screen);

    return screen;
}

void Renderer::PrintAllPixelsFromBufferToScreen(const ZBuffer& buffer, Screen& screen) {
    for (int y = 0; y < screen.GetHeight(); ++y) {
        for (int x = 0; x < screen.GetWidth(); ++x) {
            const auto& opt_vertex_from_buffer = buffer.GetVertex(Height(y), Width(x));
            if (opt_vertex_from_buffer.has_value()) {
                screen.SetPixel(Height(y), Width(x), opt_vertex_from_buffer.value().col);
            }
        }
    }
}

void Renderer::GetColorOfEachPixelByLights(const Lights& lights, ZBuffer& buffer) {
    for (int y = 0; y < buffer.GetHeight(); ++y) {
        for (int x = 0; x < buffer.GetWidth(); ++x) {
            auto& opt_vertex_from_buffer = buffer.GetVertex(Height(y), Width(x));
            if (opt_vertex_from_buffer.has_value()) {
                opt_vertex_from_buffer.value().col = lights.GetColorOfPointByLight(
                    opt_vertex_from_buffer.value().col, opt_vertex_from_buffer.value().normal);
            }
        }
    }
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
    Face global_face = detail::MoveFromLocalToGlobalCoordinates(face, pose);
    Face face_as_viewer_see = detail::MoveFromGlobalToViewerCoordinates(global_face, camera);
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

    RasterizeFace(face_in_camera_space, global_face, texture, buffer, screen);
}

// пока что примитивная растеризация
void Renderer::RasterizeFace(Face face, Face global_face, const Texture& texture, ZBuffer& buffer,
                             Screen& screen) {

    std::vector<std::pair<Vertex, Vertex>> paired_vertexes = {
        {face.a, global_face.a}, {face.b, global_face.b}, {face.c, global_face.c}};

    std::sort(paired_vertexes.begin(), paired_vertexes.end(),
              [](const std::pair<Vertex, Vertex>& a, const std::pair<Vertex, Vertex>& b) {
                  return a.first.point.y() < b.first.point.y();
              });
    face.a = paired_vertexes[0].first;
    face.b = paired_vertexes[1].first;
    face.c = paired_vertexes[2].first;
    global_face.a = paired_vertexes[0].second;
    global_face.b = paired_vertexes[1].second;
    global_face.c = paired_vertexes[2].second;

    RasterResolution res{screen.GetWidth() - 1, screen.GetHeight() - 1, raster_depth_max};
    RasterPoint3d ra = ConvertToRasterPoint(face.a.point, res);
    RasterPoint3d rb = ConvertToRasterPoint(face.b.point, res);
    RasterPoint3d rc = ConvertToRasterPoint(face.c.point, res);

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
        Vertex alpha_vertex = WeightedSum(face.a, face.c, alpha);
        RasterCoordinate beta_x;
        Vertex beta_vertex;
        if (second_seg) {
            beta_x = rb.x + (rc.x - rb.x) * beta;
            beta_vertex = WeightedSum(face.b, face.c, beta);
        } else {
            beta_x = ra.x + (rb.x - ra.x) * beta;
            beta_vertex = WeightedSum(face.a, face.b, beta);
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
            PixelOriginInformation col_vertex{.point = real_vertex.point,
                                              .normal = global_face.a.normal,
                                              .col = texture.GetPixelColor(real_vertex.text_coord)};

            // PixelOriginInformation col_vertex = PixelOriginInformation{
            //     .point = real_vertex.point, .normal = real_vertex.normal, .col = col_here};

            buffer.TryToAddVertex(
                RasterPoint3d{x, y,
                              detail::ConvertToRasterCoordinate(col_vertex.point.z(), res.z_max)},
                col_vertex);
        }
    }
}

namespace detail {
Vertex MoveFromLocalToGlobalCoordinates(const Vertex& local_vertex, const geometry::Pose& pose) {
    return Vertex{.point = pose.rot_matrix * local_vertex.point + pose.pos_vector,
                  .normal = pose.rot_matrix * local_vertex.normal,
                  .text_coord = local_vertex.text_coord};
}

Vertex MoveFromGlobalToViewerCoordinates(const Vertex& global_vertex,
                                         const geometry::Pose& viewer_pose) {
    return Vertex{
        .point = viewer_pose.rot_matrix.inverse() * (global_vertex.point - viewer_pose.pos_vector),
        .normal = viewer_pose.rot_matrix.inverse() * global_vertex.normal,
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
}  // namespace detail

}  // namespace kernel
}  // namespace project
