#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

// 4个绘制两个三角形，组成一个矩形。
float vertices[] =
{
    // 位置                // 颜色                   // 纹理坐标
    0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // 右上
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // 右下
    -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // 左上
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,  // 左下
};
// OpenGL规定：顶点坐标（0.0，0.0）在中心点，而纹理坐标（0.0，0.0）在左下角
// 在常见图片格式中，（0.0，0.0）可能是在图片左上角，因此可能导致图片倒转。

unsigned int indices[] =    // 矩形顶点索引
{
    0, 1, 2,                // 第一个三角形
    1, 2, 3,                // 第二个三角形
};

GLuint texture;

Shader shader;


GLuint VAO = 0, VBO = 0, EBO = 0;

void loadData()
{
    shader = Shader( projectDir() + "\\src\\shader\\shader_vertex",     // 顶点着色器脚本
                     projectDir() + "\\src\\shader\\shader_fragment" ); // 片段着色器脚本

    texture = loadTexture( projectDir() + "\\res\\awesomeface.png" );

    loadVertexData( vertices, sizeof( vertices ), "342", &VAO, &VBO, indices, sizeof( indices ), &EBO );
}

#endif