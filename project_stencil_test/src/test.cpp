#include "project_lib.h"
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

    glm::mat4 scaleMatrix( 1.0f );
    scaleMatrix = glm::scale( scaleMatrix, glm::vec3( 1.05f ) );

    renderLoop( window, [&]     // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                                         // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );   // 状态使用函数：清空颜色缓冲区和深度缓冲区

        shader.use();
        loadShaderData();

        glStencilMask( 0x00 );                  // 不更新模板缓冲

        // -----------------绘制坐标轴
        shader.setInt( "task", 1 );
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

        // -----------------绘制地板
        shader.setInt( "task", 4 );
        glBindVertexArray( VAO_floor );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, textureFloor );
        glDrawArrays( GL_TRIANGLES, 0, 6 );

        glStencilMask( 0xFF );                  // 开启写入模板缓冲
        glStencilFunc( GL_ALWAYS, 1, 0xFF );    // 总是写入1到模板缓冲
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );

        // -----------------绘制箱子
        shader.setInt( "task", 2 );
        glBindVertexArray( VAO_cube );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, textureCube );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glStencilMask( 0x00 );                  // 禁止写入模板缓冲
        glStencilFunc( GL_NOTEQUAL, 1, 0xFF );  // 只保留模板值不等于1的片段

        // -----------------绘制大一点的箱子
        shader.setMat4( "modelMatrix", scaleMatrix );
        shader.setInt( "task", 3 );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glStencilMask( 0xFF );                  // 开启写入模板缓冲
        glStencilFunc( GL_ALWAYS, 0, 0xFF );    // 其他屏幕片段的模板缓冲都更新为0


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
        deltaTime    = currentFrame - lastFrame;
        lastFrame    = currentFrame;

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
    glDepthFunc( GL_LESS );
    glEnable( GL_STENCIL_TEST );                        // 开启模板测试，默认不开启。
}
