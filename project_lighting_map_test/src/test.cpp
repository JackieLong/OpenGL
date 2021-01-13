#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void initGLState();
void loadMatrixTarget( Shader &shaderProgram );
void loadMatrixLight( Shader &shaderProgram );

int main()
{
    initGLFW();

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

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        shaderLight.use();
        shaderLight.setVec3( "lightColor", lightColor );
        loadMatrixLight( shaderLight );
        glBindVertexArray( VAO_light );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        shaderCoord.use();
        loadMatrixTarget( shaderCoord );
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

        shader.use();
        shader.setVec3( "viewPos", pCamera->pos() );             // 观察位置
        shader.setInt( "material.diffuse", 0 );
        shader.setInt( "material.specular", 1 );
        //shaderProgram.setVec3( "material.specular", 0.9f, 0.9f, 0.9f );
        shader.setInt( "material.glow", 2 );
        shader.setFloat( "material.shininess", 64.0f );

        glm::vec3 diffuseColor = lightColor   * glm::vec3( 0.5f );      // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3( 0.2f );      // 很低的影响

        shader.setVec3( "light.pos", lightPos );
        shader.setVec3( "light.ambient", ambientColor );
        shader.setVec3( "light.diffuse", diffuseColor );         // 将光照调暗了一些以搭配场景
        shader.setVec3( "light.specular", 1.0f, 1.0f, 1.0f );

        loadMatrixTarget( shader );
        glBindVertexArray( VAO_object );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO_object );
    glDeleteVertexArrays( 1, &VAO_coord );
    glDeleteVertexArrays( 1, &VAO_light );
    glDeleteBuffers( 1, &VBO_object );
    glDeleteBuffers( 1, &VBO_coord );
    glDeleteBuffers( 1, &VBO_light );
    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void loadMatrixTarget( Shader &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );

    // 视图变换矩阵
    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) pCamera->fov() ), // 就是FOV，field of view，视角广度
                                         ( float )ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

void loadMatrixLight( Shader &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。

    lightPos = glm::vec3( 0.0f, 1.0f, 0.0f ) +
               glm::vec3( 2 * sin( 1.5f * glfwGetTime() ),
                          0,
                          2 * cos( 1.5f * glfwGetTime() ) );
    modelMatrix = glm::translate( modelMatrix, lightPos );
    modelMatrix = glm::scale( modelMatrix, glm::vec3( 0.3f ) );

    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) pCamera->fov() ), // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,     // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
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

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
}