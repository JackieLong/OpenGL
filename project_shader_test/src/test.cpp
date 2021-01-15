#include <iostream>
#include <fstream>
#include <functional>
#include "project_lib.h"

void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );
void createVertexBuffer( GLuint &VAO, GLuint &VBO, GLuint &EBO );

int main()
{
    initGLFW();     // 初始化GLFW

    GLFWwindow *window = createWindow( 800, 600, "LearnOpenGL" );

    initGLAD();     // 初始化GLAD

    Shader shader = Shader( projectDir() + "/src/shader/shader_vertex",
                            projectDir() + "/src/shader/shader_fragment" );

    GLuint VAO = 0, VBO = 0, EBO = 0;
    createVertexBuffer( VAO, VBO, EBO );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE：线框模式；GL_FILL：填充模式

    renderLoop( window, [&]                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

        shader.use();                               // 启用着色器程序
        shader.setVec4( "appColor", 0.0f, ( float )( sin( glfwGetTime() ) / 2.0f ) + 0.5f, 0.0f, 1.0f );

        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );

    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
}

void renderLoop( GLFWwindow *window, std::function<void()> renderCallback )
{
    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );                     // 处理输入：键盘、鼠标事件等。

        if( renderCallback != nullptr )             // render loop
        {
            renderCallback();
        }

        glfwSwapBuffers( window );  // 双缓冲

        // IO轮询，检查有没有触发什么事件（keys pressed/released, mouse moved etc.）,
        // 并调用对应的回调函数（可以通过回调方法手动设置）,更新窗口状态，
        glfwPollEvents();
    }
}

void createVertexBuffer( GLuint &VAO, GLuint &VBO, GLuint &EBO )
{
    // 3个顶点绘制一个三角形
    float vertices[] =
    {
        // 位置                  // 颜色
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, 1.0f, // 右下
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, // 左下
        0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f, // 顶部
    };

    unsigned int indices[] =    // 三角形的顶点索引
    {
        0, 1, 2,                // 第一个三角形
    };

    createVertexBuffer( vertices, sizeof( vertices ), "34", &VAO, &VBO, indices, sizeof( indices ), &EBO );
}