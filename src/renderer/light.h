#pragma once

#include <color.h>
#include <point.h>
#include <memory>

namespace project {
namespace kernel {

class AmbientLight {
public:
    AmbientLight();
    AmbientLight(Color col);

    Color GetColorOfPointByLight(const Color& tex_color) const;

private:
    Color light_color_;
};

class DirectionalLight {
public:
    using Factor = double;
    DirectionalLight() = default;
    DirectionalLight(geometry::Vector3d direction);
    DirectionalLight(geometry::Vector3d direction_, Color col);

    Color GetColorOfPointByLight(const Color& tex_color, const geometry::Vector3d& normal,
                                 const geometry::Vector3d& vector_to_viewer) const;

private:
    Color light_color_;
    geometry::Vector3d direction_;
    constexpr static Factor specular_exponent = 50;
};

class Lights {
public:
    void AddAmbientLight(const AmbientLight& ambient_light);
    void AddDirectionalLight(const DirectionalLight& directional_light);

    Color GetColorOfPointByLight(const Color& tex_color, const geometry::Vector3d& normal,
                                 const geometry::Vector3d& vector_to_viewer) const;
    void Merge(const Lights& another_lights);

private:
    std::vector<AmbientLight> ambient_lights_;
    std::vector<DirectionalLight> directional_lights_;
};

}  // namespace kernel
}  // namespace project
