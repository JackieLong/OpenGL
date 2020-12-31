#include <iostream>
#include <fstream>
#include <functional>
#include "project_lib.h"

void framebuffer_size_callback( GLFWwindow *window, int width, int height );
void initGLFW();
GLFWwindow *createWindow();
bool loadGLFuncPointer();
ShaderProgram buildShaderProgram();
void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );
void buildVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO );

int main()
{
    initGLFW();                                                             // 初始化GLFW

    GLFWwindow *window = createWindow();                                    // 创建窗口
    if( window == NULL )
    {
        return -1;
    }

    loadGLFuncPointer();                                                    // glad: load all OpenGL function pointers

    ShaderProgram shaderProgram = buildShaderProgram();                     // build and compile our shader program

    GLuint VAO = 0, VBO = 0, EBO = 0;
    buildVertexData( &VAO, &VBO, &EBO );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE：线框模式；GL_FILL：填充模式

    renderLoop( window, [&]                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

        shaderProgram.use();                        // 启用着色器程序

        shaderProgram.set4Float( "appColor", 0.0f, ( sin( glfwGetTime() ) / 2.0f ) + 0.5f, 0.0f, 1.0f );

        glBindVertexArray( VAO );
        // glDrawArrays( GL_TRIANGLES, 2, 3 );
        glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );

    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback( GLFWwindow *window, int width, int height )
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport( 0, 0,               // 视口左下角位置
                width, height );    // 视口宽高
}

void initGLFW()
{
    //--------------------------------------------------------- 1、初始化glfw
    glfwInit();                                                                                                                                                       // 初始化GLFW

    //--------------------------------------------------------- 2、配置gflw：运行要求，至少是OpenGL 3.3以上。
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );                    // 主版本号：3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );                    // 次版本号：3
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );    // 使用opengl core profile核心模式，导致必须使用VAO。

    #ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
}

GLFWwindow *createWindow()
{
    GLFWwindow *window = glfwCreateWindow( 800, 600,        // 窗口宽高
                                           "LearnOpenGL",   // 窗口名称
                                           NULL, NULL );
    if( window == NULL )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    else
    {
        glfwMakeContextCurrent( window );                                       // 设置当前线程的上下文为window的上下文。
        glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );    // 窗口大小改变回调
    }

    return window;
}

// glad: load all OpenGL function pointers
bool loadGLFuncPointer()
{
    if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

ShaderProgram buildShaderProgram()
{
    std::string vertexShaderFilePath( "D:/projects/project_LearnOpenGL/LearnOpenGL/LearnOpenGL/src/shader/shader_vertex" );
    std::string fragmentShaderFilePath( "D:/projects/project_LearnOpenGL/LearnOpenGL/LearnOpenGL/src/shader/shader_fragment" );

    return ShaderProgram( vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str() );
}

void renderLoop( GLFWwindow *window, std::function<void()> renderCallback )
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

void buildVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO )
{
    // 3个顶点绘制一个三角形
    float vertices[] =
    {
        // 位置                  // 颜色
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, 1.0f, // 右下
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, // 左下
        0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f, // 顶部
    };

    unsigned int indices[] =    // 三角形的顶点索引
    {
        0, 1, 2,                // 第一个三角形
    };

    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glGenBuffers( 1, EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0,                       // 指定vertex attribute的位置值，这个值对应在顶点着色器中的location=0的属性
                           3,                       // 顶点属性的分量数量（number of component）
                           GL_FLOAT,                // 分量数据类型（data type of component）
                           GL_FALSE,                // 数据是否需要被标准化
                           7 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glVertexAttribPointer( 1,                       // 指定vertex attribute的位置值，这个值对应在顶点着色器中的location=1的属性
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           7 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );

    glEnableVertexAttribArray( 0 );     // 顶点属性配置默认是关闭的，要记得打开，0对应layout(location=0)顶点属性
    glEnableVertexAttribArray( 1 );     //

    glBindVertexArray( 0 );
}