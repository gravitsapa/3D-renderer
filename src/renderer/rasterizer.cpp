#include <rasterizer.h>
#include <iostream>

namespace project {
namespace kernel {

VertexForRasterizer PrepareForRasterization(const geometry::Point3d& projected_point,
                                            const geometry::Coordinate& z_in_camera_view,
                                            const RasterResolution& resolution,
                                            const Vertex& global_vertex) {
    auto inv_z = 1 / z_in_camera_view;
    return VertexForRasterizer{
        ConvertToRasterPoint(geometry::TruncZ(projected_point), resolution),
        InformationToInterpolate{projected_point.z(), inv_z, global_vertex.normal * inv_z,
                                 global_vertex.point * inv_z, global_vertex.text_coord * inv_z}};
}

std::vector<VertexForRasterizer> RasterizeTriangleByXY(VertexForRasterizer a, VertexForRasterizer b,
                                                       VertexForRasterizer c) {

    std::vector<VertexForRasterizer> rasterized;
    detail::SortVerticesByY(a, b, c);

    RasterCoordinate height = c.y - a.y;
    InformationToInterpolate alpha_info = InformationToInterpolate(a);
    InformationToInterpolate alpha_info_step =
        (InformationToInterpolate(c) - InformationToInterpolate(a)) * (1.0 / height);

    InformationToInterpolate beta_info_first = InformationToInterpolate(a);
    InformationToInterpolate beta_info_first_step;
    if (b.y - a.y) {
        beta_info_first_step =
            (InformationToInterpolate(b) - InformationToInterpolate(a)) * (1.0 / (b.y - a.y));
    }

    InformationToInterpolate beta_info_second = InformationToInterpolate(b);
    InformationToInterpolate beta_info_second_step;
    if (c.y - b.y) {
        beta_info_second_step =
            (InformationToInterpolate(c) - InformationToInterpolate(b)) * (1.0 / (c.y - b.y));
    }

    for (RasterCoordinate h = 0; h <= height; ++h) {
        RasterCoordinate alpha_x = a.x;
        if (height > 0) {
            alpha_x = a.x + (c.x - a.x) * h / height;
        }

        if (h > 0) {
            alpha_info = alpha_info + alpha_info_step;
        }

        RasterCoordinate beta_x;
        InformationToInterpolate beta_info;

        if (a.y + h > b.y) {
            beta_x = b.x + (c.x - b.x) * (h - b.y + a.y) / (c.y - b.y);
            beta_info_second = beta_info_second + beta_info_second_step;
            beta_info = beta_info_second;
        } else {
            beta_x = a.x;
            if (b.y > a.y) {
                beta_x = a.x + (b.x - a.x) * h / (b.y - a.y);
            }
            if (h > 0) {
                beta_info_first = beta_info_first + beta_info_first_step;
            }
            beta_info = beta_info_first;
        }

        InformationToInterpolate left_info;
        InformationToInterpolate right_info;
        if (alpha_x <= beta_x) {
            left_info = alpha_info;
            right_info = beta_info;
        } else {
            left_info = beta_info;
            right_info = alpha_info;
            std::swap(alpha_x, beta_x);
        }

        RasterCoordinate y = a.y + h;
        InformationToInterpolate gamma_info = left_info;
        InformationToInterpolate gamma_info_step;
        if (beta_x > alpha_x) {
            gamma_info_step = (right_info - left_info) * (1.0 / (beta_x - alpha_x));
        }
        for (RasterCoordinate x = alpha_x; x <= beta_x; x++) {
            if (x > alpha_x) {
                gamma_info = gamma_info + gamma_info_step;
            }

            rasterized.push_back(VertexForRasterizer{RasterPoint2d{x, y}, gamma_info});
        }
    }

    return rasterized;
}

namespace detail {
void SortVerticesByY(VertexForRasterizer& a, VertexForRasterizer& b, VertexForRasterizer& c) {
    SortVerticesByY(a, b);
    SortVerticesByY(b, c);
    SortVerticesByY(a, b);
}

void SortVerticesByY(VertexForRasterizer& a, VertexForRasterizer& b) {
    if (a.y > b.y) {
        std::swap(a, b);
    }
}

}  // namespace detail

}  // namespace kernel
}  // namespace project
