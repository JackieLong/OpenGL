#include "project_lib.h"
#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos );
void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset );
void initGLState();
void InitGLFW();
void drawMultiSample();
void drawMultiSampleToDefaultFBO();
void drawMultSamplePostProcess();
bool multiSampleOn = true;

int main()
{
    InitGLFW();

    GLFWwindow *window = createWindow( ScreenWidth,
                                       ScreenHeight,
                                       "LearnOpenGL",
                                       []( GLFWwindow * window, double xoffset, double yoffset )
    {
        pCamera->processMouseMovement( window, xoffset, yoffset );
    },
    []( GLFWwindow * window, double xoffset, double yoffset )
    {
        pCamera->processMouseScroll( window, xoffset, yoffset );
    } );

    initGLAD();

    initGLState();

    loadData();

    renderLoop( window, [&]
    {
        shader.use();
        shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
        shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
        shader.setMat4( "projectionMatrix", glm::perspective(
                                        glm::radians( pCamera->fov() ),            // 就是FOV，field of view，视角广度
                                        ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                        0.1f,                                      // near plane，近平面位置
                                        100.0f ) );

        //drawMultiSample();                // 开启多重采样示例
        //drawMultiSampleToDefaultFBO();    // 将自定义多重采样帧缓冲颜色附件还原至默认帧缓冲的颜色附件中，直接输出图像
        //drawMultSamplePostProcess();      // 多重采样后期处理示例，其实就是保存成一个2D纹理，供片段着色器逐片段处理。
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO_object );
    glDeleteVertexArrays( 1, &VAO_coord );
    glDeleteBuffers( 1, &VBO_object );
    glDeleteBuffers( 1, &VBO_coord );
    glDeleteFramebuffers( 1, &fbo_ms );
    glDeleteFramebuffers( 1, &fbo );
    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void drawMultiSample()
{
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.setInt( "task", TASK_COORD );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_CUBE );
    glBindVertexArray( VAO_object );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
}

void drawMultiSampleToDefaultFBO()
{
    // 绘制数据输出到多重采样帧缓冲纹理附件
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_ms );

    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.setInt( "task", TASK_COORD );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_CUBE );
    glBindVertexArray( VAO_object );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo_ms );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

    // 将多重采样纹理数据还原为普通2D纹理数据，且直接保存到默认帧缓冲颜色附件，即可直接输出图像
    glBlitFramebuffer( 0, 0, ScreenWidth, ScreenHeight,
                       0, 0, ScreenWidth, ScreenHeight,
                       GL_COLOR_BUFFER_BIT, GL_NEAREST );
}

void drawMultSamplePostProcess()
{
    // 绘制数据输出到多重采样帧缓冲纹理附件
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_ms );

    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.setInt( "task", TASK_COORD );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_CUBE );
    glBindVertexArray( VAO_object );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo_ms );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );
    // 将多重采样纹理数据还原为普通2D纹理数据，这样片段着色器可以通过标准采样函数进行采样
    glBlitFramebuffer( 0, 0, ScreenWidth, ScreenHeight,
                       0, 0, ScreenWidth, ScreenHeight,
                       GL_COLOR_BUFFER_BIT, GL_NEAREST );

    // 绑定为默认帧缓冲，准备将刚刚的2D纹理数据输出到屏幕
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glDisable( GL_DEPTH_TEST );

    glBindVertexArray( VAO_screen );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textureScreen );  // 这是还原的2D纹理数据

    shader.use();
    shader.setInt( "texture0", 0 );
    shader.setInt( "task", TASK_MULTISAMPLE_POST_PROCESS );
    glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
    if( multiSampleOn )
    {
        glEnable( GL_MULTISAMPLE );
    }
}

void InitGLFW()
{
    //--------------------------------------------------------- 1、初始化glfw
    glfwInit();                                                                                                                                                       // 初始化GLFW

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );                    // 主版本号：3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );                    // 次版本号：3
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );    // 使用opengl core profile核心模式，导致必须使用VAO。
    if( multiSampleOn )
    {
        glfwWindowHint( GLFW_SAMPLES, 4 );                                  // 多重采样4个采样点
    }

    #ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
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

void renderLoop( GLFWwindow *window, function<void()> renderCallback )
{
    double currentFrame;
    while( !glfwWindowShouldClose( window ) )
    {
        currentFrame = glfwGetTime();
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
