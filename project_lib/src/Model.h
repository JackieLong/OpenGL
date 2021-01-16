#ifndef __MODEL_H__
#define __MODEL_H__

#include <iostream>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "shader.h"

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

private:
    std::vector<Mesh> mMeshes;
    std::string       mDirectory;
};

#endif