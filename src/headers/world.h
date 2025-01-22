#pragma once

#include <object.h>
#include <camera.h>
#include <vector>
#include <pose.h>

struct PosedObject {
    Pose pose_;
    Object obj_;
};

struct PosedCamera {
    Pose pose_;
    Camera cam_;
};

class World {
public:
    void AddObject(const Object& object, const Pose& pose = {Position(), Rotation()});

    void AddCamera(const Camera& camera, const Pose& pose = {Position(), Rotation()});

    const std::vector<PosedObject>& GetObjects() const;

    const std::vector<PosedCamera>& GetCameras() const;

private:
    std::vector<PosedObject> objects_;
    std::vector<PosedCamera> cameras_;
};
