#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

float vertices_object[] =
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
float vertices_coord[] =          // 绘制坐标系的顶点
{
    // 顶点坐标
    -10000.0f, 0.0f, 0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,    // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Z轴上顶点位置2
};

GLfloat vertices_screen[] =
{
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
    1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,       1.0f, 1.0f,

    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,       1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f,      0.0f, 1.0f
};

enum ShaderTask
{
    TASK_COORD = 0,
    TASK_CUBE,
    TASK_MULTISAMPLE_POST_PROCESS,
    TASK_END,
};


Camera *pCamera = nullptr;
double deltaTime = 0.0;         // 当前帧与上一帧的时间差
double lastFrame = 0.0;         // 上一帧的时间

Shader shader;

GLuint VAO_object = 0, VAO_coord = 0;           // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
GLuint VBO_object = 0, VBO_coord = 0;
GLuint VAO_screen = 0, VBO_screen = 0;

GLuint fbo_ms, textureScreenMS;
GLuint fbo, textureScreen;

// 创建多重采样帧缓冲
void createFrameBufferMultisample( )
{
    glGenFramebuffers( 1, &fbo_ms );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_ms );

    // 颜色附件
    glGenTextures( 1, &textureScreenMS );
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE,           //多重采样纹理类型
                   textureScreenMS );
    glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, // glTexImage2DMultisample方法，多重采样纹理类型
                             4,                         // 采样点数
                             GL_RGB,                    // internalformat
                             ScreenWidth, ScreenHeight, // 纹理像素宽高
                             GL_TRUE );                 // 采用相同的采样点位置和数目
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, 0 );      // 回复默认绑定
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_TEXTURE_2D_MULTISAMPLE,              // 多重采样纹理类型
                            textureScreenMS,                        // 纹理ID
                            0 );                                    // mipmap level

    // 渲染缓冲附件
    GLuint depthStencilAttachment;
    glGenRenderbuffers( 1, &depthStencilAttachment );
    glBindRenderbuffer( GL_RENDERBUFFER, depthStencilAttachment );
    glRenderbufferStorageMultisample( GL_RENDERBUFFER,              // glRenderbufferStorageMultisample方法
                                      4,                            // 采样点数
                                      GL_DEPTH24_STENCIL8,          // 内部格式
                                      ScreenWidth, ScreenHeight );  // 像素宽高
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilAttachment );

    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )     // 检查完整性
    {
        cout << "ERROR::FRAMEBUFFER::FRAMEBUFFER is not complete1.";
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// 创建普通纹理附件的帧缓冲，用于保存从多重采样帧缓冲还原的纹理数据，最终用于绘制
void createFrameBufferTexture( )
{
    glGenFramebuffers( 1, &fbo );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );

    // 颜色附件
    glGenTextures( 1, &textureScreen );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textureScreen );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureScreen, 0 );

    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        cout << "ERROR::FRAMEBUFFER::FRAMEBUFFER is not complete2.";
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void loadData()
{
    pCamera = new Camera( glm::vec3( 0.0f, 0.0f, 3.0f ),            // 摄像机的初始位置
                          glm::vec3( 0.0f, 0.0f, 0.0f ),            // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ) );          // 世界坐标中的向上向量

    shader = Shader( projectDir() + "\\src\\shader\\shader_vertex",
                     projectDir() + "\\src\\shader\\shader_fragment" );

    createVertexBuffer( vertices_object, sizeof( vertices_object ), "32", &VAO_object, &VBO_object );
    createVertexBuffer( vertices_coord, sizeof( vertices_coord ), "323", &VAO_coord, &VBO_coord );
    createVertexBuffer( vertices_screen, sizeof( vertices_screen ), "32", &VAO_screen, &VBO_screen );

    createFrameBufferMultisample( );
    createFrameBufferTexture( );
}

#endif