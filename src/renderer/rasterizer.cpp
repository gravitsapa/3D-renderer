#include <rasterizer.h>
#include <iostream>

namespace project {
namespace kernel {

VertexForRasterizer PrepareForRasterization(const geometry::Point3d projected_point,
                                            const geometry::Coordinate z_in_camera_view,
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
    detail::SortVerticesByY(a, b, c);

    auto line1 = detail::BrezAlgo(a, c).GetAllSegments();
    auto line2 = detail::BrezAlgo(a, b).GetAllSegments();
    auto line3 = detail::BrezAlgo(b, c).GetAllSegments();

    int i1 = 0;
    int i2 = 0;
    int i3 = 0;

    std::vector<VertexForRasterizer> rasterized;
    for (int y = a.y; y <= c.y; ++y) {
        if (line1[i1].y < y) {
            i1++;
            assert(line1[i1].y == y);
        }
        if (line2[i2].y < y && i2 + 1 < line2.size()) {
            ++i2;
        }
        if (line3[i3].y < y) {
            i3++;
            assert(line3[i3].y == y);
        }

        detail::HorizontalSegment segment = line1[i1];
        if (line2[i2].y == y) {
            segment = detail::Merge(segment, line2[i2]);
        }
        if (line3[i3].y == y) {
            segment = detail::Merge(segment, line3[i3]);
        }

        Factor alpha = detail::GetFactorByPointInSegment(y, a.y, c.y);
        Factor beta;
        if (y <= b.y) {
            beta = detail::GetFactorByPointInSegment(y, a.y, b.y);
            for (RasterCoordinate x = segment.left_x; x <= segment.right_x; ++x) {
                Factor gamma =
                    detail::GetFactorByPointInSegment(x, segment.left_x, segment.right_x);
                if (c.x > b.x)
                    gamma = 1 - gamma;
                VertexWeights weights{(1 - alpha) * (1 - gamma) + (1 - beta) * gamma, beta * gamma,
                                      alpha * (1 - gamma)};
                // std::cerr << alpha << ' ' << beta << ' ' << gamma << " ??? " << weights.a << ' '
                //           << weights.b << ' ' << weights.c << std::endl;
                // VertexWeights weights = {1, 0, 0};

                rasterized.push_back(VertexForRasterizer{RasterPoint2d{x, y},
                                                         GetWeightedInformation(weights, a, b, c)});
            }
        } else {
            beta = detail::GetFactorByPointInSegment(y, b.y, c.y);
            for (RasterCoordinate x = segment.left_x; x <= segment.right_x; ++x) {
                Factor gamma =
                    detail::GetFactorByPointInSegment(x, segment.left_x, segment.right_x);
                if (c.x > b.x)
                    gamma = (1 - gamma);
                VertexWeights weights{(1 - alpha) * (1 - gamma), (1 - beta) * gamma,
                                      alpha * (1 - gamma) + beta * gamma};
                // std::cerr << alpha << ' ' << beta << ' ' << gamma << " !!! " << weights.a << ' '
                //           << weights.b << ' ' << weights.c << std::endl;
                // VertexWeights weights = {1, 0, 0};
                rasterized.push_back(VertexForRasterizer{RasterPoint2d{x, y},
                                                         GetWeightedInformation(weights, a, b, c)});
            }
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
