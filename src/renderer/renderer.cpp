#include <renderer.h>
#include <iostream>
#include <algorithm>
#include <rasterizer.h>
#include <interpolator.h>

namespace project {
namespace kernel {

Screen Renderer::Project(const World& world, const PosedCamera& camera, Screen&& screen,
                         const Color& background_color) {
    ZBuffer buffer(screen.GetHeight(), screen.GetWidth(), raster_depth_max + 1);
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
    RasterizedFigure pixel_triangle = RasterizeTriangleByXY(face_in_camera_space, resolution);

    Color color_by_texture = Color::Random();
    geometry::Point2d point_a_in_2d = geometry::TruncZ(face_in_camera_space.a.point);
    geometry::Point2d point_b_in_2d = geometry::TruncZ(face_in_camera_space.b.point);
    geometry::Point2d point_c_in_2d = geometry::TruncZ(face_in_camera_space.c.point);
    WeightsFinder weights_finder(point_a_in_2d, point_b_in_2d, point_c_in_2d);

    for (auto pixel : pixel_triangle.GetAllPoints()) {
        geometry::Point2d pixel_in_camera_space = ConvertToCoordinate(pixel, resolution);

        VertexWeights weights = weights_finder.FindBarycentricCoordinates(pixel_in_camera_space);

        // std::cerr << "WEIGHTS " << weights.a << ' ' << weights.b << ' ' << weights.c <<
        // std::endl;

        RasterCoordinate z_coordinate_in_buffer = detail::ConvertToRasterCoordinate(
            InterpolateCoordinate(weights, face_in_camera_space.a.point.z(),
                                  face_in_camera_space.b.point.z(),
                                  face_in_camera_space.c.point.z()),
            raster_depth_max);

        RasterPoint3d raster_point{.x = pixel.x, .y = pixel.y, .z = z_coordinate_in_buffer};
        if (!buffer.CanToAddVertex(raster_point)) {
            continue;
        }

        geometry::Vector3d interpolated_normal = InterpolateNormals(
            weights, global_face.a.normal, global_face.b.normal, global_face.c.normal);

        geometry::Point3d interpolated_point = InterpolatePoints(
            weights, global_face.a.point, global_face.b.point, global_face.c.point);

        TextureCoordinates interpolated_texture_coordinates = InterpolateTextureCoordinates(
            weights, global_face.a.text_coord, global_face.b.text_coord, global_face.c.text_coord);

        interpolated_texture_coordinates.h =
            std::max(0.0, std::min(1.0, interpolated_texture_coordinates.h));
        interpolated_texture_coordinates.w =
            std::max(0.0, std::min(1.0, interpolated_texture_coordinates.w));

        Color color_by_texture = texture.GetPixelColor(interpolated_texture_coordinates);

        buffer.TryToAddVertex(raster_point, PixelOriginInformation{.point = interpolated_point,
                                                                   .normal = interpolated_normal,
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
