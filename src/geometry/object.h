#pragma once

#include <polygon.h>
#include <vector>

namespace geometry {

class Object {
public:
    void AddPolygon(const Polygon& polygon);

    const std::vector<Polygon>& GetPolygons() const;

    void Clear();

    static Object RectangularСuboid(const Point3d& center, Coordinate w, Coordinate h,
                                    Coordinate d);

    static Object RectangularСuboid(Coordinate w, Coordinate h, Coordinate d);

    static Object Parallelepiped(const Point3d& center, const Vector3d& w, const Vector3d& h,
                                 const Vector3d d);

    static Object Rectangle(const Point3d& center, const Vector3d& h, const Vector3d& w,
                            const Color& color);

    static Object Triangle(const Point3d& a, const Point3d& b, const Point3d& c,
                           const Color& color);

    void Insert(Object&& rhs);

private:
    std::vector<Polygon> polygons_;
};

}  // namespace geometry
