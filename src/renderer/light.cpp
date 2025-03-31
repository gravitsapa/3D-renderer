#include <light.h>

namespace project {
namespace kernel {

AmbientLight::AmbientLight() : light_color_(light_color_) {
}

AmbientLight::AmbientLight(Color col) : light_color_(std::move(col)) {
}

Color AmbientLight::GetColorOfPointByLight(const Color& tex_color) const {
    return tex_color * light_color_;
}

void Lights::AddAmbientLight(const AmbientLight& ambient_light) {
    ambient_lights_.push_back(ambient_light);
}

DirectionalLight::DirectionalLight(geometry::Vector3d direction)
    : direction_(geometry::Normalized(direction)), light_color_(Color::White()) {
}
DirectionalLight::DirectionalLight(geometry::Vector3d direction, Color color)
    : direction_(geometry::Normalized(direction)), light_color_(std::move(color)) {
}

Color DirectionalLight::GetColorOfPointByLight(const Color& tex_color,
                                               const geometry::Vector3d& normal,
                                               const geometry::Vector3d& vector_to_viewer) const {
    auto normal_dot_direction = -direction_.dot(geometry::Normalized(normal));
    if (normal_dot_direction <= 0) {
        return Color::Black();
    }
    auto half_vector = geometry::Normalized(vector_to_viewer - direction_);
    return tex_color * light_color_ * normal_dot_direction +
           light_color_ * std::pow(std::max(normal.dot(half_vector), 0.0), specular_exponent);
}

Color Lights::GetColorOfPointByLight(const Color& tex_color,
                                               const geometry::Vector3d& normal,
                                               const geometry::Vector3d& vector_to_viewer) const {
    Color res = Color::Black();
    for (auto& ambient_light : ambient_lights_) {
        res = res + ambient_light.GetColorOfPointByLight(tex_color);
    }
    for (auto& directional_light : directional_lights_) {
        res = res + directional_light.GetColorOfPointByLight(tex_color, normal, vector_to_viewer);
    }
    return res;
}

void Lights::AddDirectionalLight(const DirectionalLight& directional_light) {
    directional_lights_.push_back(directional_light);
}

void Lights::Merge(const Lights& another_lights) {
    ambient_lights_.insert(ambient_lights_.end(), another_lights.ambient_lights_.begin(),
                           another_lights.ambient_lights_.end());
    directional_lights_.insert(directional_lights_.end(),
                               another_lights.directional_lights_.begin(),
                               another_lights.directional_lights_.end());
}

}  // namespace kernel
}  // namespace project
