#include <game_engine/core/mesh.hpp>

namespace game_engine::core
{

Vector3 BoundingBox::getCenter() const {
    return (min + max) * 0.5f;
}

Vector3 BoundingBox::getSize() const {
    return max - min;
}

// TODO: Add bounding box rendering
void Mesh::calculateBoundingBox() {
    if (vertices.empty()) {
        boundingBox = {Vector3(0.0f), Vector3(0.0f)};
        return;
    }

    Vector3 min = vertices[0].position;
    Vector3 max = vertices[0].position;

    for (const auto& vertex : vertices) {
        min.x = std::min(min.x, vertex.position.x);
        min.y = std::min(min.y, vertex.position.y);
        min.z = std::min(min.z, vertex.position.z);

        max.x = std::max(max.x, vertex.position.x);
        max.y = std::max(max.y, vertex.position.y);
        max.z = std::max(max.z, vertex.position.z);
    }

    boundingBox = {min, max};
}

} // namespace game_engine::core
