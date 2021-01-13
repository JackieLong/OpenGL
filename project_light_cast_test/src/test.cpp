#include "data.h"

using namespace std;

void initGLState();
void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadMatrixLight( Shader &shaderProgram );
void loadMatrixTarget( Shader &shaderProgram );

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

        loadMatrixLight( shader_light );
        glBindVertexArray( VAO_light );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        loadMatrixTarget( shader_coord );
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

        shader.use();
        // 1：平行光（太阳光）
        // 2：点光源
        // 3：聚光（SpotLight），如手电筒
        shader.setInt( "lightType", 3 );
        shader.setVec3( "viewPos", pCamera->pos() );             // 观察位置
        shader.setInt( "material.diffuse", 0 );
        shader.setInt( "material.specular", 1 );
        shader.setFloat( "material.shininess", 64.0f );

        glm::vec3 lightColor = glm::vec3( 1.0f, 1.0f, 1.0f );           // 光源颜色
        glm::vec3 diffuseColor = lightColor   * glm::vec3( 0.5f );      // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3( 0.2f );      // 很低的影响

        glm::vec3 sunLightDir = glm::vec3( 0.0f, -1.0f, 0.0f );         // 太阳光方向
        shader.setVec3( "sunLight.dir", sunLightDir );
        shader.setVec3( "sunLight.ambient", ambientColor );
        shader.setVec3( "sunLight.diffuse", diffuseColor );         // 将光照调暗了一些以搭配场景
        shader.setVec3( "sunLight.specular", glm::vec3( 1.0f ) );

        // point light 1
        shader.setVec3( "pointLights[0].pos", pointLightPositions[0] );
        shader.setVec3( "pointLights[0].ambient", 0.05f, 0.05f, 0.05f );
        shader.setVec3( "pointLights[0].diffuse", 0.8f, 0.8f, 0.8f );
        shader.setVec3( "pointLights[0].specular", 1.0f, 1.0f, 1.0f );
        shader.setFloat( "pointLights[0].constant", 1.0f );
        shader.setFloat( "pointLights[0].linear", 0.09f );
        shader.setFloat( "pointLights[0].quadratic", 0.032f );
        // point light 2
        shader.setVec3( "pointLights[1].pos", pointLightPositions[1] );
        shader.setVec3( "pointLights[1].ambient", 0.05f, 0.05f, 0.05f );
        shader.setVec3( "pointLights[1].diffuse", 0.8f, 0.8f, 0.8f );
        shader.setVec3( "pointLights[1].specular", 1.0f, 1.0f, 1.0f );
        shader.setFloat( "pointLights[1].constant", 1.0f );
        shader.setFloat( "pointLights[1].linear", 0.09f );
        shader.setFloat( "pointLights[1].quadratic", 0.032f );
        // point light 3
        shader.setVec3( "pointLights[2].pos", pointLightPositions[2] );
        shader.setVec3( "pointLights[2].ambient", 0.05f, 0.05f, 0.05f );
        shader.setVec3( "pointLights[2].diffuse", 0.8f, 0.8f, 0.8f );
        shader.setVec3( "pointLights[2].specular", 1.0f, 1.0f, 1.0f );
        shader.setFloat( "pointLights[2].constant", 1.0f );
        shader.setFloat( "pointLights[2].linear", 0.09f );
        shader.setFloat( "pointLights[2].quadratic", 0.032f );
        // point light 4
        shader.setVec3( "pointLights[3].pos", pointLightPositions[3] );
        shader.setVec3( "pointLights[3].ambient", 0.05f, 0.05f, 0.05f );
        shader.setVec3( "pointLights[3].diffuse", 0.8f, 0.8f, 0.8f );
        shader.setVec3( "pointLights[3].specular", 1.0f, 1.0f, 1.0f );
        shader.setFloat( "pointLights[3].constant", 1.0f );
        shader.setFloat( "pointLights[3].linear", 0.09f );
        shader.setFloat( "pointLights[3].quadratic", 0.032f );


        shader.setVec3( "spotLight.pos", pCamera->pos() );
        shader.setVec3( "spotLight.dir", pCamera->front() );
        shader.setFloat( "spotLight.cutoff", glm::cos( glm::radians( 12.5f ) ) );
        shader.setFloat( "spotLight.cutoff1", glm::cos( glm::radians( 18.0f ) ) );
        shader.setVec3( "spotLight.ambient", ambientColor );
        shader.setVec3( "spotLight.diffuse", diffuseColor );
        shader.setVec3( "spotLight.specular", glm::vec3( 1.0f ) );
        shader.setFloat( "spotLight.constant", 1.0f );
        shader.setFloat( "spotLight.linear", 0.09f );
        shader.setFloat( "spotLight.quadratic", 0.032f );

        loadMatrixTarget( shader );

        glBindVertexArray( VAO_object );
        for( int i = 0; i < 10; i++ )
        {
            glm::mat4 modelMatrix( 1.0f );
            modelMatrix = glm::translate( modelMatrix, cubePositions[i] );
            float angle = 20.0f * i;
            modelMatrix = glm::rotate( modelMatrix, glm::radians( angle ), glm::vec3( 1.0f, 0.3f, 0.5f ) );

            shader.use();
            shader.setMat4( "modelMatrix", modelMatrix );

            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

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

void loadMatrixLight( Shader &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。

    pointLightPos = glm::vec3( 0.0f, 1.0f, 0.0f ) +
                    glm::vec3( 3 * sin( 2.0f * glfwGetTime() ),
                               0,
                               3 * cos( 2.0f * glfwGetTime() ) );
    modelMatrix = glm::translate( modelMatrix, pointLightPos );
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