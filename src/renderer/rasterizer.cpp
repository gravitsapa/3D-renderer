#include <rasterizer.h>

namespace project {
namespace kernel {

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
    detail::SortPointsByY(a, b);
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

RasterizedFigure RasterizeTriangleByXY(const Face& face, const RasterResolution& resolution) {
    RasterPoint2d ra = ConvertToRasterPoint(geometry::TruncZ(face.a.point), resolution);
    RasterPoint2d rb = ConvertToRasterPoint(geometry::TruncZ(face.b.point), resolution);
    RasterPoint2d rc = ConvertToRasterPoint(geometry::TruncZ(face.c.point), resolution);

    detail::SortPointsByY(ra, rb, rc);

    auto line1 = BrezAlgo(ra, rc).GetAllSegments();
    auto line2 = BrezAlgo(ra, rb).GetAllSegments();
    auto line3 = BrezAlgo(rb, rc).GetAllSegments();

    int i1 = 0;
    int i2 = 0;
    int i3 = 0;

    RasterizedFigure figure;
    for (int y = ra.y; y <= rc.y; ++y) {
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

        HorizontalSegment segment = line1[i1];
        if (line2[i2].y == y) {
            segment = detail::Merge(segment, line2[i2]);
        }
        if (line3[i3].y == y) {
            segment = detail::Merge(segment, line3[i3]);
        }

        figure.AddRasterSegment(segment);
    }

    return figure;
}

namespace detail {

HorizontalSegment Merge(const HorizontalSegment& segment1, const HorizontalSegment& segment2) {
    return HorizontalSegment{segment1.y, std::min(segment1.left_x, segment2.left_x),
                             std::max(segment1.right_x, segment2.right_x)};
}

void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b, RasterPoint2d& c) {
    SortPointsByY(a, b);
    SortPointsByY(b, c);
    SortPointsByY(a, b);
}

void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b) {
    if (a.y > b.y) {
        std::swap(a, b);
    }
}

}  // namespace detail

}  // namespace kernel
}  // namespace project
