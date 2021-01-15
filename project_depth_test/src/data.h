#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

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

GLuint textureCube = -1;            // 箱子纹理
GLuint textureFloor = -1;           // 地板纹理

glm::mat4 viewMatrix( 1.0f );       // 观察矩阵
glm::mat4 projectionMatrix( 1.0f ); // 投影矩阵

const float cubeVertices[] =        // 箱子顶点数据
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


const float floorVertices[] =       // 地板顶点数据
{
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};


const float coordVertices[] =   // 坐标轴顶点数据
{
    // 顶点坐标
    -10000.0f, 0.0f, 0.0f,      // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,       // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,       // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,      // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,       // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,      // Z轴上顶点位置2
};

void loadData()
{
    pCamera = new Camera( glm::vec3( 0.0f, 0.0f, 3.0f ),    // 摄像机的初始位置
                          glm::vec3( 0.0f, 0.0f, 0.0f ),    // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ),    // 世界坐标中的向上向量
                          -90.f,                            // Yaw
                          0.0f );                           // Pitch

    shader = Shader( projectDir() + "\\src\\shader\\shader_vertex", projectDir() + "\\src\\shader\\shader_fragment" );

    createVertexBuffer( cubeVertices, sizeof( cubeVertices ), "32", &VAO_cube, &VBO_cube );
    createVertexBuffer( floorVertices, sizeof( floorVertices ), "32", &VAO_floor, &VBO_floor );
    createVertexBuffer( coordVertices, sizeof( coordVertices ), "3", &VAO_coord, &VBO_coord );

    textureFloor = createTexture( projectDir() + "/res/metal.png" );
    textureCube = createTexture( projectDir() + "/res/marble.jpg" );


}

void loadShaderData_coord( Shader &shaderProgram )
{
    shaderProgram.use();

    shaderProgram.setInt( "task", 1 );

    glm::mat4 modelMatrix( 1.0f );

    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix = glm::perspective(
            glm::radians( pCamera->fov() ),         // FOV，field of view，视角广度
            ( float ) ScreenWidth / ScreenHeight,   // aspectRatio，宽高比
            0.1f,                                   // near plane，近平面位置
            100.0f );                               // far plane，远平面位置

    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

void loadShaderData_cube( Shader &shaderProgram )
{
    shaderProgram.use();

    shaderProgram.setInt( "task", 2 );

    shader.setInt( "texture0", 0 );

    // 模型矩阵
    glm::mat4 modelMatrix( 1.0f );

    // 视图变换矩阵
    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix =  glm::perspective(
            glm::radians( pCamera->fov() ),            // FOV，field of view，视角广度
            ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
            0.1f,                                      // near plane，近平面位置
            100.0f );                                  // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

void loadShaderData_floor( Shader &shaderProgram )
{
    shaderProgram.use();

    shaderProgram.setInt( "task", 3 );
    shader.setInt( "texture0", 0 );

    glm::mat4 modelMatrix( 1.0f );                      // 模型矩阵

    glm::mat4 viewMatrix = pCamera->getViewMatrix();    // 视图矩阵

    glm::mat4 projectionMatrix = glm::perspective(  // 投影矩阵
            glm::radians( pCamera->fov() ),             // 就是FOV，field of view，视角广度
            ( float ) ScreenWidth / ScreenHeight,       // aspectRatio，宽高比
            0.1f,                                       // near plane，近平面位置
            100.0f );                                   // far plane，远平面位置

    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

#endif