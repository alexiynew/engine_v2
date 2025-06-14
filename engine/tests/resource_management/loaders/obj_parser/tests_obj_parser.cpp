#include <vector>

#include <engine/graphics/mesh.hpp>

#include <gtest/gtest.h>
#include <resource_management/loaders/obj_parser.hpp>

namespace game_engine
{

bool operator==(const VertexAttribute& a, const VertexAttribute& b)
{
    return a.location == b.location && a.components == b.components && a.offset == b.offset && a.type == b.type &&
           a.normalized == b.normalized && a.name == b.name;
}

} // namespace game_engine

namespace
{

template <typename T>
std::vector<std::byte> ConvertToBytes(const std::vector<T>& values)
{
    std::vector<std::byte> data;

    data.resize(values.size() * sizeof(T));
    std::memcpy(data.data(), values.data(), data.size());

    return data;
}

const std::vector<game_engine::VertexAttribute> DefaultAttribute = {
    game_engine::VertexAttribute{
                                 .location   = 0,
                                 .components = 4,
                                 .offset     = 0,
                                 .type       = game_engine::VertexAttributeType::Float,
                                 .normalized = false,
                                 .name       = "position",
                                 }
};

} // namespace

TEST(ObjParserTest, Empty)
{
    using game_engine::ObjParser;

    const std::string Empty = R"(
        # Just empty 
        # v 0 0 0
    )";

    ObjParser parser;
    ASSERT_TRUE(parser.Parse(Empty));

    ASSERT_EQ(parser.GetVertices().size(), 0);
    ASSERT_EQ(parser.GetPoints().size(), 0);
    ASSERT_EQ(parser.GetNormals().size(), 0);
    ASSERT_EQ(parser.GetTextureVertices().size(), 0);
    ASSERT_EQ(parser.GetFaces().size(), 0);
}

TEST(ObjParserTest, SimpleFace)
{
    using game_engine::ObjParser;

    constexpr auto Inv = ObjParser::InvalidIndex;

    const std::string SimpleFace = R"(
        v 0 0 0
        v 1 0 0
        v 0 1 0
        f 1 2 3
    )";

    ObjParser parser;
    ASSERT_TRUE(parser.Parse(SimpleFace));

    const std::vector<ObjParser::Vertex> vertices = {
        {0.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
    };

    const std::vector<ObjParser::Face> faces = {
        {
         {0, Inv, Inv},
         {1, Inv, Inv},
         {2, Inv, Inv},
         }
    };

    ASSERT_EQ(parser.GetVertices(), vertices);
    ASSERT_EQ(parser.GetPoints().size(), 0);
    ASSERT_EQ(parser.GetNormals().size(), 0);
    ASSERT_EQ(parser.GetTextureVertices().size(), 0);
    ASSERT_EQ(parser.GetFaces(), faces);
}

TEST(ObjParserTest, NegativeIndices)
{
    using game_engine::ObjParser;

    constexpr auto Inv = ObjParser::InvalidIndex;

    const std::string NegativeIndices = R"(
         v 0 0 0
         v 1 0 0
         v 0 1 0
         f -1 -2 -3
     )";

    ObjParser parser;
    ASSERT_TRUE(parser.Parse(NegativeIndices));

    const std::vector<ObjParser::Vertex> vertices = {
        {0.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
    };

    const std::vector<ObjParser::Face> faces = {
        {
         {2, Inv, Inv},
         {1, Inv, Inv},
         {0, Inv, Inv},
         }
    };

    ASSERT_EQ(parser.GetVertices(), vertices);
    ASSERT_EQ(parser.GetPoints().size(), 0);
    ASSERT_EQ(parser.GetNormals().size(), 0);
    ASSERT_EQ(parser.GetTextureVertices().size(), 0);
    ASSERT_EQ(parser.GetFaces(), faces);
}

TEST(ObjParserTest, MixedFormats)
{
    using game_engine::ObjParser;

    constexpr auto Inv = ObjParser::InvalidIndex;

    const std::string MixedFormats = R"(
         v 0 0 0
         v 1 0 0
         v 0 1 0
         vt 0 0
         vn 0 0 1
         f 1 2 3
         f 1/1 2/1 3/1
         f 1//1 2//1 3//1
         f 1/1/1 2/1/1 3/1/1
     )";

    ObjParser parser;
    ASSERT_TRUE(parser.Parse(MixedFormats));

    const std::vector<ObjParser::Vertex> vertices = {
        {0.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
    };

    const std::vector<ObjParser::TextureVertex> textture_vertices = {
        {0.0f, 0.0f, 0.0f},
    };

    const std::vector<ObjParser::Normal> normals = {
        {0.0f, 0.0f, 1.0f},
    };

    const std::vector<ObjParser::Face> faces = {
        // f 1 2 3
        {
         {0, Inv, Inv},
         {1, Inv, Inv},
         {2, Inv, Inv},
         },
        // f 1/1 2/1 3/1
        {
         {0, 0, Inv},
         {1, 0, Inv},
         {2, 0, Inv},
         },
        // f 1//1 2//1 3//1
        {
         {0, Inv, 0},
         {1, Inv, 0},
         {2, Inv, 0},
         },
        // f 1/1/1 2/1/1 3/1/1
        {
         {0, 0, 0},
         {1, 0, 0},
         {2, 0, 0},
         }
    };

    ASSERT_EQ(parser.GetVertices(), vertices);
    ASSERT_EQ(parser.GetPoints().size(), 0);
    ASSERT_EQ(parser.GetNormals(), normals);
    ASSERT_EQ(parser.GetTextureVertices(), textture_vertices);
    ASSERT_EQ(parser.GetFaces(), faces);
}

TEST(ObjParserTest, Ngon)
{
    using game_engine::ObjParser;

    constexpr auto Inv = ObjParser::InvalidIndex;

    const std::string Ngon = R"(
         v 0 0 0
         v 2 0 0
         v 3 1 0
         v 1 2 0
         v -1 1 0
         f 1 2 3 4 5
     )";

    ObjParser parser;
    ASSERT_TRUE(parser.Parse(Ngon));

    const std::vector<ObjParser::Vertex> vertices = {
        { 0.0f, 0.0f, 0.0f, 1.0f},
        { 2.0f, 0.0f, 0.0f, 1.0f},
        { 3.0f, 1.0f, 0.0f, 1.0f},
        { 1.0f, 2.0f, 0.0f, 1.0f},
        {-1.0f, 1.0f, 0.0f, 1.0f},
    };

    const std::vector<ObjParser::Face> faces = {
        // f 1 2 3 4 5
        {
         {0, Inv, Inv},
         {1, Inv, Inv},
         {2, Inv, Inv},
         {3, Inv, Inv},
         {4, Inv, Inv},
         }
    };

    ASSERT_EQ(parser.GetVertices(), vertices);
    ASSERT_EQ(parser.GetPoints().size(), 0);
    ASSERT_EQ(parser.GetNormals().size(), 0);
    ASSERT_EQ(parser.GetTextureVertices().size(), 0);
    ASSERT_EQ(parser.GetFaces(), faces);
}

TEST(ObjParserTest, InvalidCases)
{
    const std::string InvalidCases = R"(
         v 0 0 0
         v 1 0 0
         f 1 2 3
     )";

    game_engine::ObjParser parser;
    EXPECT_THROW(parser.Parse(InvalidCases), std::out_of_range);
}
