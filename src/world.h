#pragma once

#include "object.h"
#include "camera.h"
#include <vector>

class World {
public:
    void AddObject(const Object& object);

    void AddCamera(const Camera& camera);

private:
    std::vector<Object> objects_;
    std::vector<Camera> cameras_;
};
