#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>     // 包含glad来获取所有的必须OpenGL头文件
#include <iostream>
#include "glm/glm.hpp"

class Shader
{
public:
    Shader();
    Shader( const std::string &vertexPath,                  // 顶点着色器文件路径
            const std::string &fragmentPath,                // 片段着色器文件路径
            const std::string &geometryPath = "" );         // 几何着色器文件路径

    void initWithFile( const std::string &vertexPath,                   // 顶点着色器文件路径
                       const std::string &fragmentPath,                 // 片段着色器文件路径
                       const std::string &geometryPath = "" );          // 几何着色器文件路径

    void initWithSrc( const std::string &vertexShaderSrc,               // 顶点着色器代码
                      const std::string &fragmentShaderSrc,             // 片段着色器代码
                      const std::string &geometryShaderSrc = "" );      // 几何着色器代码


    void use() const;                                       // 使用/激活程序

    // uniform工具函数
    void setBool( const std::string &name, GLboolean value ) const;
    void setInt( const std::string &name, GLint value ) const;
    void setFloat( const std::string &name, GLfloat value ) const;

    void setVec2( const std::string &name, const glm::vec2 &value ) const;
    void setVec2( const std::string &name, GLfloat x, GLfloat  y ) const;
    void setVec3( const std::string &name, const glm::vec3 &value ) const;
    void setVec3( const std::string &name, GLfloat x, GLfloat  y, GLfloat z ) const;
    void setVec4( const std::string &name, const glm::vec4 &value ) const;
    void setVec4( const std::string &name, GLfloat x, GLfloat  y, GLfloat z, GLfloat w );

    void setMat2( const std::string &name, const glm::mat2 &mat ) const;
    void setMat3( const std::string &name, const glm::mat3 &mat ) const;
    void setMat4( const std::string &name, const glm::mat4 &mat ) const;
    void setMat4( const std::string &name, const GLfloat   *values ) const;
private:
    //GLuint createAndCompileShaderFromFile( GLenum shaderFlag, const std::string &shaderPath );
    GLuint createAndCompileShader( GLenum shaderFlag, const std::string &shaderSrc );
public:
    GLuint id;            // 程序ID
};

#endif
