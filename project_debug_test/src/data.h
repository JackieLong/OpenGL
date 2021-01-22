#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

enum BUFFER_MODE
{
    MODE_VBO,
    MODE_EBO,
};

const int ScreenWidth = 800;
const int ScreenHeight = 600;

// 6个顶点绘制两个三角形组成矩形
const float vertices_VBO[] =
{
    0.5f,  0.5f, 0.0f,          // 右上角
    0.5f, -0.5f, 0.0f,          // 右下角
    -0.5f,  0.5f, 0.0f,         // 左上角

    -0.5f,  0.5f, 0.0f,         // 左上角
    -0.5f, -0.5f, 0.0f,         // 左下角
    0.5f, -0.5f, 0.0f,          // 右下角
};

// 4个顶点绘制两个三角形组成矩形
const float vertices_EBO[] =
{
    0.5f,  0.5f, 0.0f,      // 右上角
    0.5f, -0.5f, 0.0f,      // 右下角
    -0.5f, -0.5f, 0.0f,     // 左下角
    -0.5f,  0.5f, 0.0f      // 左上角
};

const unsigned int indices_EBO[] =    // 三角形的顶点索引
{
    0, 1, 3,                // 第一个三角形
    1, 2, 3                 // 第二个三角形
};


GLuint VAO = 0, VBO = 0, EBO = 0;

Shader shader;

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    // GL_LINE：线框模式；GL_FILL：填充模式
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
}

#endif