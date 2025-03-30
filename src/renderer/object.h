#pragma once

#include <vector>
#include <color.h>
#include <table.h>
#include <string>
#include <point.h>
#include <texture.h>
#include <vertex.h>
#include <mesh3d.h>

namespace project {
namespace kernel {

struct Object {
    Mesh3d mesh;
    Texture texture;
};

void PrintDebugInfo(const Object& object, const std::string& object_name);

}  // namespace kernel
}  // namespace project
