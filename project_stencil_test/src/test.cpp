﻿#include "project_lib.h"
#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void processInput( GLFWwindow *window );
void initGLState();

int main()
{
    initGLFW();     // 初始化GLFW

    GLFWwindow *window = createWindow( ScreenWidth,
                                       ScreenHeight,
                                       "LearnOpenGL",
                                       []( GLFWwindow * window, double xpos, double ypos )      // 鼠标滑动回调
    {
        pCamera->processMouseMovement( window, xpos, ypos );
    },
    []( GLFWwindow * window, double xoffset, double yoffset )       // 鼠标滚轮回调
    {
        pCamera->processMouseScroll( window, xoffset, yoffset );
    } );

    initGLAD();     // 初始化GLAD

    initGLState();  // OpenGL起始状态设置

    loadData();     // 加载顶点数据、纹理数据等

    renderLoop( window, [&]                                     // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                 // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   // 状态使用函数：清空颜色缓冲区和深度缓冲区

        loadShaderData_coord( shader );                         // 绘制坐标轴
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

        loadShaderData_cube( shader );                          // 绘制箱子
        glBindVertexArray( VAO_cube );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, textureCube );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        loadShaderData_floor( shader );                         // 绘制地板
        glBindVertexArray( VAO_floor );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, textureFloor );
        glDrawArrays( GL_TRIANGLES, 0, 6 );

        glBindVertexArray( 0 );
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO_coord );
    glDeleteVertexArrays( 1, &VAO_cube );
    glDeleteVertexArrays( 1, &VAO_floor );

    glDeleteBuffers( 1, &VBO_coord );
    glDeleteBuffers( 1, &VBO_cube );
    glDeleteBuffers( 1, &VBO_floor );

    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void renderLoop( GLFWwindow *window, function<void()> renderCallback )
{
    double currentFrame;
    while( !glfwWindowShouldClose( window ) )
    {
        currentFrame = glfwGetTime();               // 计算帧间隔
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput( window );                     // 处理输入：键盘、鼠标事件等。

        if( renderCallback != nullptr )             // render loop
        {
            renderCallback();
        }

        glfwSwapBuffers( window );                  // 双缓冲

        // IO轮询，检查有没有触发什么事件（keys pressed/released, mouse moved etc.）,
        // 并调用对应的回调函数（可以通过回调方法手动设置）,更新窗口状态，
        glfwPollEvents();
    }
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

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
}
