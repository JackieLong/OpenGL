#include "data.h"

void initGLState();
void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );

bool envir_debug = true;    // 测试环境？生产环境？

int main()
{
    initGLFW();     // 初始化GLFW
    if( envir_debug )
    {
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );   // 请求调试上下文
    }

    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );
    initGLAD();     // 初始化GLAD

    // 尝试切换注释下面几句，查看效果
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    if( envir_debug )
    {
        initDebugOutputCallback();      // gl调用出错，将触发回调
    }

    loadData();

    renderLoop( window, [&]
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        renderText( shaderText,
                    "This is sample text.",
                    glm::vec2( 25.0f, 25.0f ),
                    1.0f,
                    glm::vec3( 0.5f, 0.8f, 0.2f ) );

        renderText( shaderText,
                    "(C) LearnOpengL.com",
                    glm::vec2( 540.0f, 570.0f ),
                    0.5f,
                    glm::vec3( 0.3f, 0.7f, 0.9f ) );
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO_quad );
    glDeleteBuffers( 1, &VBO_quad );
    glDeleteProgram( shaderText.id );

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
