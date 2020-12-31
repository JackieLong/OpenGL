#include <iostream>
#include <functional>
#include "project_lib.h"

void framebuffer_size_callback( GLFWwindow *window, int width, int height );
void initGLFW();
GLFWwindow *createWindow();
bool loadGLFuncPointer();
int buildShaderProgram();
int createAndCompileShader( int shaderFlag, const char *shaderSrc );
void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );
void setupData_VBO( GLuint *VAO, GLuint *VBO );
void setupData_EBO( GLuint *VAO, GLuint *VBO, GLuint *EBO );

int main()
{
    initGLFW();                                                             // 初始化GLFW

    GLFWwindow *window = createWindow();                                    // 创建窗口
    if( window == NULL )
    {
        return -1;
    }

    loadGLFuncPointer();                                                    // glad: load all OpenGL function pointers

    int shaderProgram = buildShaderProgram();                               // build and compile our shader program

    enum DATA_MODE
    {
        MODE_VBO,
        MODE_EBO,
    };
    int mode = MODE_VBO;
    GLuint VAO = 0, VBO = 0, EBO = 0;
    switch( mode )
    {
        case MODE_VBO:
            setupData_VBO( &VAO, &VBO );
            break;
        case MODE_EBO:
            setupData_EBO( &VAO, &VBO, &EBO );
            break;
        default:
            break;
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    // GL_LINE：线框模式；GL_FILL：填充模式

    renderLoop( window, [&]                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

        glUseProgram( shaderProgram );              // 启用着色器程序

        switch( mode )
        {
            case MODE_VBO:                          // 通过VBO绘制图形
            {
                glBindVertexArray( VAO );           // 因为开启了core profile模式，所以必须使用VAO
                glDrawArrays( GL_TRIANGLES, 0, 6 );
                glBindVertexArray( 0 );
                break;
            }
            case MODE_EBO:                          // 通过EBO绘制图形
            {
                glBindVertexArray( VAO );
                // glDrawArrays( GL_TRIANGLES, 2, 3 );
                glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
                glBindVertexArray( 0 );
                break;
            }
            default:
                break;
        }
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    glDeleteProgram( shaderProgram );

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

int buildShaderProgram()
{
    const char *vertexShaderSrc =
                    "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "void main()\n"
                    "{\n"
                    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                    "}\0";

    const char *fragmentShaderSrc =
                    "#version 330 core\n"
                    "out vec4 FragColor;\n"
                    "void main()\n"
                    "{\n"
                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                    "}\n\0";

    int vertexShader = createAndCompileShader( GL_VERTEX_SHADER, vertexShaderSrc );
    int fragmentShader = createAndCompileShader( GL_FRAGMENT_SHADER, fragmentShaderSrc );

    int shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );

    int success;                                                    // 编译成功与否
    const int lenLog = 512;                                         // 日志长度
    char infoLog[lenLog];                                           // 编译输出日志
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );      // check for linking errors
    if( !success )
    {
        glGetProgramInfoLog( shaderProgram, lenLog, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return shaderProgram;
}

// build and compile our shader program
int createAndCompileShader( int shaderFlag, const char *shaderSrc )
{
    int shader = glCreateShader( shaderFlag );                      // 常见shader实例
    glShaderSource( shader, 1, &shaderSrc, NULL );                  // 加载shader源码

    glCompileShader( shader );                                      // 编译shader

    int success;                                                    // 编译成功与否
    const int lenLog = 512;                                         // 日志长度
    char infoLog[lenLog];                                           // 编译输出日志
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );           // 获取编译状态数据
    if( !success )
    {
        glGetShaderInfoLog( shader, lenLog, NULL, infoLog );        // 编译没有成功，获取编译日志
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
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

void setupData_VBO( GLuint *VAO, GLuint *VBO )
{
    // 6个顶点绘制两个三角形组成矩形
    float vertices[] =
    {
        0.5f,  0.5f, 0.0f,          // 右上角
        0.5f, -0.5f, 0.0f,          // 右下角
        -0.5f,  0.5f, 0.0f,         // 左上角

        -0.5f,  0.5f, 0.0f,         // 左上角
        -0.5f, -0.5f, 0.0f,         // 左下角
        0.5f, -0.5f, 0.0f,          // 右下角
    };

    glGenVertexArrays( 1, VAO );    // 创建VAO
    glBindVertexArray( *VAO );      // 绑定到当前VAO，记录后续的状态设置

    glGenBuffers( 1, VBO );                 // 创建一个缓冲对象
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );  // 绑定到当前GL_ARRAY_BUFFER类型的缓冲，即VBO
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );  // 填充数据

    glVertexAttribPointer( 0,                       // 指定vertex attribute的位置值，这个值对应在顶点着色器中的location=0的属性
                           3,                       // 顶点属性的分量数量（number of component）
                           GL_FLOAT,                // 分量数据类型（data type of component）
                           GL_FALSE,                // 数据是否需要被标准化
                           3 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glEnableVertexAttribArray( 0 );                 // 恢复至默认状态
    glBindVertexArray( 0 );                         // 恢复至默认状态，只要有VAO就能快速恢复。
}

void setupData_EBO( GLuint *VAO, GLuint *VBO, GLuint *EBO )
{
    // 4个顶点绘制两个三角形组成矩形
    float vertices[] =
    {
        0.5f,  0.5f, 0.0f,      // 右上角
        0.5f, -0.5f, 0.0f,      // 右下角
        -0.5f, -0.5f, 0.0f,     // 左下角
        -0.5f,  0.5f, 0.0f      // 左上角
    };

    unsigned int indices[] =    // 三角形的顶点索引
    {
        0, 1, 3,                // 第一个三角形
        1, 2, 3                 // 第二个三角形
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
                           3 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glEnableVertexAttribArray( 0 );

    glBindVertexArray( 0 );
}