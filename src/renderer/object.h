#pragma once

#include <polygon.h>
#include <vector>
#include <point.h>

namespace project {

namespace kernel {

class Object {
public:
    void AddPolygon(const Polygon& polygon);
    const std::vector<Polygon>& GetPolygons() const;
    void Merge(Object&& rhs);

    static Object RectangularСuboid(const geometry::Point3d& center, geometry::Coordinate w,
                                    geometry::Coordinate h, geometry::Coordinate d);
    static Object RectangularСuboid(geometry::Coordinate w, geometry::Coordinate h,
                                    geometry::Coordinate d);
    static Object Parallelepiped(const geometry::Point3d& center, const geometry::Vector3d& w,
                                 const geometry::Vector3d& h, const geometry::Vector3d d);
    static Object Rectangle(const geometry::Point3d& center, const geometry::Vector3d& h,
                            const geometry::Vector3d& w, const Color& color);
    static Object Triangle(const geometry::Point3d& a, const geometry::Point3d& b,
                           const geometry::Point3d& c, const Color& color);

private:
    std::vector<Polygon> polygons_;
};

}  // namespace kernel
}  // namespace project
