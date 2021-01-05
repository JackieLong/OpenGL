#include "project_lib.h"
#include "other.h"

using namespace std;

Camera *pCamera  = nullptr;
double deltaTime = 0.0;         // 当前帧与上一帧的时间差
double lastFrame = 0.0;         // 上一帧的时间

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadMatrixTarget( ShaderProgram &shaderProgram );
void loadMatrixLight( ShaderProgram &shaderProgram );
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos );
void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset );

int main()
{
    GLFWwindow *window = init( mouse_move_callback,                 // 鼠标移动回调
                               mouse_scroll_callback );             // 鼠标滚轮回调

    pCamera = new Camera( glm::vec3( 1.0f, 1.0f, 7.0f ),            // 摄像机的初始位置
                          glm::vec3( 0.5f, 0.5f, 0.0f ),            // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ),            // 世界坐标中的向上向量
                          -90.f,                                    // Yaw
                          0.0f );                                   // Pitch

    GLuint VAO = 0, VBO = 0;                           // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    GLuint VAO1 = 0, VBO1 = 0;                        // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    GLuint VAO2 = 0, VBO2 = 0;
    loadVertexData1( &VAO, &VBO );                       // 加载顶点数据到缓冲区
    loadVertexData2( &VAO1, &VBO1 );
    loadVertexData3( &VAO2, &VBO2 );

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        loadMatrixTarget( shaderProgram );              // 物体和坐标轴的模型视图投影矩阵
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        loadMatrixTarget( shaderProgramCoord );         // 物体和坐标轴的模型视图投影矩阵
        glBindVertexArray( VAO1 );
        glDrawArrays( GL_LINES, 0, 6 );

        loadMatrixLight( shaderProgramLight );          // 光源的模型视图变换矩阵
        glBindVertexArray( VAO2 );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteVertexArrays( 1, &VAO1 );
    glDeleteVertexArrays( 1, &VAO2 );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &VBO1 );
    glDeleteBuffers( 1, &VBO2 );
    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &EBO1 );
    glDeleteBuffers( 1, &EBO2 );
    glDeleteProgram( shaderProgram.id );

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

bool   firstMouse = true;
double lastX      = 0.0;
double lastY      = 0.0;
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos )
{
    if( firstMouse )
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    pCamera->processMouseMovement( float( xpos - lastX ), float( lastY - ypos ) );

    lastX = xpos;
    lastY = ypos;
}

void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset )
{
    pCamera->processMouseScroll( window, xoffset, yoffset );
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

        glfwSwapBuffers( window );  // 双缓冲

        // IO轮询，检查有没有触发什么事件（keys pressed/released, mouse moved etc.）,
        // 并调用对应的回调函数（可以通过回调方法手动设置）,更新窗口状态，
        glfwPollEvents();
    }
}

void loadMatrixTarget( ShaderProgram &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );

    // 视图变换矩阵
    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) pCamera->fov() ),          // 就是FOV，field of view，视角广度
                                         ( float )ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

void loadMatrixLight( ShaderProgram &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。
    modelMatrix = glm::translate( modelMatrix, glm::vec3( 2.0f, 2.0f, 2.0f ) );
    modelMatrix = glm::scale( modelMatrix, glm::vec3( 0.3f ) );

    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) pCamera->fov() ),            // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,     // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}