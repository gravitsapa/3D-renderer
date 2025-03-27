#pragma once

#include <object.h>
#include <camera.h>
#include <vector>
#include <pose.h>

namespace project {
namespace kernel {

struct PosedObject : geometry::Pose, Object {};

struct PosedCamera : geometry::Pose, Camera {};

class World {
public:
    void AddObject(const Object& object, const geometry::Pose& pose = geometry::Pose());

    void AddCamera(const Camera& camera, const geometry::Pose& pose = geometry::Pose());

    const std::vector<PosedObject>& GetObjects() const;

    const std::vector<PosedCamera>& GetCameras() const;

private:
    std::vector<PosedObject> objects_;
    std::vector<PosedCamera> cameras_;
};

}  // namespace kernel
}  // namespace project
