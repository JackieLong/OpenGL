#include "project_lib.h"
#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadMatrix( Shader &shaderProgram );
void loadMatrixCoord( Shader &shaderProgram );
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos );
void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset );

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

        loadMatrix( shader );                                // 传递变换矩阵给顶点着色器使用。包含模型、视图、投影矩阵
        glBindVertexArray( VAO_object );
        for( unsigned int i = 0; i < 10; i++ )                      // 绘制10个这样个立方体
        {
            glm::mat4 model( 1.0f );                                // 主要要显式初始化为单位矩阵
            model = glm::translate( model, cubePositions[i] );      // 每个立方体平移不同位移（模型变换矩阵完成）
            shader.use();
            shader.setMat4( "modelMatrix", glm::value_ptr( model ) );    // 绘制每个立方体，都使用各自的模型变换矩阵。

            glDrawArrays( GL_TRIANGLES, 0, 36 );                                // 绘制立方体，触发管线流程
        }

        loadMatrixCoord( shader_coord );
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO_object );
    glDeleteVertexArrays( 1, &VAO_coord );
    glDeleteBuffers( 1, &VBO_object );
    glDeleteBuffers( 1, &VBO_coord );
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

void loadMatrix( Shader &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );

    // OpenGL是右手坐标系，此旋转效果是：从X轴正方向朝负方向看，逆时针旋转。
    modelMatrix = glm::rotate( modelMatrix,                                         // 旋转变换
                               ( float ) glfwGetTime() * glm::radians( 50.0f ),     // 旋转角度，正负号是方向
                               glm::vec3( 1.0f, 0.0f, 0.0f ) );                     // 旋转轴：X轴，是一个向量

    // 视图变换矩阵
    glm::mat4 viewMatrix( 1.0f );
    viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( pCamera->fov() ),            // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                         0.1f,                                      // near plane，近平面位置
                                         100.0f );                                  // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

void loadMatrixCoord( Shader &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。

    glm::mat4 viewMatrix( 1.0f );
    viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( pCamera->fov() ),            // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                         0.1f,                                      // near plane，近平面位置
                                         100.0f );                                  // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}