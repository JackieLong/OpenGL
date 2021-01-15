#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 600;

// 6个顶点（6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点）
// 这里没有使用索引缓冲对象
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
    -10000.0f, 0.0f, 0.0f,      // X轴上顶点位置1
    10000.0f, 0.0f, 0.0f,       // X轴上顶点位置2

    0.0f, 10000.0f, 0.0f,       // Y轴上顶点位置1
    0.0f, -10000.0f, 0.0f,      // Y轴上顶点位置2

    0.0f, 0.0f, 10000.0f,       // Z轴上顶点位置1
    0.0f, 0.0f, -10000.0f,      // Z轴上顶点位置2
};

glm::vec3 lightColor = glm::vec3( 1.0f, 1.0f, 1.0f );           // 光源颜色
glm::vec3 diffuseColor = lightColor   * glm::vec3( 0.5f );      // 降低影响
glm::vec3 ambientColor = diffuseColor * glm::vec3( 0.2f );      // 很低的影响

Camera *pCamera = nullptr;                         // 摄像机
double deltaTime       = 0.0;         // 当前帧与上一帧的时间差
double lastFrame       = 0.0;         // 上一帧的时间

Shader shader, shader_coord, shader_light;

GLuint VAO_coord = 0, VBO_coord = 0;                          // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象

void loadData()
{
    pCamera = new Camera( glm::vec3( 1.0f, 1.0f, 7.0f ),            // 摄像机的初始位置
                          glm::vec3( 0.5f, 0.5f, 0.0f ),            // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ),            // 世界坐标中的向上向量
                          -90.f,                                    // Yaw
                          0.0f );                                   // Pitch

    shader = Shader( projectDir() + "\\src\\shader\\shader_vertex",
                     projectDir() + "\\src\\shader\\shader_fragment" );

    shader_coord = Shader( projectDir() + "\\src\\shader\\shader_vertex_coord",
                           projectDir() + "\\src\\shader\\shader_fragment_coord" );

    shader_light = Shader( projectDir() + "\\src\\shader\\shader_vertex_light",
                           projectDir() + "\\src\\shader\\shader_fragment_light" );

    //loadVertexData( vertices_object, sizeof( vertices_object ), "32", &VAO_object, &VBO_object );
    createVertexBuffer( vertices_coord, sizeof( vertices_coord ), "3", &VAO_coord, &VBO_coord );
}

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }

    pCamera->processKeyboard( window, ( float ) deltaTime );
}

void renderLoop( GLFWwindow *window, function<bool()> renderCallback )
{
    double currentFrame;

    while( !glfwWindowShouldClose( window ) )
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput( window );                     // 处理输入：键盘、鼠标事件等。

        if( renderCallback != nullptr && !renderCallback() )             // render loop
        {
            glfwSetWindowShouldClose( window, true );
        }

        glfwSwapBuffers( window );                  // 双缓冲

        // IO轮询，检查有没有触发什么事件（keys pressed/released, mouse moved etc.）,
        // 并调用对应的回调函数（可以通过回调方法手动设置）,更新窗口状态，
        glfwPollEvents();
    }
}

#endif