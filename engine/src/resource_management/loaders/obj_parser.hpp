#pragma once

#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <engine/graphics/mesh.hpp>

namespace game_engine
{

class ObjParser final
{
public:

    ObjParser();
    ~ObjParser();

    std::tuple<VertexData, std::vector<SubMesh>> Parse(std::string source);

private:

    struct Vertex
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 1.0f;
    };

    struct Point
    {
        float u = 0.0f;
        float v = 0.0f;
        float w = 1.0f;
    };

    struct Normal
    {
        float i = 0.0f;
        float j = 0.0f;
        float k = 0.0f;
    };

    struct TextureVertex
    {
        float u = 0.0f;
        float v = 0.0f;
        float w = 0.0f;
    };

    using ParserFunction = void (ObjParser::*)(const std::vector<std::string_view>&);
    using ParsersMap     = std::unordered_map<std::string_view, ParserFunction>;

    static const ParsersMap& GetParsers();

    void Cleanup();

    void ParseVertex(const std::vector<std::string_view>& tokens);
    void ParsePoint(const std::vector<std::string_view>& tokens);
    void ParseNormal(const std::vector<std::string_view>& tokens);
    void ParseTextureVertex(const std::vector<std::string_view>& tokens);

    void ParseFace(const std::vector<std::string_view>& tokens);

    void NotImplementedStub(const std::vector<std::string_view>& tokens);

    std::vector<Vertex> m_vertices;
    std::vector<Point> m_points;
    std::vector<Normal> m_normals;
    std::vector<TextureVertex> m_texture_vertices;
    std::vector<SubMesh> m_submeshes;

    SubMesh m_current_submesh;
};

} // namespace game_engine
