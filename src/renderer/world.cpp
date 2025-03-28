#include <world.h>

namespace project {
namespace kernel {

void World::AddObject(const Object& object, const geometry::Pose& pose) {
    objects_.push_back({pose, object});
}

void World::AddObject(Object&& object, geometry::Pose&& pose) {
    objects_.push_back({std::move(pose), std::move(object)});
}

void World::AddCamera(const Camera& camera, const geometry::Pose& pose) {
    cameras_.push_back({pose, camera});
}

const std::vector<PosedObject>& World::GetObjects() const {
    return objects_;
}

const std::vector<PosedCamera>& World::GetCameras() const {
    return cameras_;
}

}  // namespace kernel
}  // namespace project
