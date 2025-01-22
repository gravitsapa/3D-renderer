#include <world.h>

void World::AddObject(const Object& object) {
    objects_.push_back({{Position(), Rotation()}, object});
}

void World::AddCamera(const Camera& camera) {
    cameras_.push_back({{Position(), Rotation()}, camera});
}

const std::vector<PosedObject>& World::GetObjects() const {
    return objects_;
}

const std::vector<PosedCamera>& World::GetCameras() const {
    return cameras_;
}