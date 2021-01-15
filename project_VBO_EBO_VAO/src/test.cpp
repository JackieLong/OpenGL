#include "project_lib.h"
#include "data.h"

void initGLState();
void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );

int main()
{
    initGLFW();     // 初始化GLFW

    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );

    initGLAD();     // 初始化GLAD

    initGLState();

    shader = Shader( projectDir() + "/src/shader/shader_vertex", projectDir() + "/src/shader/shader_fragment" );

    int mode = MODE_VBO;
    switch( mode )
    {
        case MODE_VBO:
            createVertexBuffer( vertices_VBO, sizeof( vertices_VBO ), "3", &VAO, &VBO );
            break;
        case MODE_EBO:
            createVertexBuffer( vertices_VBO, sizeof( vertices_VBO ), "3", &VAO, &VBO, indices_EBO, sizeof( indices_EBO ), &EBO );
            break;
        default:
            break;
    }

    renderLoop( window, [&]                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

        shader.use();                               // 启用着色器程序

        switch( mode )
        {
            case MODE_VBO:                          // 通过VBO绘制图形
            {
                glBindVertexArray( VAO );           // 因为开启了core profile模式，所以必须使用VAO
                glDrawArrays( GL_TRIANGLES, 0, 6 );
                glBindVertexArray( 0 );
                break;
            }
            case MODE_EBO:                          // 通过EBO绘制图形
            {
                glBindVertexArray( VAO );
                glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
                glBindVertexArray( 0 );
                break;
            }
            default:
                break;
        }
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

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    // GL_LINE：线框模式；GL_FILL：填充模式
}