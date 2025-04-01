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

const PosedCamera& World::GetCamera(size_t index) {
    return cameras_[index];
}
void World::SetCamera(size_t index, const PosedCamera& camera){
    cameras_[index] = camera;
}
void World::MoveCamera(size_t index, const geometry::Pose& pose) {
    cameras_[index].pos_vector = pose.pos_vector;
    cameras_[index].rot_matrix = pose.rot_matrix;
}

const PosedObject& World::GetObject(size_t index) {
    return objects_[index];
}
void World::SetObject(size_t index, const PosedObject& camera) {
    objects_[index] = camera;
}
void World::MoveObject(size_t index, const geometry::Pose& pose) {
    objects_[index].pos_vector = pose.pos_vector;
    objects_[index].rot_matrix = pose.rot_matrix;
}

}  // namespace kernel
}  // namespace project
