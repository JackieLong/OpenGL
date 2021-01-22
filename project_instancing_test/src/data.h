#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth  = 800;
const int ScreenHeight = 600;

GLfloat vertices_quad[] =
{
    // 位置                 // 颜色
    -0.05f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f,    0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f, 0.0f,   0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f,    0.0f, 1.0f, 0.0f,
    0.05f,  0.05f, 0.0f,    0.0f, 1.0f, 1.0f
};

GLfloat vertices_coord[] =          // 绘制坐标系的顶点
{
    // 顶点坐标
    -10000.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,       // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,       // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,   0.0f, 1.0f, 0.0f,       // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,  0.0f, 1.0f, 0.0f,       // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,   0.0f, 0.0f, 1.0f,       // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,  0.0f, 0.0f, 1.0f,       // Z轴上顶点位置2
};

Camera *pCamera  = nullptr;
double deltaTime = 0.0;         // 当前帧与上一帧的时间差
double lastFrame = 0.0;         // 上一帧的时间

Shader shader_quad;
Shader shader_coord;
Shader shader_planet;
Shader shader_rock;

GLuint VAO_quad = 0;
GLuint VBO_quad = 0;
GLuint VBO_offset;
GLuint VAO_coord = 0;
GLuint VBO_coord = 0;
GLuint VBO_modelMatrix_rock;

glm::mat4 viewMatrix, projectionMatrix, rotateSelfMatrix, rotatePlanetMatrix;

GLfloat translations[2 * 100];

const int ROCK_NUM = 5000;
vector<glm::mat4> vModelMatrix_rock;

Model *modelPlanet = nullptr;
Model *modelRock   = nullptr;


void loadInstanceVertexAttrib( GLuint &VBO, const vector<glm::mat4> &vModelMatrix_rock );

void loadData()
{
    pCamera = new Camera( glm::vec3( 0.0f, 30.0f, 30.0f ),      // 摄像机的初始位置
                          glm::vec3( 0.0f, 0.0f, 0.0f ),        // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ),        // 世界坐标中的向上向量
                          -90.0f,
                          -45.0f );

    // 矩形框着色器
    shader_quad   = Shader( projectDir() + "\\src\\shader\\shader_vertex_quad",
                            projectDir() + "\\src\\shader\\shader_fragment_quad" );
    // 坐标着色器
    shader_coord  = Shader( projectDir() + "\\src\\shader\\shader_vertex_coord",
                            projectDir() + "\\src\\shader\\shader_fragment_coord" );
    // 行星着色器
    shader_planet = Shader( projectDir() + "\\src\\shader\\shader_vertex_planet",
                            projectDir() + "\\src\\shader\\shader_fragment_planet" );
    // 陨石着色器
    shader_rock   = Shader( projectDir() + "\\src\\shader\\shader_vertex_rock",
                            projectDir() + "\\src\\shader\\shader_fragment_rock" );

    createVertexBuffer( vertices_quad, sizeof( vertices_quad ), "33", &VAO_quad, &VBO_quad );
    createVertexBuffer( vertices_coord, sizeof( vertices_coord ), "33", &VAO_coord, &VBO_coord );

    // 要绘制100个矩形框，每个矩形框有一个不同的位移。
    int   index  = 0;
    float offset = 0.1f;
    for( int y = -10; y < 10; y += 2 )
    {
        for( int x = -10; x < 10; x += 2 )
        {
            translations[index]     = ( float ) x / 10.0f + offset;
            translations[index + 1] = ( float ) y / 10.0f + offset;
            index += 2;
        }
    }

    // 每个矩形框实例施加一个不同的位移变换矩阵
    updateVertexBuffer( &VAO_quad, translations, sizeof( translations ), "2", &VBO_offset, 2, nullptr, 0, nullptr, []
    {
        // 设置实例渲染的时，顶点属性数据的前进速率（更新速度）。
        // 这里的作用是每个矩形（6个顶点）使用相同的location=2的顶点属性值
        glVertexAttribDivisor( 2,       // 更新location = 2的属性值
                               1 );     // 0：每个顶点着色器迭代更新一次，默认情况，即逐顶点更新
        // 1：每个实例渲染更新一次。
        // 2：每2个实例渲染更新一次。
    } );

    // 行星模型
    modelPlanet = new Model( projectDir() + "/res/planet/planet.obj" );
    // 陨石模型
    modelRock   = new Model( projectDir() + "/res/rock/rock.obj" );

    // 行星带的每颗陨石有一个对应的模型矩阵，其物理意义是：
    // 每颗陨石在随机分布在行星带上，有（半）随机的转动方向，有随机的大小
    vModelMatrix_rock.reserve( ROCK_NUM );

    const float radius_start  = 25.0f;
    const float radius_end    = 30.0f;
    const float radius_height = ( radius_start - radius_end ) * 0.4f;
    for( int i = 0; i < ROCK_NUM; i++ )
    {
        glm::mat4 tmpModel( 1.0f );

        // 随机位置：随机分布在一个环上，环内径radius_start，外径radius_end，高度radius_height
        float randAngle = glm::radians( randomFloat() * 360 );
        float radius = radius_start + ( radius_end - radius_start ) * randomFloat();
        tmpModel = glm::translate( tmpModel,
                                   glm::vec3( glm::sin( randAngle ) * radius,
                                              randomFloat() * radius_height,
                                              glm::cos( randAngle ) * radius ) );

        // 随机的大小：0.05 ~ 0.25大小倍数之间
        tmpModel = glm::scale( tmpModel, glm::vec3( randomFloat() * 0.2f + 0.05f ) );

        // 半随机旋转角度
        tmpModel = glm::rotate( tmpModel, glm::radians( randomFloat() * 360 ), glm::vec3( 0.4f, 0.6f, 0.8f ) );

        vModelMatrix_rock.push_back( tmpModel );
    }

    // 将模型矩阵数组设置为实例化顶点属性数据
    loadInstanceVertexAttrib( VBO_modelMatrix_rock, vModelMatrix_rock );
}

void loadInstanceVertexAttrib( GLuint &VBO, const vector<glm::mat4> &vModelMatrix_rock )
{
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, ROCK_NUM * sizeof( glm::mat4 ), &vModelMatrix_rock[0], GL_STATIC_DRAW );

    const GLsizei vec4Size      = sizeof( glm::vec4 );
    const GLsizei stride        = 4 * vec4Size;
    const GLint   components    = 4;
    const GLuint  startLocation = 10;       // 矩阵顶点属性位置值（第一个vec4的位置值）
    for( auto &mesh : modelRock->mMeshes )
    {
        glBindVertexArray( mesh.mVAO );

        // 设置矩阵类型顶点属性的方法，实际上就是把矩阵看成一个4 x vec4的向量数组。
        glEnableVertexAttribArray( startLocation + 0 );
        glVertexAttribPointer( startLocation + 0, components, GL_FLOAT, GL_FALSE, stride, ( GLvoid * )( 0 * vec4Size ) );
        glEnableVertexAttribArray( startLocation + 1 );
        glVertexAttribPointer( startLocation + 1, components, GL_FLOAT, GL_FALSE, stride, ( GLvoid * )( 1 * vec4Size ) );
        glEnableVertexAttribArray( startLocation + 2 );
        glVertexAttribPointer( startLocation + 2, components, GL_FLOAT, GL_FALSE, stride, ( GLvoid * )( 2 * vec4Size ) );
        glEnableVertexAttribArray( startLocation + 3 );
        glVertexAttribPointer( startLocation + 3, components, GL_FLOAT, GL_FALSE, stride, ( GLvoid * )( 3 * vec4Size ) );

        // 每一个实例迭代更新一次
        glVertexAttribDivisor( startLocation + 0, 1 );
        glVertexAttribDivisor( startLocation + 1, 1 );
        glVertexAttribDivisor( startLocation + 2, 1 );
        glVertexAttribDivisor( startLocation + 3, 1 );

        glBindVertexArray( 0 );
    }
}

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
}

#endif