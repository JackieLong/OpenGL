#ifndef __MODEL_H__
#define __MODEL_H__

#include "project_lib.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"

static std::vector<Texture> textures_loaded;

class Model
{
public:
    Model( const std::string &path );

    void draw( Shader &shader );

private:
    void loadModel( const std::string &path );
    void processNode( aiNode *node, const aiScene *scene );
    Mesh processMesh( aiMesh *mesh, const aiScene *scene );
    std::vector<Texture> loadMaterialTextures( aiMaterial *mat, aiTextureType type, const TextureType &typeName );

    // 构建第一张图的纹理
    GLuint loadTexture( const std::string &path );

private:
    std::vector<Mesh> mMeshes;
    std::string       mDirectory;
};

#endif