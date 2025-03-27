#pragma once

#include <polygon.h>
#include <vector>
#include <point.h>

namespace project {

namespace kernel {

class Object {
public:
    void AddPolygon(const Triangle& polygon);
    const std::vector<Triangle>& GetPolygons() const;
    void Merge(Object&& rhs);

    static Object CreateRectangularСuboid(const geometry::Point3d& center, geometry::Coordinate w,
                                    geometry::Coordinate h, geometry::Coordinate d);
    static Object CreateRectangularСuboid(geometry::Coordinate w, geometry::Coordinate h,
                                    geometry::Coordinate d);
    static Object CreateParallelepiped(const geometry::Point3d& center, const geometry::Vector3d& w,
                                 const geometry::Vector3d& h, const geometry::Vector3d d);
    static Object CreateRectangle(const geometry::Point3d& center, const geometry::Vector3d& h,
                            const geometry::Vector3d& w, const Color& color);
    static Object CreateTriangle(const geometry::Point3d& a, const geometry::Point3d& b,
                           const geometry::Point3d& c, const Color& color);

private:
    std::vector<Triangle> polygons_;
};

}  // namespace kernel
}  // namespace project
