#pragma once

#include <object.h>
#include <camera.h>
#include <vector>
#include <pose.h>
#include <light.h>

namespace project {
namespace kernel {

struct PosedObject : geometry::Pose, Object {};

struct PosedCamera : geometry::Pose, Camera {};

class World {
    friend class Renderer;
public:
    void AddObject(const Object& object, const geometry::Pose& pose = geometry::Pose());
    void AddCamera(const Camera& camera, const geometry::Pose& pose = geometry::Pose());
    void AddLights(const Lights& lights);

    const PosedCamera& GetCamera(size_t index);
    void SetCamera(size_t index, const PosedCamera& camera);
    void MoveCamera(size_t index, const geometry::Pose& pose);

    const PosedObject& GetObject(size_t index);
    void SetObject(size_t index, const PosedObject& camera);
    void MoveObject(size_t index, const geometry::Pose& pose);

private:
    const std::vector<PosedObject>& GetObjects() const;
    const std::vector<PosedCamera>& GetCameras() const;
    const Lights& GetLights() const;

    std::vector<PosedObject> objects_;
    std::vector<PosedCamera> cameras_;
    Lights lights_;
};

}  // namespace kernel
}  // namespace project
