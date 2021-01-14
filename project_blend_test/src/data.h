#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

const GLfloat cubeVertices[] =        // 箱子顶点数据
{
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const GLfloat floorVertices[] =       // 地板顶点数据
{
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

std::vector<glm::vec3> windowLocation =
{
    glm::vec3( -1.5f, 0.0f, -0.48f ),
    glm::vec3( 0.0f, 0.0f, 0.7f ),
    glm::vec3( -0.3f, 0.0f, -2.3f ),
    glm::vec3( 0.5f, 0.0f, -0.6f )
};

const GLfloat grassVertices[] =
{
    // 顶点坐标             // 纹理坐标
    -0.5f,  -0.5f,  0.0f,   0.0f, 0.0f,
    0.5f,   -0.5f,  0.0f,   1.0f, 0.0f,
    0.5f,   0.5f,   0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,   0.0f,   0.0f, 1.0f
};

const GLuint grassIndices[] =
{
    0, 1, 2,
    0, 2, 3,
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
    TASK_DRAW_FLOOR,
    TASK_DRAW_WINDOW,
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

GLuint VAO_floor = 0;               // 地板的VAO、VBO
GLuint VBO_floor = 0;

GLuint VAO_window = 0;
GLuint VBO_window = 0;
GLuint EBO_window = 0;

GLuint textureCube   = -1;           // 箱子纹理
GLuint textureFloor  = -1;           // 地板纹理
GLuint textureWindow = -1;           // 窗户纹理

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

    loadVertexData( cubeVertices, sizeof( cubeVertices ), "32", &VAO_cube, &VBO_cube );
    loadVertexData( floorVertices, sizeof( floorVertices ), "32", &VAO_floor, &VBO_floor );
    loadVertexData( coordVertices, sizeof( coordVertices ), "323", &VAO_coord, &VBO_coord );
    loadVertexData( grassVertices, sizeof( grassVertices ), "32", &VAO_window, &VBO_window, grassIndices, sizeof( grassIndices ), &EBO_window );

    textureFloor  = loadTexture( projectDir() + "/res/metal.png" );
    textureCube   = loadTexture( projectDir() + "/res/marble.jpg" );
    textureWindow = loadTexture( projectDir() + "/res/blending_transparent_window.png" );

}
#endif