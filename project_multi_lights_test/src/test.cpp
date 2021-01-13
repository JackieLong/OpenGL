#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );

void loadMatrixLight( Shader &shaderProgram );
void loadMatrixTarget( Shader &shaderProgram );
void loadVertexDataObject( GLuint *VAO, GLuint *VBO );

int main()
{
    initGLFW();                                                     // 初始化GLFW

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

    loadData();

    glEnable( GL_DEPTH_TEST );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, sampler_diffuse );
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, sampler_specular );

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        loadMatrixLight( shaderLight );
        glBindVertexArray( VAO_light );
        for( int i = 0; i < 4; i++ )
        {
            shaderLight.use();
            glm::mat4 modelMatrix( 1.0f );
            modelMatrix = glm::translate( modelMatrix, pointLightPositions[i] );
            modelMatrix = glm::scale( modelMatrix, glm::vec3( 0.3f ) );
            shaderLight.setMat4( "modelMatrix", modelMatrix );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        loadMatrixTarget( shaderCoord );
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

        shader.setVec3( "sunLight.dir", glm::vec3( 0.0f, -1.0f, 0.0f ) );
        shader.setVec3( "sunLight.ambient", glm::vec3( 0.1f ) );
        shader.setVec3( "sunLight.diffuse", glm::vec3( 0.5f ) );        // 将光照调暗了一些以搭配场景
        shader.setVec3( "sunLight.specular", glm::vec3( 1.0f ) );

        // point light 1
        glm::vec3 ambient( 0.05f, 0.05f, 0.05f );
        glm::vec3 diffuse( 0.1f, 0.8f, 0.1f );
        glm::vec3 specular( 1.0f, 1.0f, 1.0f );
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        ostringstream ostrstream;
        for( int i = 0; i < 4; i++ )
        {
            ostrstream << "pointLights[" << i << "].pos";
            shader.setVec3( ostrstream.str(), pointLightPositions[i] );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].ambient";
            shader.setVec3( ostrstream.str(), ambient );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].diffuse";
            shader.setVec3( ostrstream.str(), diffuse );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].specular";
            shader.setVec3( ostrstream.str(), specular );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].constant";
            shader.setFloat( ostrstream.str(), constant );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].linear";
            shader.setFloat( ostrstream.str(), linear );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].quadratic";
            shader.setFloat( ostrstream.str(), quadratic );
            ostrstream.str( "" );
        }

        shader.setVec3( "spotLight.pos", pCamera->pos() );
        shader.setVec3( "spotLight.dir", pCamera->front() );

        shader.setVec3( "spotLight.ambient", ambient );
        shader.setVec3( "spotLight.diffuse", diffuse );
        shader.setVec3( "spotLight.specular", specular );

        shader.setFloat( "spotLight.constant", 1.0f );
        shader.setFloat( "spotLight.linear", 0.09f );
        shader.setFloat( "spotLight.quadratic", 0.032f );

        shader.setFloat( "spotLight.cutoff", glm::cos( glm::radians( 12.5f ) ) );
        shader.setFloat( "spotLight.cutoff1", glm::cos( glm::radians( 18.0f ) ) );

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
                                         ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
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