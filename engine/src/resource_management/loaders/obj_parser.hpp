#pragma once

#include <array>
#include <limits>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace game_engine
{

class ObjParser final
{
public:

    struct Vertex
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 1.0f; // optional

        friend bool operator==(const Vertex&, const Vertex&) = default;
    };

    struct Point
    {
        float u = 0.0f;
        float v = 0.0f;
        float w = 1.0f; // optional

        friend bool operator==(const Point&, const Point&) = default;
    };

    struct Normal
    {
        float i = 0.0f;
        float j = 0.0f;
        float k = 0.0f;

        friend bool operator==(const Normal&, const Normal&) = default;
    };

    struct TextureVertex
    {
        float u = 0.0f;
        float v = 0.0f; // optional
        float w = 0.0f; // optional

        friend bool operator==(const TextureVertex&, const TextureVertex&) = default;
    };

    using IndexType = std::uint32_t;

    static constexpr IndexType InvalidIndex = std::numeric_limits<IndexType>::max();

    // 0-based index or InvalidIndex
    struct Triplet
    {
        IndexType vertex         = InvalidIndex;
        IndexType texture_vertex = InvalidIndex;
        IndexType normal         = InvalidIndex;

        friend bool operator==(const Triplet&, const Triplet&) = default;
    };

    using Face = std::vector<Triplet>;

    ObjParser();
    ~ObjParser();

    bool Parse(const std::string& source);

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<Point>& GetPoints() const;
    const std::vector<Normal>& GetNormals() const;
    const std::vector<TextureVertex>& GetTextureVertices() const;

    const std::vector<Face>& GetFaces() const;

    const std::vector<std::string>& GetMaterialLibrary() const;

private:

    using ParserFunction = void (ObjParser::*)(const std::vector<std::string_view>&);
    using ParsersMap     = std::unordered_map<std::string_view, ParserFunction>;

    static const ParsersMap& GetParsers();

    void Cleanup();

    void ParseVertex(const std::vector<std::string_view>& tokens);
    void ParsePoint(const std::vector<std::string_view>& tokens);
    void ParseNormal(const std::vector<std::string_view>& tokens);
    void ParseTextureVertex(const std::vector<std::string_view>& tokens);

    void ParseFace(const std::vector<std::string_view>& tokens);
    void ParseMaterialLibrary(const std::vector<std::string_view>& tokens);

    void NotImplementedStub(const std::vector<std::string_view>& tokens);

    std::vector<Vertex> m_vertices;
    std::vector<Point> m_points;
    std::vector<Normal> m_normals;
    std::vector<TextureVertex> m_texture_vertices;

    std::vector<Face> m_faces;

    std::vector<std::string> m_material_library;
};

} // namespace game_engine
