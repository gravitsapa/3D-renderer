#include <object.h>

namespace project {

namespace kernel {

void Object::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

const std::vector<Polygon>& Object::GetPolygons() const {
    return polygons_;
}

Object Object::RectangularСuboid(const geometry::Point3d& center, geometry::Coordinate w,
                                 geometry::Coordinate h, geometry::Coordinate d) {
    w /= 2;
    h /= 2;
    d /= 2;

    return Parallelepiped(center, {w, 0, 0}, {0, h, 0}, {0, 0, d});
}

Object Object::RectangularСuboid(geometry::Coordinate w, geometry::Coordinate h,
                                 geometry::Coordinate d) {
    return RectangularСuboid(geometry::Point3d(), w, h, d);
}

Object Object::Parallelepiped(const geometry::Point3d& center, const geometry::Vector3d& w,
                              const geometry::Vector3d& h, const geometry::Vector3d d) {
    Object res;
    res.Merge(Rectangle(center + w, h, d, Color::Random()));
    res.Merge(Rectangle(center - w, h, d, Color::Random()));
    res.Merge(Rectangle(center + h, w, d, Color::Random()));
    res.Merge(Rectangle(center - h, w, d, Color::Random()));
    res.Merge(Rectangle(center + d, h, w, Color::Random()));
    res.Merge(Rectangle(center - d, h, w, Color::Random()));
    return res;
}

Object Object::Rectangle(const geometry::Point3d& center, const geometry::Vector3d& h,
                         const geometry::Vector3d& w, const Color& color) {
    Object res;
    res.AddPolygon({center + h + w, center - h - w, center + h - w, color});
    res.AddPolygon({center + h + w, center - h - w, center - h + w, color});
    return res;
}

Object Object::Triangle(const geometry::Point3d& a, const geometry::Point3d& b,
                        const geometry::Point3d& c, const Color& color) {
    Object res;
    res.AddPolygon({a, b, c, color});
    return res;
}

void Object::Merge(Object&& rhs) {
    polygons_.insert(polygons_.end(), rhs.polygons_.begin(), rhs.polygons_.end());
    rhs.polygons_.clear();
}

}  // namespace kernel
}  // namespace project
