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

    if (a.y == c.y) {
        return {};
    }

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

    for (RasterCoordinate h = 0; h < height; ++h) {
        bool second_seg = h > b.y - a.y || b.y == a.y;
        RasterCoordinate seg_height = second_seg ? c.y - b.y : b.y - a.y;

        Factor alpha = (Factor)(h) / height;
        Factor beta = (Factor)(second_seg ? h - b.y + a.y : h) / seg_height;

        RasterCoordinate alpha_x = a.x + (c.x - a.x) * alpha;
        if (h > 0) {
            alpha_info = alpha_info + alpha_info_step;
        }

        RasterCoordinate beta_x;
        InformationToInterpolate beta_info;

        if (second_seg) {
            beta_x = b.x + (c.x - b.x) * beta;
            if (a.y + h > b.y) {
                beta_info_second = beta_info_second + beta_info_second_step;
            }
            beta_info = beta_info_second;
        } else {
            beta_x = a.x + (b.x - a.x) * beta;
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

void RasterizedFigure::AddRasterSegment(const HorizontalSegment& point) {
    data_.push_back(point);
}

const std::vector<HorizontalSegment>& RasterizedFigure::GetAllSegments() const {
    return data_;
}

void RasterizedFigure::Merge(const RasterizedFigure& another_figure) {
    data_.insert(data_.end(), another_figure.data_.begin(), another_figure.data_.end());
}

RasterizedFigure BrezAlgo(RasterPoint2d a, RasterPoint2d b) {
    assert(a.y <= b.y);
    RasterizedFigure line;

    int x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y;
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = x2 >= x1 ? 1 : -1;
    int sy = y2 >= y1 ? 1 : -1;

    if (dy <= dx) {
        int d = 2 * dy - dx;
        int d1 = 2 * dy;
        int d2 = (dy - dx) * 2;

        HorizontalSegment segment{y1, x1, x1};
        int x = x1 + sx;
        for (int y = y1, i = 1; i <= dx; i++, x += sx) {
            if (d > 0) {
                d += d2;
                if (sx == 1) {
                    segment.right_x = x - sx;
                } else {
                    segment.left_x = x - sx;
                }
                line.AddRasterSegment(segment);
                y += sy;
                segment = HorizontalSegment{y, x, x};
            } else {
                d += d1;
            }
        }
        if (sx == 1) {
            segment.right_x = x - sx;
        } else {
            segment.left_x = x - sx;
        }
        line.AddRasterSegment(segment);
    } else {
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = (dx - dy) * 2;

        line.AddRasterSegment(HorizontalSegment{y1, x1, x1});
        for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy) {
            if (d > 0) {
                d += d2;
                x += sx;
            } else {
                d += d1;
            }
            line.AddRasterSegment(HorizontalSegment{y, x, x});
        }
    }

    return line;
}

Factor GetFactorByPointInSegment(RasterCoordinate x, RasterCoordinate begin, RasterCoordinate end) {
    assert(begin <= x && x <= end);
    return end > begin ? static_cast<Factor>(x - begin) / (end - begin) : 0;
}

HorizontalSegment Merge(const HorizontalSegment& segment1, const HorizontalSegment& segment2) {
    return HorizontalSegment{segment1.y, std::min(segment1.left_x, segment2.left_x),
                             std::max(segment1.right_x, segment2.right_x)};
}

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
