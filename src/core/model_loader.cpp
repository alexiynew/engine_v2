#include <fstream>
#include <iostream>
#include <sstream>

#include <game_engine/core/model_loader.hpp>

namespace game_engine::core
{

// TODO: Test model loading fom obj.files with materials

std::shared_ptr<Mesh> ModelLoader::loadModel(const std::filesystem::path& filePath)
{
    // Check if the model is already loaded
    if (m_modelCache.find(filePath) != m_modelCache.end()) {
        return m_modelCache[filePath];
    }

    // Load the model from the file (example for a simple .obj file)
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open model file: " << filePath << std::endl;
        return nullptr;
    }

    auto mesh = std::make_shared<Mesh>();
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<unsigned int> indices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vector3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (type == "vn") {
            Vector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (type == "vt") {
            Vector2 uv;
            iss >> uv.x >> uv.y;
            uvs.push_back(uv);
        } else if (type == "f") {
            unsigned int v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            indices.push_back(v1 - 1);
            indices.push_back(v2 - 1);
            indices.push_back(v3 - 1);
        }
    }

    // Fill the mesh with loaded data
    for (size_t i = 0; i < positions.size(); ++i) {
        Vertex vertex;
        vertex.position = positions[i];
        vertex.normal   = (i < normals.size()) ? normals[i] : Vector3(0.0f, 0.0f, 1.0f);
        vertex.uv       = (i < uvs.size()) ? uvs[i] : Vector2(0.0f, 0.0f);
        vertex.color    = Vector3(1.0f, 1.0f, 1.0f); // Default color
        mesh->vertices.push_back(vertex);
    }

    SubMesh subMesh;
    subMesh.indices = indices;
    mesh->subMeshes.push_back(subMesh);

    // Cache the loaded model
    m_modelCache[filePath] = mesh;

    return mesh;
}

} // namespace game_engine::core
