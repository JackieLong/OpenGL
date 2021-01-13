#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void drawCoord();
void drawTrangleTex();
void drawTex();
void drawTex_MAG_FILTER_LINEAR();
void drawTex_MAG_FILTER_NEAREST();
void drawTex_MIN_FILTER_LINEAR();
void drawTex_MIN_FILTER_NEAREST();

void drawTex_WRAPING_REPEAT();
void drawTex_WRAPING_MIRRORED_REPEAT();
void drawTex_WRAPING_CLAMP_TO_EDGE();
void drawTex_WRAPING_CLAMP_TO_BORDER();

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

    initGLState();

    loadData();

    renderLoop( window, [&]                                     // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                 // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   // 状态使用函数：执行上面的状态设置。

        drawCoord();
        drawTrangleTex();
        drawTex();
        drawTex_MAG_FILTER_LINEAR();
        drawTex_MAG_FILTER_NEAREST();
        drawTex_MIN_FILTER_LINEAR();
        drawTex_MIN_FILTER_NEAREST();
        drawTex_WRAPING_REPEAT();
        drawTex_WRAPING_MIRRORED_REPEAT();
        drawTex_WRAPING_CLAMP_TO_EDGE();
        drawTex_WRAPING_CLAMP_TO_BORDER();
        //glBindVertexArray( VAO );

        //shader.use();
        //shader.setInt( "task", 1 );

        //glActiveTexture( GL_TEXTURE0 );
        //glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, ( GLvoid * )( 0 * sizeof( GLuint ) ) );

        ////glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 6 * sizeof( GLuint ) ) );
        ////glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 0 * sizeof( GLuint ) ) );

        //glBindVertexArray( 0 );
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

void drawCoord()
{
    shader.use();
    shader.setInt( "task", 0 );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
    shader.setMat4( "projectionMatrix",
                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glBindVertexArray( VAO_coord );
    glDrawElements( GL_LINES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 0 * sizeof( GLuint ) ) );
    glBindVertexArray( 0 );
}

void drawTrangleTex()
{
    shader.use();
    shader.setInt( "task", 1 );

    shader.setMat4( "modelMatrix", glm::translate( glm::mat4( 1.0f ), glm::vec3( 1.0f, 1.0f, 0.0f ) ) );
    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
    shader.setMat4( "projectionMatrix",
                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, ( GLvoid * )( 0 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex()
{
    shader.use();
    shader.setInt( "task", 2 );

    shader.setMat4( "modelMatrix", glm::translate( glm::mat4( 1.0f ), glm::vec3( 2.0f + 0.1f, 1.0f, 0.0f ) ) );
    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
    shader.setMat4( "projectionMatrix",
                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 3 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex_MAG_FILTER_LINEAR()
{
    shader.use();
    shader.setInt( "task", 3 );

    shader.setMat4( "modelMatrix", glm::translate( glm::mat4( 1.0f ), glm::vec3( 3.0f + 0.2f, 1.0f, 0.0f ) ) );
    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
    shader.setMat4( "projectionMatrix",
                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 9 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex_MAG_FILTER_NEAREST()
{
    shader.use();
    shader.setInt( "task", 4 );

    shader.setMat4( "modelMatrix", glm::translate( glm::mat4( 1.0f ), glm::vec3( 4.0f + 0.3f, 1.0f, 0.0f ) ) );
    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
    shader.setMat4( "projectionMatrix",
                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall_nearest );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 9 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex_MIN_FILTER_LINEAR()
{
    shader.use();
    shader.setInt( "task", 5 );

    glm::mat4 modelMatrix( 1.0f );
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 5.0f + 0.4f, 1.0f, 0.0f ) );
    //modelMatrix = glm::scale( modelMatrix, glm::vec3( 4.0f ) );
    shader.setMat4( "modelMatrix", modelMatrix );

    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );
    shader.setMat4( "projectionMatrix",
                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 15 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex_MIN_FILTER_NEAREST()
{
    shader.use();
    shader.setInt( "task", 6 );

    glm::mat4 modelMatrix( 1.0f );
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 6.0f + 0.5f, 1.0f, 0.0f ) );
    //modelMatrix = glm::scale( modelMatrix, glm::vec3( 4.0f ) );
    shader.setMat4( "modelMatrix", modelMatrix );

    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );

    shader.setMat4( "projectionMatrix",

                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall_nearest );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 15 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}


void drawTex_WRAPING_REPEAT()
{
    shader.use();
    shader.setInt( "task", 6 );

    glm::mat4 modelMatrix( 1.0f );
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 1.0f, -1.0f, 0.0f ) );
    shader.setMat4( "modelMatrix", modelMatrix );

    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );

    shader.setMat4( "projectionMatrix",

                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall_repeat );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 21 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex_WRAPING_MIRRORED_REPEAT()
{
    shader.use();
    shader.setInt( "task", 6 );

    glm::mat4 modelMatrix( 1.0f );
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 2.0f + 0.1f, -1.0f, 0.0f ) );
    shader.setMat4( "modelMatrix", modelMatrix );

    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );

    shader.setMat4( "projectionMatrix",

                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall_mirrored_repeat );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 21 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

void drawTex_WRAPING_CLAMP_TO_EDGE()
{
    shader.use();
    shader.setInt( "task", 6 );

    glm::mat4 modelMatrix( 1.0f );
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 3.0f + 0.2f, -1.0f, 0.0f ) );
    shader.setMat4( "modelMatrix", modelMatrix );

    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );

    shader.setMat4( "projectionMatrix",

                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall_clamp_to_edge );
    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 21 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}


void drawTex_WRAPING_CLAMP_TO_BORDER()
{
    shader.use();
    shader.setInt( "task", 6 );

    glm::mat4 modelMatrix( 1.0f );
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 4.0f + 0.4f, -1.0f, 0.0f ) );
    shader.setMat4( "modelMatrix", modelMatrix );

    shader.setMat4( "viewMatrix", pCamera->getViewMatrix() );

    shader.setMat4( "projectionMatrix",

                    glm::perspective( glm::radians( pCamera->fov() ),
                                      ( float ) ScreenWidth / ScreenHeight,
                                      0.1f, 100.0f ) );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture_wall_clamp_to_border );

    float borderColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

    shader.setInt( "texture0", 0 );

    glBindVertexArray( VAO );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( GLvoid * )( 21 * sizeof( GLuint ) ) );

    glBindVertexArray( 0 );
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
    pCamera->processKeyboard( window, ( float )deltaTime );
}

void renderLoop( GLFWwindow *window, function<void()> renderCallback )
{
    double currentTime;
    while( !glfwWindowShouldClose( window ) )
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

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
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );
}

