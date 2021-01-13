#include <fstream>

#include "shader.h"

using namespace std;

int createAndCompileShader( int shaderFlag, const char *shaderSrc );

Shader::Shader( const string &vertexPath, const string &fragmentPath )
{
    int vertexShader = -1;
    int fragmentShader = -1;
    std::ifstream vertexFile( vertexPath );
    std::ifstream fragmentFile( fragmentPath );
    if( vertexFile.is_open() && fragmentFile.is_open() )
    {
        std::string vertexShaderSrc( ( std::istreambuf_iterator<char>( vertexFile ) ), std::istreambuf_iterator<char>() );
        std::string fragmentShaderSrc( ( std::istreambuf_iterator<char>( fragmentFile ) ), std::istreambuf_iterator<char>() );
        vertexShader = createAndCompileShader( GL_VERTEX_SHADER, vertexShaderSrc.c_str() );
        fragmentShader = createAndCompileShader( GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str() );
    }
    vertexFile.close();
    fragmentFile.close();

    id = glCreateProgram();
    glAttachShader( id, vertexShader );
    glAttachShader( id, fragmentShader );
    glLinkProgram( id );

    int success;                                                    // 编译成功与否
    const int lenLog = 512;                                         // 日志长度
    char infoLog[lenLog];                                           // 编译输出日志
    glGetProgramiv( id, GL_LINK_STATUS, &success );                 // check for linking errors
    if( !success )
    {
        glGetProgramInfoLog( id, lenLog, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

}

Shader::Shader()
    : id( -1 )
{

}

void Shader::use() const
{
    glUseProgram( id );
}

void Shader::setBool( const std::string &name, bool value ) const
{
    glUniform1i( glGetUniformLocation( id, name.c_str() ), ( int )value );
}

void Shader::setInt( const std::string &name, int value ) const
{
    glUniform1i( glGetUniformLocation( id, name.c_str() ), value );
}

void Shader::setFloat( const std::string &name, float value ) const
{
    glUniform1f( glGetUniformLocation( id, name.c_str() ), value );
}

void Shader::setVec2( const std::string &name, const glm::vec2 &value ) const
{
    glUniform2fv( glGetUniformLocation( id, name.c_str() ), 1, &value[0] );
}

void Shader::setVec2( const std::string &name, float x, float y ) const
{
    glUniform2f( glGetUniformLocation( id, name.c_str() ), x, y );
}

void Shader::setVec3( const std::string &name, const glm::vec3 &value ) const
{
    glUniform3fv( glGetUniformLocation( id, name.c_str() ), 1, &value[0] );
}

void Shader::setVec3( const std::string &name, float x, float y, float z ) const
{
    glUniform3f( glGetUniformLocation( id, name.c_str() ), x, y, z );
}

void Shader::setVec4( const std::string &name, const glm::vec4 &value ) const
{
    glUniform4fv( glGetUniformLocation( id, name.c_str() ), 1, &value[0] );
}

void Shader::setVec4( const std::string &name, float x, float y, float z, float w )
{
    glUniform4f( glGetUniformLocation( id, name.c_str() ), x, y, z, w );
}

void Shader::setMat2( const std::string &name, const glm::mat2 &mat ) const
{
    glUniformMatrix2fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

void Shader::setMat3( const std::string &name, const glm::mat3 &mat ) const
{
    glUniformMatrix3fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

void Shader::setMat4( const std::string &name, const GLfloat *values ) const
{
    unsigned int transformLoc = glGetUniformLocation( id, name.c_str() );
    glUniformMatrix4fv( transformLoc,   // 位置值
                        1,              // 需要修改的矩阵数量
                        GL_FALSE,       // 是否需要转置矩阵，有些平台使用列向量（列主序），有些平台使用行向量（行主序）。
                        values );       // 矩阵里元素数据
}

void Shader::setMat4( const std::string &name, const glm::mat4 &mat ) const
{
    glUniformMatrix4fv( glGetUniformLocation( id, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

// build and compile our shader program
int createAndCompileShader( int shaderFlag, const char *shaderSrc )
{
    int shader = glCreateShader( shaderFlag );                      // 常见shader实例
    glShaderSource( shader, 1, &shaderSrc, NULL );                  // 加载shader源码

    glCompileShader( shader );                                      // 编译shader

    int success;                                                    // 编译成功与否
    const int lenLog = 512;                                         // 日志长度
    char infoLog[lenLog];                                           // 编译输出日志
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );           // 获取编译状态数据
    if( !success )
    {
        glGetShaderInfoLog( shader, lenLog, NULL, infoLog );        // 编译没有成功，获取编译日志
        std::cout << ( shaderFlag == GL_VERTEX_SHADER ?
                       "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" :
                       "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" )
                  << infoLog
                  << std::endl;
    }
    return shader;
}
