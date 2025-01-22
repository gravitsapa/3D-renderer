#pragma once

#include <polygon.h>
#include <vector>

class Object {
public:
    void AddPolygon(const Polygon& polygon);

    const std::vector<Polygon>& GetPolygons() const;

    void Clear();

    static Object RectangularСuboid(const Point3d& center, Coordinate w, Coordinate h, Coordinate d) {
        w /= 2;
        h /= 2;
        d /= 2;

        return Parallelepiped(center, {w, 0, 0}, {0, h, 0}, {0, 0, d});
    }

    static Object RectangularСuboid(Coordinate w, Coordinate h, Coordinate d) {
        return RectangularСuboid(Point3d(), w, h, d);
    }

    static Object Parallelepiped(const Point3d& center, const Vector3d& w, const Vector3d& h,
                                 const Vector3d d) {
        Object res;
        res += Rectangle(center + w, h, d, Color::Random());
        res += Rectangle(center - w, h, d, Color::Random());
        res += Rectangle(center + h, w, d, Color::Random());
        res += Rectangle(center - h, w, d, Color::Random());
        res += Rectangle(center + d, h, w, Color::Random());
        res += Rectangle(center - d, h, w, Color::Random());
        return res;
    }

    static Object Rectangle(const Point3d& center, const Vector3d& h, const Vector3d& w,
                            const Color& color) {
        Object res;
        res.AddPolygon({center + h + w, center - h - w, center + h - w, color});
        res.AddPolygon({center + h + w, center - h - w, center - h + w, color});
        return res;
    }

    static Object Triangle(const Point3d& a, const Point3d& b, const Point3d& c,
                           const Color& color) {
        Object res;
        res.AddPolygon({a, b, c, color});
        return res;
    }

    Object& operator+=(Object&& rhs) {
        if (polygons_.size() > rhs.polygons_.size()) {
            polygons_.swap(rhs.polygons_);
        }

        while (!rhs.polygons_.empty()) {
            polygons_.push_back(rhs.polygons_.back());
            rhs.polygons_.pop_back();
        }

        return *this;
    }

private:
    std::vector<Polygon> polygons_;
};
