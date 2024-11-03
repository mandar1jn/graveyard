#pragma once
#include "PreviewFile.h"
#include <array>

namespace AWExtractor
{

    struct BoundBox
    {
        std::vector<float> min;
        std::vector<float> max;
    };

    enum AttributeType : u32
    {
        FLOAT = 0,
        VEC2 = 1,
        VEC3 = 2,
        VEC4 = 3,
        SAMPLER = 8,
        TEXTURE = 9,
        BOOL = 12
    };

    struct Attribute
    {
        string name = "";
        AttributeType type;
        union
        {
            float valueFloat = 0;
            bool valueBool;
            float valueVec2[2];
            float valueVec3[3];
            float valueVec4[4];
        };
        string valueTexture;
    };

    struct Bone
    {
        string name;
        float bonePoses[12];
    };

    struct Material
    {
        string name;
        string shaderName;
        string sourcePath;
        u32 attributeCount;
        std::vector<Attribute> attributes;
    };

    enum VertexAttributeType : u8
    {
        POSITION = 0,
        COLOR = 1,
        BONE_INDEX = 2,
        BONE_WEIGHT = 3,
        TEX_COORD = 4,
        NORMAL = 5
    };

    enum VertexAttributeDataType : u8
    {
        VEC3F = 0,
        VEC4S = 1,
        VEC2S = 2,
        VEC4BF = 3,
        VEC4BI = 4,
        VEC4SI = 5
    };

    struct VertexAttribute
    {
        bool positionOnlyBuffer;
        VertexAttributeType type;
        VertexAttributeDataType dataType;
    };

    struct Mesh
    {
        u32 lod;
        u32 vertexCount;
        u32 faceCount;
        u32 vertexOffset;
        u32 positionOnlyVertexOffset;
        u32 faceOffset;
        u8 vertexAttributeCount;
        std::vector<VertexAttribute> vertexAttributes;
        u32 boneCount;
        std::vector<std::array<u32, 3>> faces;
    };

    class ModelPreview :public PreviewFile
    {
    public:
        ModelPreview(BinIndexFile* file);

        void ReadMesh();
        void Draw() override;

    private:
        u32 version;
        u32 vertexBufferSize;
        u32 positionOnlyVertexBufferSize;
        u32 indexCount;
        u32 indexSize;
        std::vector<u8> vertexBuffer;
        std::vector<u8> positionOnlyVertexBuffer;
        std::vector<u32> indexBuffer;
        u32 boneCount;
        std::vector<Bone> bones;
        BoundBox boundBox;
        u32 lodCount;
        u32 materialCount;
        std::vector<Material> materials;
        u32 meshCount;
        std::vector<Mesh> meshes;
    };

}