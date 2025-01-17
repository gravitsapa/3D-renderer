#include "world.h"

void World::AddObject(const Object& object) {
    objects_.push_back(object);
}

void World::AddCamera(const Camera& camera) {
    cameras_.push_back(camera);
}
