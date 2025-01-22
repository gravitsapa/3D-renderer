#include <world.h>

void World::AddObject(const Object& object, const Pose& pose) {
    objects_.push_back({pose, object});
}

void World::AddCamera(const Camera& camera, const Pose& pose) {
    cameras_.push_back({pose, camera});
}

const std::vector<PosedObject>& World::GetObjects() const {
    return objects_;
}

const std::vector<PosedCamera>& World::GetCameras() const {
    return cameras_;
}