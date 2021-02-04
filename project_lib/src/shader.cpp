#include <fstream>
#include "shader.h"

using namespace std;

void loadShaderSrcFromFile( const string &filePath, string &content );

Shader::Shader()
    : id( -1 )
{

}

Shader::Shader( const string &vertexPath, const string &fragmentPath, const string &geometryPath )
{
    initWithFile( vertexPath, fragmentPath, geometryPath );
}

void Shader::initWithFile( const string &vertexPath, const string &fragmentPath, const string &geometryPath )
{
    string vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc;
    loadShaderSrcFromFile( vertexPath, vertexShaderSrc );
    loadShaderSrcFromFile( fragmentPath, fragmentShaderSrc );
    loadShaderSrcFromFile( geometryPath, geometryShaderSrc );

    initWithSrc( vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc );
}

void Shader::initWithSrc( const string &vertexShaderSrc, const string &fragmentShaderSrc, const string &geometryShaderSrc )
{
    GLuint vertexShader = createAndCompileShader( GL_VERTEX_SHADER, vertexShaderSrc );
    GLuint fragmentShader = createAndCompileShader( GL_FRAGMENT_SHADER, fragmentShaderSrc );

    GLboolean gotGeometryShader = geometryShaderSrc != "";

    GLuint geometryShader = gotGeometryShader ? createAndCompileShader( GL_GEOMETRY_SHADER, geometryShaderSrc ) : -1;

    id = glCreateProgram();
    glAttachShader( id, vertexShader );
    glAttachShader( id, fragmentShader );
    if( gotGeometryShader )
    {
        glAttachShader( id, geometryShader );
    }
    glLinkProgram( id );

    GLint  success;                                                    // 编译成功与否
    const  GLsizei lenLog = 512;                                         // 日志长度
    GLchar infoLog[lenLog];                                           // 编译输出日志
    glGetProgramiv( id, GL_LINK_STATUS, &success );                 // check for linking errors
    if( !success )
    {
        glGetProgramInfoLog( id, lenLog, NULL, infoLog );
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    // 在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
    if( gotGeometryShader )
    {
        glDeleteShader( geometryShader );
    }
}

void Shader::use() const
{
    glUseProgram( id );
}

void Shader::setBool( const string &name, GLboolean value ) const
{
    glUniform1i( glGetUniformLocation( id, name.c_str() ), ( int )value );
}

void Shader::setInt( const string &name, GLint value ) const
{
    glUniform1i( glGetUniformLocation( id, name.c_str() ), value );
}

void Shader::setFloat( const string &name, GLfloat value ) const
{
    glUniform1f( glGetUniformLocation( id, name.c_str() ), value );
}

void Shader::setVec2( const string &name, const glm::vec2 &value ) const
{
    glUniform2fv( glGetUniformLocation( id, name.c_str() ), 1, &value[0] );
}

void Shader::setVec2( const string &name, GLfloat x, GLfloat y ) const
{
    glUniform2f( glGetUniformLocation( id, name.c_str() ), x, y );
}

void Shader::setVec3( const string &name, const glm::vec3 &value ) const
{
    glUniform3fv( glGetUniformLocation( id, name.c_str() ), 1, &value[0] );
}

void Shader::setVec3( const string &name, GLfloat x, GLfloat y, GLfloat z ) const
{
    glUniform3f( glGetUniformLocation( id, name.c_str() ), x, y, z );
}

void Shader::setVec4( const string &name, const glm::vec4 &value ) const
{
    glUniform4fv( glGetUniformLocation( id, name.c_str() ), 1, &value[0] );
}

void Shader::setVec4( const string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    glUniform4f( glGetUniformLocation( id, name.c_str() ), x, y, z, w );
}

void Shader::setMat2( const string &name, const glm::mat2 &mat ) const
{
    glUniformMatrix2fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

void Shader::setMat3( const string &name, const glm::mat3 &mat ) const
{
    glUniformMatrix3fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

void Shader::setMat4( const string &name, const GLfloat *values ) const
{
    GLint transformLoc = glGetUniformLocation( id, name.c_str() );
    glUniformMatrix4fv( transformLoc,   // 位置值
                        1,              // 需要修改的矩阵数量
                        GL_FALSE,       // 是否需要转置矩阵，有些平台使用列向量（列主序），有些平台使用行向量（行主序）。
                        values );       // 矩阵里元素数据
}

void Shader::setMat4( const string &name, const glm::mat4 &mat ) const
{
    glUniformMatrix4fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

//GLuint Shader::createAndCompileShaderFromFile( GLenum shaderFlag, const string &shaderPath )
//{
//    GLuint shader = -1;
//    ifstream shaderFile( shaderPath );
//    if( shaderFile.is_open() )
//    {
//        string shaderSrc( ( istreambuf_iterator<char>( shaderFile ) ), istreambuf_iterator<char>() );
//        shader = createAndCompileShader( shaderFlag, shaderSrc );
//    }
//    shaderFile.close();
//    return shader;
//}


// build and compile our shader program
GLuint Shader::createAndCompileShader( GLenum shaderFlag, const string &shaderSrc )
{
    GLuint shader = glCreateShader( shaderFlag );                   // 常见shader实例
    const char *tmpShaderSrc = shaderSrc.c_str();
    glShaderSource( shader, 1, &tmpShaderSrc, NULL );               // 加载shader源码

    glCompileShader( shader );                                      // 编译shader

    GLint  success;                                                 // 编译成功与否
    const  GLsizei lenLog = 512;                                    // 日志长度
    GLchar infoLog[lenLog];                                         // 编译输出日志
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );           // 获取编译状态数据
    if( !success )
    {
        glGetShaderInfoLog( shader, lenLog, NULL, infoLog );        // 编译没有成功，获取编译日志
        cout << ( shaderFlag == GL_VERTEX_SHADER ? "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" :
                  shaderFlag == GL_FRAGMENT_SHADER ? "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" :
                  shaderFlag == GL_GEOMETRY_SHADER ? "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" :
                  "ERROR::SHADER::UNKNOWN::COMPILATION_FAILED\n" )
             << infoLog
             << endl;
    }
    return shader;
}

void loadShaderSrcFromFile( const string &filePath, string &content )
{
    if( filePath == "" )
    {
        content = "";
        return;
    }
    ifstream fileStream( filePath );
    if( fileStream.is_open() )
    {
        content = string( ( istreambuf_iterator<char>( fileStream ) ), istreambuf_iterator<char>() );
        fileStream.close();
    }
    else
    {
        //cout << "file open failed.[" << filePath.c_str() << "]";
        content = "";
    }
}
