#include "project_lib.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadVertexData( GLuint *VAO, GLuint *VBO );
void loadTextureData( const Shader & );
void loadMatrix( Shader &shaderProgram );

const int ScreenWidth = 800;
const int ScreenHeight = 600;

int main()
{
    initGLFW();

    GLFWwindow *window = createWindow( ScreenWidth,
                                       ScreenHeight,
                                       "LearnOpenGL" );

    initGLAD();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                      // 开启深度测试，默认是不开启的。

    Shader shaderProgram = Shader( projectDir() + "\\src\\shader\\shader_vertex",     // 顶点着色器脚本
                                   projectDir() + "\\src\\shader\\shader_fragment" ); // 片段着色器脚本

    GLuint VAO = 0, VBO = 0;
    loadVertexData( &VAO, &VBO );            // 加载顶点数据到缓冲区

    GLuint texture = loadTexture( projectDir() + "\\res\\awesomeface.png" );     // 加载镜面反射贴图
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );


    glm::vec3 cubePositions[] =                     // 每个物体模型的位移。
    {
        glm::vec3( 0.0f,  0.0f,  0.0f ),
        glm::vec3( 2.0f,  5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f, -2.5f ),
        glm::vec3( -3.8f, -2.0f, -12.3f ),
        glm::vec3( 2.4f, -0.4f, -3.5f ),
        glm::vec3( -1.7f,  3.0f, -7.5f ),
        glm::vec3( 1.3f, -2.0f, -2.5f ),
        glm::vec3( 1.5f,  2.0f, -2.5f ),
        glm::vec3( 1.5f,  0.2f, -1.5f ),
        glm::vec3( -1.3f,  1.0f, -1.5f )
    };

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        loadMatrix( shaderProgram );                                // 传递变换矩阵给顶点着色器使用。包含模型、视图、投影矩阵

        glBindVertexArray( VAO );

        for( unsigned int i = 0; i < 10; i++ )                      // 绘制10个这样个立方体
        {
            glm::mat4 model( 1.0f );                                // 主要要显式初始化为单位矩阵
            model = glm::translate( model, cubePositions[i] );      // 每个立方体平移不同位移（模型变换矩阵完成）
            model = glm::rotate( model,                             // 旋转变换
                                 glm::radians( i % 3 == 0 ? ( float )glfwGetTime() * 25.0f : ( 20.0f * i ) ),   // 旋转角度，OpenGL是右手坐标系
                                 glm::vec3( 0.0f, 0.0f, 1.0f ) );   // 旋转轴
            shaderProgram.use();
            shaderProgram.setMat4( "modelMatrix", glm::value_ptr( model ) );    // 绘制每个立方体，都使用各自的模型变换矩阵。

            glDrawArrays( GL_TRIANGLES, 0, 36 );                // 绘制立方体，触发管线流程
        }

        glBindVertexArray( 0 );
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
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

void loadVertexData( GLuint *VAO, GLuint *VBO )
{
    // 6个顶点（6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点）
    // 这里没有使用索引缓冲对象
    float vertices[] =
    {
        // 顶点坐标          // 纹理坐标
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f
    };

    loadVertexData( vertices, sizeof( vertices ), "32", VAO, VBO );
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
    viewMatrix = glm::translate( viewMatrix,
                                 glm::vec3( 0.0f, 0.0f, -3.0f ) );      //将场景往Z轴负方向移动。

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( 45.0f ),          // 就是FOV，field of view，视角广度
                                         ( float )ScreenWidth / ScreenHeight,   // aspectRatio，宽高比
                                         0.1f,                            // near plane，近平面位置
                                         100.0f );                       // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}
