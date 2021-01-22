#ifndef __MODEL_H__
#define __MODEL_H__

#include <iostream>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "shader.h"


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

public:
    std::vector<Mesh>    mMeshes;
    std::vector<Texture> mTextures_loaded;
    std::string          mDirectory;
};

#endif