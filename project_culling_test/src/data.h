#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

const GLfloat cubeVertices[] =        // 箱子顶点数据
{
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
};

const GLfloat coordVertices[] =   // 坐标轴顶点数据
{
    // 顶点坐标             // 纹理坐标  // 颜色坐标
    -10000.0f, 0.0f, 0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,      // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,      // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f,      // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,      // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,   0.0f, 0.0f, 1.0f,       // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,      // Z轴上顶点位置2
};

enum SHADER_TASK
{
    TASK_DRAW_COORD = 0,
    TASK_DRAW_CUBE,
    TASK_MAX
};

Camera *pCamera = nullptr;

double deltaTime = 0.0;             // 当前帧与上一帧的时间差
double lastFrame = 0.0;             // 上一帧的时间

Shader shader;                      // 着色器

GLuint VAO_cube = 0;                // 箱子的VAO、VBO
GLuint VBO_cube = 0;

GLuint VAO_coord = 0;               // 坐标系的VAO、VBO
GLuint VBO_coord = 0;

GLuint textureCube   = -1;           // 箱子纹理

glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;

void loadData()
{
    pCamera       = new Camera( glm::vec3( 0.0f, 0.0f, 3.0f ),    // 摄像机的初始位置
                                glm::vec3( 0.0f, 0.0f, 0.0f ),    // 摄像机注视点
                                glm::vec3( 0.0f, 1.0f, 0.0f ),    // 世界坐标中的向上向量
                                -90.f,                            // Yaw
                                0.0f );                           // Pitch

    shader        = Shader( projectDir() + "\\src\\shader\\shader_vertex", projectDir() + "\\src\\shader\\shader_fragment" );

    createVertexBuffer( cubeVertices, sizeof( cubeVertices ), "32", &VAO_cube, &VBO_cube );
    createVertexBuffer( coordVertices, sizeof( coordVertices ), "323", &VAO_coord, &VBO_coord );

    textureCube   = createTexture( projectDir() + "/res/marble.jpg" );

}

#endif