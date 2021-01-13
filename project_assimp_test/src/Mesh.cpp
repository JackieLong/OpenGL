#include "Mesh.h"

using namespace std;

Mesh::Mesh( std::vector<Vertex>  vertices,
            std::vector<GLuint>  indices,
            std::vector<Texture> textures )
    : mVertices( vertices ),
      mIndices( indices ),
      mTextures( textures ),
      mVAO( -1 ),
      mVBO( -1 ),
      mEBO( -1 )
{
    setupMesh();
}

void Mesh::draw( Shader &shader )
{
    shader.use();

    GLuint fuckCount[TEXTURE_END - TEXTURE_DIFFUSE] = { 0 };
    for( unsigned int i = 0; i < mTextures.size(); i++ )
    {
        glActiveTexture( GL_TEXTURE0 + i );
        switch( mTextures[i].type )
        {
            case TEXTURE_DIFFUSE:
                shader.setInt( "texture_diffuse" + to_string( fuckCount[mTextures[i].type]++ ), i );
                break;
            case TEXTURE_SPECULAR:
                shader.setInt( "texture_specular" + to_string( fuckCount[mTextures[i].type]++ ), i );
                break;
            case TEXTURE_NORMAL:
                shader.setInt( "texture_normal" + to_string( fuckCount[mTextures[i].type]++ ), i );
                break;
            case TEXTURE_HEIGHT:
                shader.setInt( "texture_height" + to_string( fuckCount[mTextures[i].type]++ ), i );
                break;
        }
        glBindTexture( GL_TEXTURE_2D, mTextures[i].id );
    }

    glBindVertexArray( mVAO );
    glDrawElements( GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );

    glActiveTexture( GL_TEXTURE0 );
}

void Mesh::setupMesh()
{
    glGenVertexArrays( 1, &mVAO );
    glGenBuffers( 1, &mVBO );
    glGenBuffers( 1, &mEBO );

    glBindVertexArray( mVAO );

    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size() * sizeof( Vertex ), &mVertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof( GLint ), &mIndices[0], GL_STATIC_DRAW );

    glEnableVertexAttribArray( 0 );     // 顶点坐标位置值
    glVertexAttribPointer( 0,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof( Vertex ),
                           ( void * ) 0 );

    glEnableVertexAttribArray( 1 );     // 顶点法线位置值
    glVertexAttribPointer( 1,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof( Vertex ),
                           ( void * ) offsetof( Vertex, normal ) );     // 名称为normal的成员在Vertex中的偏移量

    glEnableVertexAttribArray( 2 );     // 纹理坐标位置值
    glVertexAttribPointer( 2,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof( Vertex ),
                           ( void * ) offsetof( Vertex, texCoord ) );   // 名称为normal的成员在Vertex中的偏移量

    glEnableVertexAttribArray( 3 );     // vertex tangent
    glVertexAttribPointer( 3,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof( Vertex ),
                           ( void * ) offsetof( Vertex, tangent ) );

    glEnableVertexAttribArray( 4 );     // vertex bitangent
    glVertexAttribPointer( 4,
                           3,
                           GL_FLOAT, GL_FALSE,
                           sizeof( Vertex ),
                           ( void * ) offsetof( Vertex, bitangent ) );

    glBindVertexArray( 0 );
}

