#include "obj_parser.hpp"

#include <regex>
#include <sstream>
#include <string_view>

#include <engine/graphics/vertex_traits.hpp>

namespace
{

inline std::vector<std::string_view> SplitToTokens(const std::string& line)
{
    std::vector<std::string_view> tokens;
    std::size_t begin = 0;

    while (begin < line.size()) {
        begin = line.find_first_not_of(" \t", begin);
        if (begin == std::string_view::npos) {
            break;
        }

        std::size_t end = line.find_first_of(" \t", begin);
        if (end == std::string_view::npos) {
            end = line.size();
        }

        tokens.push_back({&line[begin], end - begin});

        begin = end;
    }

    return tokens;
}

template <typename T>
requires(std::same_as<T, int> || std::same_as<T, float>)
inline bool ParseValue(std::string_view s, T& value)
{
    auto result = std::from_chars(s.data(), s.data() + s.size(), value);
    return (result.ec == std::errc() && result.ptr == s.data() + s.size());
}

} // namespace

namespace game_engine
{

ObjParser::ObjParser() = default;

ObjParser::~ObjParser() = default;

// https://www.fileformat.info/format/material/
// https://www.martinreddy.net/gfx/3d/OBJ.spec
// https://paulbourke.net/dataformats/obj/
std::tuple<VertexData, std::vector<SubMesh>> ObjParser::Parse(std::string source)
{
    // Canonical line endings
    source = std::regex_replace(source, std::regex(R"~((\r\n))~"), "\n");
    // Remove comments
    source = std::regex_replace(source, std::regex(R"~((\s*#.*))~"), "");
    // Concat lines with continuation
    source = std::regex_replace(source, std::regex(R"~((\s*\\\s*\n))~"), "");

    std::istringstream iss_source(source);

    std::string line;
    while (std::getline(iss_source, line)) {
        if (line.empty()) {
            continue;
        }
        const auto tokens = SplitToTokens(line);
        if (tokens.empty()) {
            continue;
        }

        const auto& map = GetParsers();
        if (auto it = map.find(tokens[0]); it != map.end()) {
            auto ptr = it->second;
            (this->*ptr)(tokens);
        }
    }

    if (!m_current_submesh.indices.empty()) {
        m_submeshes.push_back(std::move(m_current_submesh));
    }

    std::tuple<VertexData, std::vector<SubMesh>> result = std::make_tuple(vertex_traits::ConvertToVertexData(m_vertices,
                                                                              {
                                                                                  {0, 4, 0, VertexAttributeType::Float, false, "position"}
    }),
        std::move(m_submeshes)

    );

    Cleanup();

    return result;
} // namespace game_engine

const ObjParser::ParsersMap& ObjParser::GetParsers()
{
    static const ParsersMap map = []() -> ParsersMap {
        return {
            // Vertex data
            {         "v",        &ObjParser::ParseVertex},
            {        "vp",         &ObjParser::ParsePoint},
            {        "vn",        &ObjParser::ParseNormal},
            {        "vt", &ObjParser::ParseTextureVertex},

            {    "cstype", &ObjParser::NotImplementedStub},
            {       "deg", &ObjParser::NotImplementedStub},
            {      "bmat", &ObjParser::NotImplementedStub},
            {      "step", &ObjParser::NotImplementedStub},

            // Elements
            {         "p", &ObjParser::NotImplementedStub},
            {         "l", &ObjParser::NotImplementedStub},
            {         "f",          &ObjParser::ParseFace},
            {      "curv", &ObjParser::NotImplementedStub},
            {     "curv2", &ObjParser::NotImplementedStub},
            {      "surf", &ObjParser::NotImplementedStub},

            // Free-form curve/surface body statements
            {      "parm", &ObjParser::NotImplementedStub},
            {      "trim", &ObjParser::NotImplementedStub},
            {      "hole", &ObjParser::NotImplementedStub},
            {      "scrv", &ObjParser::NotImplementedStub},
            {        "sp", &ObjParser::NotImplementedStub},
            {       "end", &ObjParser::NotImplementedStub},

            // Connectivity between free-form surfaces
            {       "con", &ObjParser::NotImplementedStub},

            // Grouping
            {         "g", &ObjParser::NotImplementedStub},
            {         "s", &ObjParser::NotImplementedStub},
            {        "mg", &ObjParser::NotImplementedStub},
            {         "o", &ObjParser::NotImplementedStub},

            // Display/render attributes
            {     "bevel", &ObjParser::NotImplementedStub},
            {  "c_interp", &ObjParser::NotImplementedStub},
            {  "d_interp", &ObjParser::NotImplementedStub},
            {       "lod", &ObjParser::NotImplementedStub},
            {    "maplib", &ObjParser::NotImplementedStub},
            {    "usemap", &ObjParser::NotImplementedStub},
            {    "usemtl", &ObjParser::NotImplementedStub},
            {    "mtllib", &ObjParser::NotImplementedStub},
            {"shadow_obj", &ObjParser::NotImplementedStub},
            { "trace_obj", &ObjParser::NotImplementedStub},
            {     "ctech", &ObjParser::NotImplementedStub},
            {     "stech", &ObjParser::NotImplementedStub},
        };
    }();

    return map;
}

void ObjParser::Cleanup()
{
    m_vertices.clear();
    m_points.clear();
    m_normals.clear();
    m_texture_vertices.clear();
    m_submeshes.clear();

    m_current_submesh = {};
}

void ObjParser::ParseVertex(const std::vector<std::string_view>& tokens)
{
    // Specifies a geometric vertex and its x y z coordinates.

    if (tokens.size() < 4) {
        throw std::runtime_error("Vertex data invalid format");
    }

    Vertex tmp;
    if (!ParseValue<float>(tokens[1], tmp.x) || !ParseValue<float>(tokens[2], tmp.y) || !ParseValue<float>(tokens[3], tmp.z)) {
        throw std::runtime_error("Vertex data value parsing error");
    }

    if (tokens.size() > 4) {
        ParseValue<float>(tokens[4], tmp.w);
    }

    m_vertices.push_back(tmp);
}

void ObjParser::ParsePoint(const std::vector<std::string_view>& tokens)
{
    // Specifies a point in the parameter space of a curve or surface.

    if (tokens.size() < 3) {
        throw std::runtime_error("Point data invalid format");
    }

    Point tmp;
    if (!ParseValue<float>(tokens[1], tmp.u) || !ParseValue<float>(tokens[2], tmp.v)) {
        throw std::runtime_error("Point data value parsing error");
    }

    if (tokens.size() > 3) {
        ParseValue<float>(tokens[3], tmp.w);
    }

    m_points.push_back(tmp);
}

void ObjParser::ParseNormal(const std::vector<std::string_view>& tokens)
{
    // Specifies a normal vector with components i, j, and k.

    if (tokens.size() < 4) {
        throw std::runtime_error("Normal data invalid format");
    }

    Normal tmp;
    if (!ParseValue<float>(tokens[1], tmp.i) || !ParseValue<float>(tokens[2], tmp.j) || !ParseValue<float>(tokens[3], tmp.k)) {
        throw std::runtime_error("Normal data value parsing error");
    }

    m_normals.push_back(tmp);
}

void ObjParser::ParseTextureVertex(const std::vector<std::string_view>& tokens)
{
    // Specifies a texture vertex and its coordinates.

    if (tokens.size() < 2) {
        throw std::runtime_error("TextureVertex data invalid format");
    }

    TextureVertex tmp;
    if (!ParseValue<float>(tokens[1], tmp.u)) {
        throw std::runtime_error("TextureVertex data value parsing error");
    }

    if (tokens.size() > 2) {
        ParseValue<float>(tokens[2], tmp.v);
    }

    if (tokens.size() > 3) {
        ParseValue<float>(tokens[3], tmp.w);
    }

    m_texture_vertices.push_back(tmp);
}

void ObjParser::ParseFace(const std::vector<std::string_view>& tokens)
{
    // Specifies a face element and its vertex reference number.

    if (tokens.size() < 4) {
        throw std::runtime_error("TextureVertex data invalid format");
    }

    if (tokens.size() > 5) {
        throw std::runtime_error("Using n-gons in your 3d model? You deserve a slap in the face with a tuna for that!");
    }

    std::vector<int> indices;

    for (std::size_t i = 1; i < tokens.size(); ++i) {
        std::string_view token = tokens[i];

        int v;
        if (!ParseValue<int>(token, v)) {
            throw std::runtime_error("Vertex index value parsing error");
        }

        indices.push_back(v);
    }

    m_current_submesh.indices.push_back(indices[0] - 1);
    m_current_submesh.indices.push_back(indices[1] - 1);
    m_current_submesh.indices.push_back(indices[2] - 1);

    if (indices.size() == 4) {
        m_current_submesh.indices.push_back(indices[2] - 1);
        m_current_submesh.indices.push_back(indices[3] - 1);
        m_current_submesh.indices.push_back(indices[0] - 1);
    }
}

void ObjParser::NotImplementedStub(const std::vector<std::string_view>& tokens)
{
    throw std::runtime_error("Not implemented");
}

} // namespace game_engine
