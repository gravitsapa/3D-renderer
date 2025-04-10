#pragma once

#include <vertex.h>
#include <face.h>

namespace project {
namespace kernel {

class Mesh3d {
    friend Mesh3d ReadMeshFromFile(const std::string& filename);

public:
    const std::vector<Face>& GetAllFaces() const;

    static Mesh3d RectangularСuboid(const geometry::Point3d& center, geometry::Coordinate w,
                                    geometry::Coordinate h, geometry::Coordinate d);
    static Mesh3d RectangularСuboid(geometry::Coordinate w, geometry::Coordinate h,
                                    geometry::Coordinate d);
    static Mesh3d Parallelepiped(const geometry::Point3d& center, const geometry::Vector3d& w,
                                 const geometry::Vector3d& h, const geometry::Vector3d d);
    static Mesh3d Rectangle(const geometry::Point3d& center, const geometry::Vector3d& h,
                            const geometry::Vector3d& w);
    static Mesh3d Triangle(const geometry::Point3d& a, const geometry::Point3d& b,
                           const geometry::Point3d& c);

    void Insert(Mesh3d&& rhs);

private:
    Mesh3d() = default;
    Mesh3d(std::vector<Face>&& faces);
    
    std::vector<Face> faces_;
};

}  // namespace kernel
}  // namespace project
