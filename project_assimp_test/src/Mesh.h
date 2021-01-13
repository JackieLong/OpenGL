#ifndef __MESH_H__
#define __MESH_H__

#include "project_lib.h"

struct Vertex               // 顶点数据结构体
{
    glm::vec3 pos;          // 顶点坐标
    glm::vec3 normal;       // 顶点法向量
    glm::vec2 texCoord;     // 顶点纹理坐标
    glm::vec3 tangent;      // tangent
    glm::vec3 bitangent;    // bitangent
};

enum TextureType
{
    TEXTURE_DIFFUSE = 0,    // 漫反射贴图纹理
    TEXTURE_SPECULAR,       // 镜面反射贴图纹理
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,

    TEXTURE_END

};
struct Texture          // 纹理数据结构体
{
    GLuint      id;     // 纹理id
    TextureType type;   // 纹理类型（漫反射贴图、镜面反射贴图）
    std::string path;   // 纹理路径
};

class Mesh
{
public:
    // mesh Data
    std::vector<Vertex>       mVertices;
    std::vector<GLuint>       mIndices;
    std::vector<Texture>      mTextures;

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;

    Mesh::Mesh( std::vector<Vertex>  vertices,
                std::vector<GLuint>  indices,
                std::vector<Texture> textures );
    void Mesh::draw( Shader &shader );
    void Mesh::setupMesh();
};



#endif