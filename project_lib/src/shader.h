#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>     // 包含glad来获取所有的必须OpenGL头文件

#include <iostream>

class ShaderProgram
{
public:
    ShaderProgram( const GLchar *vertexPath,        // 顶点着色器文件路径
                   const GLchar *fragmentPath );    // 片元着色器文件路径


    void use();                                     // 使用/激活程序

    // uniform工具函数
    void setBool( const std::string &name, bool value ) const;
    void setInt( const std::string &name, int value ) const;
    void setFloat( const std::string &name, float value ) const;
    void set4Float( const std::string &name, float value1, float value2, float value3, float value4 ) const;

public:
    unsigned int id;            // 程序ID
};

#endif