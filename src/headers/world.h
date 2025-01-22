#pragma once

#include <object.h>
#include <camera.h>
#include <vector>

struct Rotation {
    Matrix3d rot_matrix_ = EyeMatrix3d();
};

struct Position {
    Vector3d pos_vector_ = ZeroVector3d();
};

struct Pose {
    Position pos_;
    Rotation rot_;
};

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
    void AddObject(const Object& object);

    void AddCamera(const Camera& camera);

    const std::vector<PosedObject>& GetObjects() const;

    const std::vector<PosedCamera>& GetCameras() const;

private:
    std::vector<PosedObject> objects_;
    std::vector<PosedCamera> cameras_;
};
