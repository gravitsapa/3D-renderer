#include <renderer.h>
#include <iostream>
#include <algorithm>
#include <rasterizer.h>
#include <interpolator.h>

namespace project {
namespace kernel {

Screen Renderer::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    ZBuffer buffer(screen.GetHeight(), screen.GetWidth());
    screen.Fill(background_color);

    RasterizeWorld(world, buffer, screen, camera);
    GetColorOfEachPixelByLights(world.GetLights(), buffer, camera);
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

void Renderer::GetColorOfEachPixelByLights(const Lights& lights, ZBuffer& buffer,
                                           const PosedCamera& camera) {
    for (int y = 0; y < buffer.GetHeight(); ++y) {
        for (int x = 0; x < buffer.GetWidth(); ++x) {
            auto& opt_vertex_from_buffer = buffer.GetVertex(Height(y), Width(x));
            if (opt_vertex_from_buffer.has_value()) {
                opt_vertex_from_buffer.value().col = lights.GetColorOfPointByLight(
                    opt_vertex_from_buffer.value().col, opt_vertex_from_buffer.value().normal,
                    camera.pos_vector - opt_vertex_from_buffer.value().point);
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

    RasterResolution resolution{screen.GetWidth() - 1, screen.GetHeight() - 1};

    auto rasterized = RasterizeTriangleByXY(
        PrepareForRasterization(face_in_camera_space.a.point, face_as_viewer_see.a.point.z(),
                                resolution, global_face.a),
        PrepareForRasterization(face_in_camera_space.b.point, face_as_viewer_see.b.point.z(),
                                resolution, global_face.b),
        PrepareForRasterization(face_in_camera_space.c.point, face_as_viewer_see.c.point.z(),
                                resolution, global_face.c));

    Color color_by_texture = Color::Random();

    for (auto& pixel : rasterized) {

        Color color_by_texture = texture.GetPixelColor(pixel.tex_coord_div_z / pixel.z_coord_inv);

        assert(std::abs(pixel.z_coord_in_camera_view) <= 1);
        buffer.TryToAddVertex(
            pixel, pixel.z_coord_in_camera_view,
            PixelOriginInformation{
                .point = pixel.global_point_div_z / pixel.z_coord_inv,
                .normal = geometry::Normalized(pixel.normal_div_z / pixel.z_coord_inv),
                .col = color_by_texture});
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
