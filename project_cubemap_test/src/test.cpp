#include "project_lib.h"
#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void processInput( GLFWwindow *window );
void initGLState();

void testSkybox();
void testReflection_cube();
void testReflection_model( Model &model );
void testRefraction_model( Model &model );

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

    Model model( projectDir() + "/res/nanosuit/nanosuit.obj" );

    renderLoop( window, [&]                                     // 渲染循环
    {
        viewMatrix = pCamera->getViewMatrix();

        projectionMatrix = glm::perspective( glm::radians( pCamera->fov() ),         // FOV，field of view，视角广度
                                             ( float ) ScreenWidth / ScreenHeight,   // aspectRatio，宽高比
                                             0.1f,                                   // near plane，近平面位置
                                             100.0f );                               // far plane，远平面位置

        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                 // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   // 状态使用函数：清空颜色缓冲区和深度缓冲区

        //testSkybox();                 // 立方体贴图实践应用：天空盒
        //testReflection_cube();        // 立方体贴图实践应用：反射
        //testReflection_model( model );  // 立方体贴图实践应用：反射模型
        testRefraction_model( model );  //  立方体贴图实践应用：折射
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO_coord );
    glDeleteVertexArrays( 1, &VAO_cube );
    glDeleteVertexArrays( 1, &VAO_skybox );

    glDeleteBuffers( 1, &VBO_coord );
    glDeleteBuffers( 1, &VBO_cube );
    glDeleteBuffers( 1, &VBO_skybox );

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

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
    //glDepthFunc( GL_ALWAYS );                           // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
}

void testSkybox()
{
    // 如果在最开始绘制天空盒，则屏幕每个像素都将运行一遍着色器，我们可以在没有可见物体的时候再绘制天空盒，提升性能。
    // 优化方法在下面
    //shader.setInt( "task", TASK_SKY );
    //shader.setInt( "textureCube0", 0 );
    //shader.setMat4( "viewMatrix", glm::mat4( glm::mat3( viewMatrix ) ) );
    //shader.setMat4( "projectionMatrix", projectionMatrix );
    //glBindVertexArray( VAO_skybox );
    //glBindTexture( GL_TEXTURE_CUBE_MAP, textureSkybox );
    //glDrawArrays( GL_TRIANGLES, 0, 36 );

    shader.use();
    shader.setInt( "task", TASK_COORD );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_CUBE );
    shader.setInt( "texture0", 0 );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_cube );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textureCube );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    // 让天空盒的深度值始终为最大值1.0，深度测试为小于等于当前缓冲深度值才能通过测试
    // 意味着只要绘制了物体的地方就，天空盒片段的深度测试就不会通过。
    glDepthFunc( GL_LEQUAL );
    shader.setInt( "task", TASK_SKY );
    shader.setInt( "textureCube0", 0 );
    shader.setMat4( "viewMatrix", glm::mat4( glm::mat3( viewMatrix ) ) );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_skybox );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureSkybox );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glDepthFunc( GL_LESS );
}

void testReflection_cube()
{
    shader.use();
    shader.setInt( "task", TASK_COORD );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_REFLECT );
    shader.setInt( "texture0", 0 );
    shader.setVec3( "viewPos", pCamera->pos() );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_cube );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textureCube );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    // 让天空盒的深度值始终为最大值1.0，深度测试为小于等于当前缓冲深度值才能通过测试
    // 意味着只要绘制了物体的地方就，天空盒片段的深度测试就不会通过。
    glDepthFunc( GL_LEQUAL );
    shader.setInt( "task", TASK_SKY );
    shader.setInt( "textureCube0", 0 );
    shader.setMat4( "viewMatrix", glm::mat4( glm::mat3( viewMatrix ) ) );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_skybox );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureSkybox );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glDepthFunc( GL_LESS );
}

void testReflection_model( Model &model )
{
    shader.use();
    shader.setInt( "task", TASK_COORD );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_REFLECT_MODEL );
    shader.setVec3( "viewPos", pCamera->pos() );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    model.draw( shader );

    // 让天空盒的深度值始终为最大值1.0，深度测试为小于等于当前缓冲深度值才能通过测试
    // 意味着只要绘制了物体的地方就，天空盒片段的深度测试就不会通过。
    glDepthFunc( GL_LEQUAL );
    shader.setInt( "task", TASK_SKY );
    shader.setInt( "textureCube0", 0 );
    shader.setMat4( "viewMatrix", glm::mat4( glm::mat3( viewMatrix ) ) );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_skybox );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureSkybox );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glDepthFunc( GL_LESS );
}

void testRefraction_model( Model &model )
{
    shader.use();
    shader.setInt( "task", TASK_COORD );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );

    shader.setInt( "task", TASK_REFRACT_MODEL );
    shader.setVec3( "viewPos", pCamera->pos() );
    shader.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    model.draw( shader );

    // 让天空盒的深度值始终为最大值1.0，深度测试为小于等于当前缓冲深度值才能通过测试
    // 意味着只要绘制了物体的地方就，天空盒片段的深度测试就不会通过。
    glDepthFunc( GL_LEQUAL );
    shader.setInt( "task", TASK_SKY );
    shader.setInt( "textureCube0", 0 );
    shader.setMat4( "viewMatrix", glm::mat4( glm::mat3( viewMatrix ) ) );
    shader.setMat4( "projectionMatrix", projectionMatrix );
    glBindVertexArray( VAO_skybox );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureSkybox );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glDepthFunc( GL_LESS );
}


