#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

const int ScreenWidth = 800;
const int ScreenHeight = 600;

// 6个顶点（6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点）
// 这里没有使用索引缓冲对象
const GLfloat vertices_object[] =
{
    // 物体顶点坐标           // 顶点的单位法向量
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f
};

// 6个顶点（6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点）
// 这里没有使用索引缓冲对象
float vertices_light[] =
{
    // 顶点坐标          // 纹理坐标
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

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
    0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,

};

const GLfloat vertices_coord[] =          // 绘制坐标系的顶点
{
    // 顶点坐标
    -10000.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,      // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,      // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,   0.0f, 1.0f, 0.0f,      // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,  0.0f, 1.0f, 0.0f,      // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,   0.0f, 0.0f, 1.0f,      // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,  0.0f, 0.0f, 1.0f,      // Z轴上顶点位置2
};

Camera *pCamera      = nullptr;
glm::vec3 lightPos;                                     // 光源位置
glm::vec3 lightColor = glm::vec3( 1.0f, 1.0f, 1.0f );   // 光源颜色
double deltaTime     = 0.0;         // 当前帧与上一帧的时间差
double lastFrame     = 0.0;         // 上一帧的时间

Shader shaderProgram, shaderProgramCoord, shaderProgramLight;

GLuint VAO_object    = 0, VAO_coord = 0, VAO_light = 0;           // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
GLuint VBO_object    = 0, VBO_coord = 0, VBO_light = 0;

GLuint texture;

void loadData()
{
    pCamera = new Camera( glm::vec3( 1.0f, 1.0f, 7.0f ),            // 摄像机的初始位置
                          glm::vec3( 0.5f, 0.5f, 0.0f ),            // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ) );          // 世界坐标中的向上向量

    createVertexBuffer( vertices_object, sizeof( vertices_object ), "33", &VAO_object, &VBO_object );
    createVertexBuffer( vertices_light, sizeof( vertices_light ), "32", &VAO_light, &VBO_light );
    createVertexBuffer( vertices_coord, sizeof( vertices_coord ), "33", &VAO_coord, &VBO_coord );

    // 笑脸立方体着色器
    shaderProgram = Shader( projectDir() + "\\src\\shader\\shader_vertex",
                            projectDir() + "\\src\\shader\\shader_fragment" );

    // 坐标轴着色器
    shaderProgramCoord = Shader( projectDir() + "\\src\\shader\\shader_vertex_coord",
                                 projectDir() + "\\src\\shader\\shader_fragment_coord" );

    shaderProgramLight = Shader( projectDir() + "\\src\\shader\\shader_vertex_light",
                                 projectDir() + "\\src\\shader\\shader_fragment_light" );

    texture = createTexture( projectDir() + "\\res\\awesomeface.png" );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    shaderProgram.use();
    shaderProgram.setInt( "texture0", 0 );
}

#endif // !__DATA_H__
