#include "project_lib.h"
#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void processInput( GLFWwindow *window );
void drawToCustomFrameBuffer();
void drawToDefaultFrameBuffer();

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

    loadData();     // 加载顶点数据、纹理数据等

    renderLoop( window, [&]                                     // 渲染循环
    {
        modelMatrix = glm::mat4( 1.0f );
        viewMatrix = pCamera->getViewMatrix();

        // 投影变换矩阵（透视投影）
        projectionMatrix = glm::perspective(
                        glm::radians( pCamera->fov() ),         // FOV，field of view，视角广度
                        ( float ) ScreenWidth / ScreenHeight,   // aspectRatio，宽高比
                        0.1f,                                   // near plane，近平面位置
                        100.0f );                               // far plane，远平面位置
        shader.use();
        shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
        shader.setMat4( "viewMatrix", viewMatrix );
        shader.setMat4( "projectionMatrix", projectionMatrix );

        drawToCustomFrameBuffer();
        drawToDefaultFrameBuffer();
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

void drawToCustomFrameBuffer()
{
    // ***************************************************
    // ******* 使用自定义缓冲，后续的渲染数据会写入颜色附件（离屏渲染），深度值和模板值也有相应的附件保存。
    // ***************************************************
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    glEnable( GL_DEPTH_TEST );

    glClearColor( 0.1f, 0.1f, 0.1f, 0.1f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 使用不使用模板缓冲

    // ------------------------------------------
    shader.setInt( "task", TASK_COORD );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    // ------------------------------------------
    shader.setInt( "task", TASK_OBJECT );
    glBindVertexArray( VAO_cube );
    glBindTexture( GL_TEXTURE_2D, textureCube );
    shader.setMat4( "modelMatrix", glm::translate( glm::mat4( 1.0f ), glm::vec3( -1.0f, 0.0f, -1.0f ) ) );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    shader.setMat4( "modelMatrix", glm::translate( glm::mat4( 1.0f ), glm::vec3( 2.0f, 0.0f, 0.0f ) ) );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    // ------------------------------------------
    shader.setInt( "task", TASK_OBJECT );
    glBindVertexArray( VAO_floor );
    glBindTexture( GL_TEXTURE_2D, textureFloor );
    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void drawToDefaultFrameBuffer()
{
    // ***************************************************
    // ******* 使用默认帧缓冲，恢复成正常默认，
    // ***************************************************
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );          // 我们所看到的3D画面，其实就是一张平面图，即通过两个三角形组成的矩形绘制出来的。
    glDisable( GL_DEPTH_TEST );                             // 就是平面图，不需要深度值了。

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    shader.setInt( "task", TASK_SCREEN );
    glBindTexture( GL_TEXTURE_2D, textureColorBuffer );     // 使用颜色附件数据来纹理贴图。

    glBindVertexArray( VAO_screen );
    glDrawArrays( GL_TRIANGLES, 0, 6 );
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