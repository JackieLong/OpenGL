#include "Model.h"

using namespace std;

Model::Model( const std::string &path )
{
    loadModel( path );
}

void Model::draw( ShaderProgram &shader )
{
    for( unsigned int i = 0; i < mMeshes.size(); i++ )
    {
        mMeshes[i].draw( shader );
    }
}

void Model::loadModel( const std::string &path )
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
    if( !scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    mDirectory = path.substr( 0, path.find_last_of( '/' ) );
    processNode( scene->mRootNode, scene );
}

void Model::processNode( aiNode *node, const aiScene *scene )
{
    for( unsigned int i = 0; i < node->mNumMeshes; i++ )
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back( processMesh( mesh, scene ) );
    }
    for( unsigned int i = 0; i < node->mNumChildren; i++ )
    {
        processNode( node->mChildren[i], scene );
    }
}

Mesh Model::processMesh( aiMesh *mesh, const aiScene *scene )
{
    vector<Vertex>  vertices;
    vector<GLuint>  indices;
    vector<Texture> textures;

    for( unsigned int i = 0; i < mesh->mNumVertices; i++ )
    {
        Vertex vertex;

        glm::vec3 vec;

        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.pos = vec;

        // normals
        if( mesh->HasNormals() )
        {
            vec.x = mesh->mNormals[i].x;
            vec.y = mesh->mNormals[i].y;
            vec.z = mesh->mNormals[i].z;
            vertex.normal = vec;
        }

        if( mesh->mTextureCoords[0] )
        {
            glm::vec2 texCoord;
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = texCoord;

            // tangent
            vec.x = mesh->mTangents[i].x;
            vec.y = mesh->mTangents[i].y;
            vec.z = mesh->mTangents[i].z;
            vertex.tangent = vec;

            // bitangent
            vec.x = mesh->mBitangents[i].x;
            vec.y = mesh->mBitangents[i].y;
            vec.z = mesh->mBitangents[i].z;
            vertex.bitangent = vec;
        }
        else
        {
            vertex.texCoord = glm::vec2( 0.0f, 0.0f );
        }
        vertices.push_back( vertex );
    }

    for( unsigned int i = 0; i < mesh->mNumFaces; i++ )
    {
        aiFace face = mesh->mFaces[i];
        for( unsigned int j = 0; j < face.mNumIndices; j++ )
        {
            indices.push_back( face.mIndices[j] );
        }
    }

    if( mesh->mMaterialIndex >= 0 )
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures( material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE );
        textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );

        vector<Texture> specularMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, TEXTURE_SPECULAR );
        textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );

        vector<Texture> normalMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, TEXTURE_NORMAL );
        textures.insert( textures.end(), normalMaps.begin(), normalMaps.end() );

        vector<Texture> heightMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, TEXTURE_HEIGHT );
        textures.insert( textures.end(), heightMaps.begin(), heightMaps.end() );
    }

    return Mesh( vertices, indices, textures );
}

vector<Texture> Model::loadMaterialTextures( aiMaterial *mat, aiTextureType type, const TextureType &typeName )
{
    vector<Texture> textures;
    for( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ )
    {
        aiString str;
        mat->GetTexture( type, i, &str );
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for( unsigned int j = 0; j < textures_loaded.size(); j++ )
        {
            if( std::strcmp( textures_loaded[j].path.data(), str.C_Str() ) == 0 )
            {
                textures.push_back( textures_loaded[j] );
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if( !skip )     // if texture hasn't been loaded already, load it
        {
            Texture texture;
            texture.id = this->loadTexture( mDirectory + "/" +  str.C_Str() );
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back( texture );
            textures_loaded.push_back( texture );  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

GLuint Model::loadTexture( const std::string &path )
{
    GLuint texture = -1;
    glGenTextures( 1, &texture );

    glActiveTexture( GL_TEXTURE0 + 0 );         // 纹理数据由GL_TEXTURE0纹理单元管理
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );  // “X”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );  // “Y”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略

    GLint width, height, nrChannels;

    // 加载图片到内存中
    unsigned char *data = stbi_load( path.c_str(),          // 图片路径
                                     &width, &height,       // 图片宽高
                                     &nrChannels, 0 );      // 颜色通道数量
    if( data )
    {
        GLenum format;
        if( nrChannels == 1 )
        {
            format = GL_RED;
        }
        else
            if( nrChannels == 3 )
            {
                format = GL_RGB;
            }
            else
                if( nrChannels == 4 )
                {
                    format = GL_RGBA;
                }

        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,        // 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      format,              // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      format,              // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );  // 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture::" << path << endl;
    }
    stbi_image_free( data );                // 纹理数据已经上传到显存中，内存中的数据可以删除了。

    return  texture;
}


