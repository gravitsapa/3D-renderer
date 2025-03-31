#include <rasterizer.h>

namespace project {
namespace kernel {

void RasterizedFigure::AddRasterPoint(const RasterPoint2d& point) {
    data_.push_back(point);
}

const std::vector<RasterPoint2d>& RasterizedFigure::GetAllPoints() const {
    return data_;
}

RasterizedFigure RasterizeTriangleByXY(const Face& face, const RasterResolution& resolution) {
    RasterizedFigure figure;

    RasterPoint2d ra = ConvertToRasterPoint(geometry::TruncZ(face.a.point), resolution);
    RasterPoint2d rb = ConvertToRasterPoint(geometry::TruncZ(face.b.point), resolution);
    RasterPoint2d rc = ConvertToRasterPoint(geometry::TruncZ(face.c.point), resolution);

    detail::SortPointsByY(ra, rb, rc);

    if (ra.y == rc.y) {
        return figure;
    }

    RasterCoordinate height = rc.y - ra.y;
    for (RasterCoordinate h = 0; h <= height; ++h) {
        bool second_seg = h > rb.y - ra.y || rb.y == ra.y;
        RasterCoordinate seg_height = second_seg ? rc.y - rb.y : rb.y - ra.y;

        using Factor = float;
        Factor alpha = (Factor)(h) / height;
        Factor beta = (Factor)(second_seg ? h - rb.y + ra.y : h) / seg_height;

        RasterCoordinate alpha_x = ra.x + (rc.x - ra.x) * alpha;
        RasterCoordinate beta_x = second_seg ? rb.x + (rc.x - rb.x) * beta : ra.x + (rb.x - ra.x) * beta;

        if (alpha_x > beta_x) {
            std::swap(alpha_x, beta_x);
        }

        RasterCoordinate y = ra.y + h;
        for (RasterCoordinate x = alpha_x; x <= beta_x; x++) {
            figure.AddRasterPoint(RasterPoint2d{x, y});
        }
    }

    return figure;
}

namespace detail {
void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b, RasterPoint2d& c) {
    SortPointsByY(a, b);
    SortPointsByY(b, c);
    SortPointsByY(a, c);
}

void SortPointsByY(RasterPoint2d& a, RasterPoint2d& b) {
    if (a.y > b.y) {
        std::swap(a, b);
    }
}

}

}
}  // namespace project
