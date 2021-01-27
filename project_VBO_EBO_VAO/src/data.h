#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

const GLint ScreenWidth  = 800;
const GLint ScreenHeight = 600;

// 6个顶点绘制两个三角形组成矩形
const GLfloat vertices_VBO[] =
{
    0.5f,  0.5f, 0.0f,          // 右上角
    0.5f, -0.5f, 0.0f,          // 右下角
    -0.5f,  0.5f, 0.0f,         // 左上角

    -0.5f,  0.5f, 0.0f,         // 左上角
    -0.5f, -0.5f, 0.0f,         // 左下角
    0.5f, -0.5f, 0.0f,          // 右下角
};

// 4个顶点绘制两个三角形组成矩形
const GLfloat vertices_EBO[] =
{
    0.5f,  0.5f, 0.0f,      // 右上角
    0.5f, -0.5f, 0.0f,      // 右下角
    -0.5f, -0.5f, 0.0f,     // 左下角
    -0.5f,  0.5f, 0.0f      // 左上角
};

const GLuint indices_EBO[] =    // 三角形的顶点索引
{
    0, 1, 3,                // 第一个三角形
    1, 2, 3                 // 第二个三角形
};


static GLuint VAO = 0, VBO = 0, EBO = 0;

static Shader shader;

static void loadData()
{
    shader = Shader( projectDir() + "/src/shader/shader_vertex",
                     projectDir() + "/src/shader/shader_fragment" );
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
}


#endif