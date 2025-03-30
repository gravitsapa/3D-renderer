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

    Color GetColorOfPointByLight(Color tex_color) const;

private:
    Color light_color_;
};

class DirectionalLight {
public:
    DirectionalLight() = default;
    DirectionalLight(geometry::Vector3d direction);
    DirectionalLight(geometry::Vector3d direction_, Color col);

    Color GetColorOfPointByLight(Color tex_color, geometry::Vector3d normal) const;

private:
    Color light_color_;
    geometry::Vector3d direction_;
};

class Lights {
public:
    void AddAmbientLight(const AmbientLight& ambient_light);
    void AddDirectionalLight(const DirectionalLight& directional_light);

    Color GetColorOfPointByLight(Color tex_color, geometry::Vector3d normal) const;
    void Merge(const Lights& another_lights);

private:
    std::vector<AmbientLight> ambient_lights_;
    std::vector<DirectionalLight> directional_lights_;
};

}  // namespace kernel
}  // namespace project
