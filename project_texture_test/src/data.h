#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

const int ScreenWidth = 800;
const int ScreenHeight = 600;

// 4个绘制两个三角形，组成一个矩形。
const GLfloat vertices[] =
{
    //-------------显示一张三角形纹理
    // 位置               // 颜色             // 纹理坐标
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,     0.0f, 0.0f, 0.0f,   0.5f, 1.0f,

    //-------------显示一张矩形纹理
    // 位置                // 颜色             // 纹理坐标
    0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // 右上
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // 右下
    -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // 左上
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // 左下

    //-------------矩形，纹理放大，MAG_FILTER
    // 位置                // 颜色             // 纹理坐标
    0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 0.0f,   0.75f, 0.75f,  // 右上
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   0.75f, 0.65f,  // 右下
    -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   0.65f, 0.75f,  // 左上
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.65f, 0.65f,  // 左下

    //-------------矩形，纹理缩小，MIN_FILTER
    // 位置                // 颜色             // 纹理坐标
    0.25f, 0.25f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // 右上
    0.25f, -0.25f, 0.0f,    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // 右下
    -0.25f, 0.25f, 0.0f,    0.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // 左上
    -0.25f, -0.25f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // 左下

    //-------------矩形，环绕
    // 位置                // 颜色             // 纹理坐标
    0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 0.0f,   2.0f, 2.0f,  // 右上
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   2.0f, 0.0f,  // 右下
    -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 0.0f,   0.0f, 2.0f,  // 左上
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // 左下
};
// OpenGL规定：顶点坐标（0.0，0.0）在中心点，而纹理坐标（0.0，0.0）在左下角
// 在常见图片格式中，（0.0，0.0）可能是在图片左上角，因此可能导致图片倒转。

const GLuint indices[] =    // 矩形顶点索引
{
    // 绘制一个三角形：显示一张纹理
    0, 1, 2,

    // 绘制一个矩形：显示一张纹理
    3, 4, 5,
    4, 5, 6,

    // 绘制一个矩形：纹理放大，MAG_FILTER
    7, 8, 9,
    8, 9, 10,

    // 绘制一个矩形：纹理缩小，MIN_FILTER
    11, 12, 13,
    12, 13, 14,

    // 绘制一个矩形：WRAPING环绕
    15, 16, 17,
    16, 17, 18,
};

const GLfloat vertices_coord[] =   // 坐标轴顶点数据
{
    // 顶点坐标              // 颜色
    -10000.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    10000.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    0.0f, 10000.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    0.0f, -10000.0f, 0.0f,  0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 10000.0f,   0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, -10000.0f,  0.0f, 0.0f, 1.0f,
};

const GLuint indices_coord[] =
{
    0, 1,   // 两个连线构成X轴
    2, 3,   // 两个连线构成Y轴
    4, 5    // 两个连线构成Z轴
};

double deltaTime = 0.0;
double lastFrame = 0.0;

GLuint VAO = 0, VBO = 0, EBO = 0;
GLuint VAO_coord = 0, VBO_coord = 0, EBO_coord = 0;

GLuint texture_wall;
GLuint texture_wall_nearest;

GLuint texture_wall_repeat;
GLuint texture_wall_mirrored_repeat;
GLuint texture_wall_clamp_to_edge;
GLuint texture_wall_clamp_to_border;

GLuint texture_face;

Shader shader;

Camera *pCamera = nullptr;

void loadData()
{
    pCamera = new Camera( glm::vec3( 0.0f, 0.0f, 3.0f ),
                          glm::vec3( 0.0f ),
                          glm::vec3( 0.0f, 1.0f, 0.0f ) );

    shader = Shader( projectDir() + "\\src\\shader\\shader_vertex",     // 顶点着色器脚本
                     projectDir() + "\\src\\shader\\shader_fragment" ); // 片段着色器脚本

    createVertexBuffer( vertices, sizeof( vertices ), "332", &VAO, &VBO, indices, sizeof( indices ), &EBO );

    createVertexBuffer( vertices_coord, sizeof( vertices_coord ), "33", &VAO_coord, &VBO_coord, indices_coord, sizeof( indices_coord ), &EBO_coord );

    shader.use();

    texture_wall = createTexture( projectDir() + "\\res\\wall.jpg", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略
    } );

    texture_wall_nearest = createTexture( projectDir() + "\\res\\wall.jpg", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );     // 纹理放大时，采用GL_LINEAR策略
    } );

    texture_wall_repeat = createTexture( projectDir() + "\\res\\awesomeface.png", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );     // 纹理放大时，采用GL_LINEAR策略
    } );

    texture_wall_mirrored_repeat = createTexture( projectDir() + "\\res\\awesomeface.png", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );     // 纹理放大时，采用GL_LINEAR策略
    } );

    texture_wall_clamp_to_edge = createTexture( projectDir() + "\\res\\wall.jpg", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );     // 纹理放大时，采用GL_LINEAR策略
    } );

    texture_wall_clamp_to_border = createTexture( projectDir() + "\\res\\awesomeface.png", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );     // 纹理放大时，采用GL_LINEAR策略
    } );

    texture_face = createTexture( projectDir() + "\\res\\awesomeface.png", []
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );         // “X”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );         // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略
    } );
}

#endif