#include "obj_parser.hpp"

#include <array>
#include <regex>
#include <sstream>
#include <string_view>

namespace
{

std::vector<std::string_view> SplitToTokens(const std::string& line)
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

template <typename T>
requires(std::same_as<T, int> || std::same_as<T, float>)
bool ParseTriplet(std::string_view str, std::array<T, 3>& triplet)
{
    std::size_t slash1 = str.find('/');
    std::size_t slash2 = str.find('/', slash1 + 1);

    std::array<T, 3> tmp = {0, 0, 0};

    // No slashes - 1 value
    if (slash1 == str.npos) {
        if (!ParseValue<T>(str, tmp[0])) {
            return false;
        }
    }

    // One slash - 2 values
    if (slash1 != str.npos && slash2 == str.npos) {
        auto part1 = str.substr(0, slash1);
        auto part2 = str.substr(slash1 + 1);

        if (!ParseValue<T>(part1, tmp[0]) || !ParseValue<T>(part2, tmp[1])) {
            return false;
        }
    }

    // Two slashes - 2 or 3 values
    if (slash1 != str.npos && slash2 != str.npos) {
        auto part1 = str.substr(0, slash1);
        auto part2 = str.substr(slash1 + 1, slash2 - slash1 - 1);
        auto part3 = str.substr(slash2 + 1);

        if (part2.size() == 0) {
            // Two values
            if (!ParseValue<T>(part1, tmp[0]) || !ParseValue<T>(part3, tmp[2])) {
                return false;
            }

        } else {
            // Three values
            if (!ParseValue<T>(part1, tmp[0]) || !ParseValue<T>(part2, tmp[1]) || !ParseValue<T>(part3, tmp[2])) {
                return false;
            }
        }
    }

    triplet = tmp;

    return true;
}

} // namespace

namespace game_engine
{

ObjParser::ObjParser() = default;

ObjParser::~ObjParser() = default;

// https://www.fileformat.info/format/material/
// https://www.martinreddy.net/gfx/3d/OBJ.spec
// https://paulbourke.net/dataformats/obj/
bool ObjParser::Parse(std::string source)
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

    return true;
}

const std::vector<ObjParser::Vertex>& ObjParser::GetVertices() const
{
    return m_vertices;
}

const std::vector<ObjParser::Point>& ObjParser::GetPoints() const
{
    return m_points;
}

const std::vector<ObjParser::Normal>& ObjParser::GetNormals() const
{
    return m_normals;
}

const std::vector<ObjParser::TextureVertex>& ObjParser::GetTextureVertices() const
{
    return m_texture_vertices;
}

const std::vector<ObjParser::Face>& ObjParser::GetFaces() const
{
    return m_faces;
}

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

    m_faces.clear();
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
        throw std::runtime_error("Face requires at last 3 vertices, got " + std::to_string(tokens.size() - 1));
    }

    Face face;
    face.reserve(tokens.size() - 1);

    for (std::size_t i = 1; i < tokens.size(); ++i) {
        std::array<int, 3> triplet;
        if (!ParseTriplet(tokens[i], triplet)) {
            throw std::runtime_error("Invalid face element format: " + std::string(tokens[i]));
        }

        const int vertex_count     = static_cast<int>(m_vertices.size());
        const int tex_vertex_count = static_cast<int>(m_texture_vertices.size());
        const int normals_count    = static_cast<int>(m_normals.size());

        const auto& [v_index, tv_index, n_index] = triplet;

        // Vertex index is required
        if (v_index == 0 || std::abs(v_index) > vertex_count) {
            throw std::out_of_range(
                "Vertex index out of range [" + std::to_string(v_index) + "]. Valid range: 1 to " + std::to_string(vertex_count));
        }

        // Texture vertex index is optional
        if (tv_index != 0 && std::abs(tv_index) > tex_vertex_count) {
            throw std::out_of_range("Texture vertex index out of range [" + std::to_string(tv_index) + "]. Valid range: 1 to " +
                                    std::to_string(tex_vertex_count));
        }

        // Normal index is optional
        if (n_index != 0 && std::abs(n_index) > normals_count) {
            throw std::out_of_range(
                "Normals index out of range [" + std::to_string(n_index) + "]. Valid range: 1 to " + std::to_string(normals_count));
        }

        // Convert to 0-based index
        const int v  = v_index > 0 ? v_index - 1 : vertex_count + v_index;
        const int tv = tv_index > 0 ? tv_index - 1 : tex_vertex_count + tv_index;
        const int n  = n_index > 0 ? n_index - 1 : normals_count + n_index;

        // Add triplet
        face.push_back({
            (v_index == 0 ? InvalidIndex : static_cast<IndexType>(v)),
            (tv_index == 0 ? InvalidIndex : static_cast<IndexType>(tv)),
            (n_index == 0 ? InvalidIndex : static_cast<IndexType>(n)),
        });
    }

    m_faces.push_back(face);
}

void ObjParser::NotImplementedStub(const std::vector<std::string_view>& tokens)
{
    throw std::runtime_error("Not implemented");
}

} // namespace game_engine
