#pragma once

#include <object.h>
#include <camera.h>
#include <vector>
#include <pose.h>

namespace scene {

using namespace geometry;

struct PosedObject : Pose, Object {
};

struct PosedCamera : Pose, Camera {
};

class World {
public:
    void AddObject(const Object& object,
                   const Pose& pose = Pose());

    void AddCamera(const Camera& camera, const Pose& pose = Pose());

    const std::vector<PosedObject>& GetObjects() const;

    const std::vector<PosedCamera>& GetCameras() const;

private:
    std::vector<PosedObject> objects_;
    std::vector<PosedCamera> cameras_;
};

}  // namespace scene
