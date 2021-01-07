/****************************************************************************

    本文件只是为了将无关摄像机知识点的代码抽离出来

****************************************************************************/
#ifndef __OTHER_H_CAMERA__
#define __OTHER_H_CAMERA__

#include "project_lib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"              // 图片加载库，支持大部分常见格式

using namespace std;

const int ScreenWidth  = 800;
const int ScreenHeight = 600;

Camera *fuckCamera     = nullptr;
double deltaTime       = 0.0;         // 当前帧与上一帧的时间差
double lastFrame       = 0.0;         // 上一帧的时间

ShaderProgram shaderProgram, shaderProgramCoord, shaderProgramLight;

GLuint VAO = 0, VAO1 = 0, VAO2 = 0;           // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
GLuint VBO = 0, VBO1 = 0, VBO2 = 0;
GLuint EBO = 0, EBO1 = 0, EBO2 = 0;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos );
void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset );

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

// glad: load all OpenGL function pointers
bool loadGLFuncPointer()
{
    if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
    {
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }
    return true;
}

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式
    glEnable( GL_DEPTH_TEST );                          // 开启深度测试，默认是不开启的。
}

void loadVertexDataCoord( GLuint *VAO, GLuint *VBO )
{
    float vertices[] =          // 绘制坐标系的顶点
    {
        // 顶点坐标
        -10000.0f, 0.0f, 0.0f,      // X轴上顶点位置1
        10000.0f, 0.0f, 0.0f,       // X轴上顶点位置2

        0.0f, 10000.0f, 0.0f,       // Y轴上顶点位置1
        0.0f, -10000.0f, 0.0f,      // Y轴上顶点位置2

        0.0f, 0.0f, 10000.0f,       // Z轴上顶点位置1
        0.0f, 0.0f, -10000.0f,      // Z轴上顶点位置2
    };

    // OpenGL规定：顶点坐标（0.0，0.0）在中心点，而纹理坐标（0.0，0.0）在左下角
    // 在常见图片格式中，（0.0，0.0）可能是在图片左上角，因此可能导致图片倒转。

    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0,                       // 顶点属性（3D坐标）位置值，指定vertex attribute的位置值，这个值对应在顶点着色器中的location=0的属性
                           3,                       // 顶点属性的分量数量（number of component）
                           GL_FLOAT,                // 分量数据类型（data type of component）
                           GL_FALSE,                // 数据是否需要被标准化
                           3 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glEnableVertexAttribArray( 0 );                 // 顶点属性配置默认是关闭的，要记得打开，0对应layout(location=0)顶点属性

    glBindVertexArray( 0 );
}

void loadVertexDataLight( GLuint *VAO, GLuint *VBO )
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
        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,

    };
    // OpenGL规定：顶点坐标（0.0，0.0）在中心点，而纹理坐标（0.0，0.0）在左下角
    // 在常见图片格式中，（0.0，0.0）可能是在图片左上角，因此可能导致图片倒转。

    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0,                       // 顶点属性（3D坐标）位置值，指定vertex attribute的位置值，这个值对应在顶点着色器中的location=0的属性
                           3,                       // 顶点属性的分量数量（number of component）
                           GL_FLOAT,                // 分量数据类型（data type of component）
                           GL_FALSE,                // 数据是否需要被标准化
                           5 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glVertexAttribPointer( 1,                       // 顶点属性（纹理坐标）位置值
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           5 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );

    glEnableVertexAttribArray( 0 );     // 顶点属性配置默认是关闭的，要记得打开，0对应layout(location=0)顶点属性
    glEnableVertexAttribArray( 1 );     //

    glBindVertexArray( 0 );
}

GLFWwindow *init( Camera *camera )
{
    fuckCamera = camera;
    initGLFW();                                                     // 初始化GLFW

    GLFWwindow *window = glfwCreateWindow( ScreenWidth, ScreenHeight,        // 窗口宽高
                                           "LearnOpenGL",   // 窗口名称
                                           NULL, NULL );
    if( window == NULL )
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    else
    {
        glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );          // 隐藏光标，并捕捉。
        glfwMakeContextCurrent( window );                                       // 设置当前线程的上下文为window的上下文。
        glfwSetCursorPosCallback( window, mouse_move_callback );                     // 鼠标移动触发回调
        glfwSetScrollCallback( window, mouse_scroll_callback );                       // 鼠标滚轮回调

        // glfw: whenever the window size changed (by OS or user resize) this callback function executes
        // ---------------------------------------------------------------------------------------------
        glfwSetFramebufferSizeCallback( window, []( GLFWwindow * window, int width, int height )
        {
            // make sure the viewport matches the new window dimensions; note that width and
            // height will be significantly larger than specified on retina displays.
            glViewport( 0, 0,               // 视口左下角位置
                        width, height );    // 视口宽高
        } );    // 窗口大小改变回调

    }

    loadGLFuncPointer();                                            // glad: load all OpenGL function pointers

    initGLState();

    // 笑脸立方体着色器
    shaderProgram = loadShaderProgram( projectDir() + "\\src\\shader\\shader_vertex",
                                       projectDir() + "\\src\\shader\\shader_fragment" );

    // 坐标轴着色器
    shaderProgramCoord = loadShaderProgram( projectDir() + "\\src\\shader\\shader_vertex_coord",
                                            projectDir() + "\\src\\shader\\shader_fragment_coord" );

    shaderProgramLight = loadShaderProgram( projectDir() + "\\src\\shader\\shader_vertex_light",
                                            projectDir() + "\\src\\shader\\shader_fragment_light" );

    return window;
}

bool   firstMouse = true;
double lastX = 0.0;
double lastY = 0.0;
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos )
{
    if( firstMouse )
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    fuckCamera->processMouseMovement( float( xpos - lastX ), float( lastY - ypos ) );

    lastX = xpos;
    lastY = ypos;
}

void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset )
{
    fuckCamera->processMouseScroll( window, xoffset, yoffset );
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }

    fuckCamera->processKeyboard( window, ( float ) deltaTime );
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

#endif