#include <world.h>

namespace project {
namespace kernel {

void World::AddObject(const Object& object, const geometry::Pose& pose) {
    objects_.push_back({pose, object});
}

void World::AddCamera(const Camera& camera, const geometry::Pose& pose) {
    cameras_.push_back({pose, camera});
}

void World::AddLights(const Lights& lights) {
    lights_.Merge(lights);
}

const std::vector<PosedObject>& World::GetObjects() const {
    return objects_;
}

const std::vector<PosedCamera>& World::GetCameras() const {
    return cameras_;
}

const Lights& World::GetLights() const {
    return lights_;
}

}  // namespace kernel
}  // namespace project
