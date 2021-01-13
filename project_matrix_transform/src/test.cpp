#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadMatrix1( Shader &shaderProgram );
void loadMatrix2( Shader &shaderProgram );
void loadMatrix3( Shader &shaderProgram );

int main()
{
    initGLFW();                                         // 初始化GLFW


    GLFWwindow *window = createWindow( 800, 600,        // 窗口宽高
                                       "LearnOpenGL" ); // 创建窗口

    initGLAD();

    loadData();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE：线框模式；GL_FILL：填充模式

    renderLoop( window, [&]                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        shader.use();
        shader.setInt( "texture0", 0 );

        loadMatrix1( shader );
        //loadMatrix2( shader );
        //loadMatrix3( shader );

        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );  // 根据索引缓冲中的6个元素，绘制三角形，刚好得到2个三角形
        glBindVertexArray( 0 );
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
}

void renderLoop( GLFWwindow *window, function<void()> renderCallback )
{
    while( !glfwWindowShouldClose( window ) )
    {
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

void loadMatrix1( Shader &shaderProgram )
{
    // glm 0.9.9版本以前：默认初始化一个单位矩阵
    // glm 0.9.9版本之后：默认初始化一个零矩阵
    glm::mat4 trans( 1.0f );                                     // 初始化一个单位矩阵

    trans = glm::rotate( trans,                                  // 生成一个旋转的变换矩阵，旋转效果如下：
                         glm::radians( 90.0f ),                  // 逆时针旋转90度
                         glm::vec3( 0.0, 0.0, 1.0 ) );           // 沿z轴旋转

    trans = glm::scale( trans, glm::vec3( 0.5, 0.5, 0.5 ) );     // 在原矩阵基础上再加一个缩放变换，沿x、y、z轴缩小0.5倍。

    shaderProgram.use();
    shaderProgram.setMat4( "transform", glm::value_ptr( trans ) );
}

void loadMatrix2( Shader &shaderProgram )
{
    glm::mat4 trans( 1.0f );
    trans = glm::translate( trans, glm::vec3( 0.5f, -0.5f, 0.0f ) );    // 先平移
    trans = glm::rotate( trans,                                         // 后旋转，旋转角度随时间变化
                         ( float ) glfwGetTime(),
                         glm::vec3( 1.0f, 1.0f, 1.0f ) );
    shaderProgram.use();
    shaderProgram.setMat4( "transform", glm::value_ptr( trans ) );
}

void loadMatrix3( Shader &shaderProgram )
{
    glm::mat4 trans( 1.0f );
    trans = glm::rotate( trans,                                         // 旋转平移顺序调换，效果完全变化，提示：旋转后的x、y、z轴发生变化
                         ( float ) glfwGetTime(),
                         glm::vec3( 0.0f, 0.0f, 1.0f ) );
    trans = glm::translate( trans, glm::vec3( 0.5f, -0.5f, 0.0f ) );
    shaderProgram.use();
    shaderProgram.setMat4( "transform", glm::value_ptr( trans ) );
}
