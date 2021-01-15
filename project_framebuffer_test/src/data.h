#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

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
    // 顶点坐标             //纹理坐标      //颜色
    -10000.0f, 0.0f, 0.0f,  0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,   0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,   0.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,  0.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,   0.0f, 0.0f,     0.0f, 0.0f, 1.0f,   // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,  0.0f, 0.0f,     0.0f, 0.0f, 1.0f,   // Z轴上顶点位置2
};

// NDC坐标，[-1.0, 1.0]之间
const float screenVertices[] =
{
    // positions          // texCoords
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, -1.0f, 0.0f,    1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
    1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,    1.0f, 1.0f
};

enum SHADER_TASK
{
    TASK_COORD = 0,
    TASK_OBJECT,
    TASK_SCREEN,
    TASK_EFFECT_REVERSE,            // 反相
    TASK_EFFECT_GRAY_SCALE,         // 灰度
    TASK_EFFECT_GRAY_SCALE_WEIGHT,  // 加权灰度
    TASK_EFFECT_KERNEL,             // 核效果
    TASK_EFFECT_BLUR,               // 模糊
    TASK_EFFECT_EDGE_DETECTION,     // 边缘检测
    TASK_END
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

GLuint VAO_screen = 0;               // 地板的VAO、VBO
GLuint VBO_screen = 0;

GLuint textureCube = -1;            // 箱子纹理
GLuint textureFloor = -1;           // 地板纹理

GLuint frameBuffer = -1;            // 帧缓冲
GLuint textureColorBuffer;

glm::mat4 modelMatrix( 1.0f );       // 观察矩阵
glm::mat4 viewMatrix( 1.0f );       // 观察矩阵
glm::mat4 projectionMatrix( 1.0f ); // 投影矩阵


GLuint createFrameBuffer();

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
    createVertexBuffer( coordVertices, sizeof( coordVertices ), "323", &VAO_coord, &VBO_coord );
    createVertexBuffer( screenVertices, sizeof( screenVertices ), "32", &VAO_screen, &VBO_screen );

    textureFloor = createTexture( projectDir() + "/res/metal.png" );
    textureCube = createTexture( projectDir() + "/res/marble.jpg" );

    frameBuffer = createFrameBuffer();
}

// 创建自定义帧缓冲例子
GLuint createFrameBuffer()
{
    // ***************************************************
    // ******* 第一步，创建帧缓冲
    // ***************************************************
    GLuint frameBuffer;
    glGenFramebuffers( 1, &frameBuffer );               // 创建自己的帧缓冲
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );   // 暂时绑定到当前帧缓冲（代替默认帧缓冲），目的是保证后续的附件绑定能生效，在最后别忘记恢复到默认的帧缓冲

    // ***************************************************
    // ******* 第二步，绑定颜色附件，创建的纹理类型，原因是后续我们需要进行采样（读取数据）。
    // ***************************************************
    glGenTextures( 1, &textureColorBuffer );
    glBindTexture( GL_TEXTURE_2D, textureColorBuffer ); // 暂时绑定到当前激活纹理单元，目的是保证后面开辟纹理内存能生效，最后别忘了恢复成默认

    // 开辟纹理内存
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,     // 和纹理一样的参数类型
                  ScreenWidth, ScreenHeight,    // 设置为一个指定大小（比如屏幕大小）
                  0,                            // 总是0
                  GL_RGB, GL_UNSIGNED_BYTE,     // 和纹理一样的参数类型
                  NULL );                       // 空纹理数据，暂不填充

    // 和普通纹理一样，设置纹理参数
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // 注意，核在对屏幕纹理的边缘进行采样的时候，由于还会对中心像素周围的8个像素进行采样，
    // 其实会取到纹理之外的像素。由于环绕方式默认是GL_REPEAT，所以在没有设置的情况下取到的是
    // 屏幕另一边的像素，而另一边的像素本不应该对中心像素产生影响，这就可能会在屏幕边缘产
    // 生很奇怪的条纹。为了消除这一问题，我们可以将屏幕纹理的环绕方式都设置为GL_CLAMP_TO_EDGE。
    // 这样子在取到纹理外的像素时，就能够重复边缘的像素来更精确地估计最终的值了。
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glBindTexture( GL_TEXTURE_2D, 0 );          // 别忘了恢复成默认的，到这一步，纹理附件已经创建完成

    // 绑定到我们创建的帧缓冲中
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0 );

    // ***************************************************
    // ******* 第三步，绑定一个深度和模板附件（深度和模板数据都保存在一块内存中）,使用的渲染缓冲类型，原因是我们只会写入数据，不进行采样读取，使用渲染缓冲，性能会非常好。
    // ***************************************************
    GLuint renderbuffer;
    glGenRenderbuffers( 1, &renderbuffer );
    glBindRenderbuffer( GL_RENDERBUFFER, renderbuffer );    // 暂时绑定，目的保证下面的开辟内存能生效

    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight );

    glBindRenderbuffer( GL_RENDERBUFFER, 0 );               // 恢复成默认绑定

    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer );   // 绑定渲染缓冲

    // ***************************************************
    // ******* 第四步：检查帧缓冲的完整性，是否所有必需的工作都做完了，绑定附件就是必须的。
    // ***************************************************
    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        cout << "ERROR::FRAMEBUFFER:: FRAMEBUFFER is not complete" << endl;
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );     // 恢复到默认的帧缓冲，后续有需要，随时绑定使用。

    return frameBuffer;
}

#endif