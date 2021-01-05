#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <string>

#include "project_lib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"              // 图片加载库，支持大部分常见格式

using namespace std;

enum CameraView         // 摄像机视角
{
    FIX = 0,                // 一个固定视角
    ROTATE_Y,               // 绕Y轴旋转视角
    FREE,                   // 可自由移动/旋转的视角
    FREE_LOOKAT,            // 手动创建LookAt矩阵
    FIX_XZ
};

const int ScreenWidth  = 800;
const int ScreenHeight = 600;

glm::vec3 cameraPosStart = glm::vec3( 0.0f, 0.0f, 3.0f );    // 摄像机的初始位置
glm::vec3 cameraPos      = cameraPosStart;                   // 摄像机的位置
glm::vec3 cameraTarget   = glm::vec3( 0.0f, 0.0f, 0.0f );    // 摄像机注视点

glm::vec3 cameraFront    = glm::vec3( 0.0f, 0.0f, -1.0f );   // 摄像机的朝向，基向量，其实就是z轴基向量的反方向，一直在动态计算

glm::vec3 upVecWorld     = glm::vec3( 0.0f, 1.0f, 0.0f );    // 世界坐标中的向上向量

double    deltaTime      = 0.0;                              // 当前帧与上一帧的时间差
double    lastFrame      = 0.0;                              // 上一帧的时间
double    fov            = 45.0;                             // 视角广度，field of view
bool      firstMouse     = true;

double    lastX          = ScreenWidth / 2;
double    lastY          = ScreenHeight / 2;

double    yaw            = 0.0;
double    pitch          = 0.0;

CameraView cameraView;

void framebuffer_size_callback( GLFWwindow *window, int width, int height );
void mouse_callback( GLFWwindow *window, double xpos, double ypos );
void scroll_callback( GLFWwindow *window, double xoffset, double yoffset );

void initGLFW();
GLFWwindow *createWindow();
bool loadGLFuncPointer();
void initGLState();
ShaderProgram loadShaderProgram();
void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadVertexData1( GLuint *VAO, GLuint *VBO, GLuint *EBO );
void loadVertexData2( GLuint *VAO, GLuint *VBO, GLuint *EBO );
void loadTextureData( const ShaderProgram & );
void loadMatrix1( ShaderProgram &shaderProgram );
void loadMatrix2( ShaderProgram &shaderProgram );
glm::mat4 createViewMatrix( const CameraView &view = CameraView::FREE_LOOKAT );

int main()
{
    initGLFW();                                                 // 初始化GLFW

    GLFWwindow *window = createWindow();                        // 创建窗口
    if( window == NULL )
    {
        return -1;
    }

    loadGLFuncPointer();                                        // glad: load all OpenGL function pointers

    initGLState();

    // build and compile our shader program
    ShaderProgram shaderProgram = loadShaderProgram( projectDir() + "\\src\\shader\\shader_vertex",
                                  projectDir() + "\\src\\shader\\shader_fragment" );

    ShaderProgram shaderProgramCoord = loadShaderProgram( projectDir() + "\\src\\shader\\shader_vertex_coord",
                                       projectDir() + "\\src\\shader\\shader_fragment_coord" );

    GLuint VAO = 0, VBO = 0, EBO = 0;                           // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    GLuint VAO1 = 0, VBO1 = 0, EBO1 = 0;                        // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    loadVertexData1( &VAO, &VBO, &EBO );                        // 加载顶点数据到缓冲区
    loadVertexData2( &VAO1, &VBO1, &EBO1 );

    loadTextureData( shaderProgram );                           // 加载纹理数据到显存中

    glm::vec3 cubePositions[] =                                 // 每个物体模型的位移。
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

        loadMatrix1( shaderProgram );                               // 传递变换矩阵给顶点着色器使用。包含模型、视图、投影矩阵

        glBindVertexArray( VAO );

        for( unsigned int i = 0; i < 10; i++ )                      // 绘制10个这样个立方体
        {
            glm::mat4 model( 1.0f );                                // 主要要显式初始化为单位矩阵
            model = glm::translate( model, cubePositions[i] );      // 每个立方体平移不同位移（模型变换矩阵完成）
            model = glm::rotate( model,                             // 旋转变换
                                 glm::radians( i % 3 == 0 ? ( float ) glfwGetTime() * 25.0f : ( 20.0f * i ) ),   // 旋转角度，OpenGL是右手坐标系
                                 glm::vec3( 0.0f, 0.0f, 1.0f ) );   // 旋转轴
            shaderProgram.use();
            shaderProgram.setMat4( "modelMatrix", glm::value_ptr( model ) );    // 绘制每个立方体，都使用各自的模型变换矩阵。

            glDrawArrays( GL_TRIANGLES, 0, 36 );                // 绘制立方体，触发管线流程
        }

        loadMatrix2( shaderProgramCoord );
        glBindVertexArray( VAO1 );

        shaderProgramCoord.use();

        glDrawArrays( GL_LINES, 0, 6 );

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

    if( cameraView == CameraView::FIX ) { return; }

    // 本函数是每帧调用，不同设备上，帧率不一样，如果cameraSpeed=一个常量，则不同设备上的摄像机移动速度会差异很大
    // 将cameraSpeed与帧率间隔deltaTime线性关联，可以有效确保不同帧率下的摄像机移动速度差不多。

    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    double cameraSpeed = 2.5f * deltaTime;


    if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )    // 摄像机向正前方移动
    {
        cameraPos += ( float ) cameraSpeed * cameraFront;   // 位置 + 方向向量*速度，方向向量必须是单位向量，否则不是匀速
    }
    if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )    // 摄像机向正后方移动
    {
        cameraPos -= ( float ) cameraSpeed * cameraFront;   // 位置 - 方向向量*速度，方向向量必须是单位向量，否则不是匀速
    }
    if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )    // 摄像机向正左方移动
    {
        // 第一步：先通过z、y轴向量叉乘得到x轴向量（注意标准化，否则不是匀速）
        // 第二步：同上理，位置 - 摄像机z轴方向向量*速度
        cameraPos -= glm::normalize( glm::cross( cameraFront, upVecWorld ) ) * ( float ) cameraSpeed;
    }
    if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )    // 摄像机向正右方移动
    {
        // 同上理
        cameraPos += glm::normalize( glm::cross( cameraFront, upVecWorld ) ) * ( float )cameraSpeed;
    }
    if( cameraView == CameraView::FIX_XZ )
    {
        cameraPos.y = cameraPosStart.y;
    }
}

void mouse_callback( GLFWwindow *window, double xpos, double ypos )
{
    if( cameraView != CameraView::FREE && cameraView != CameraView::FIX_XZ && cameraView != CameraView::FREE_LOOKAT )
    {
        return;
    }

    if( firstMouse )
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    double sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if( pitch > 89.0f )
    {
        pitch = 89.0f;
    }
    if( pitch < -89.0f )
    {
        pitch = -89.0f;
    }

    cameraFront = glm::normalize( glm::vec3(
            cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ),
            sin( glm::radians( pitch ) ),
            sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ) ) );
}

void scroll_callback( GLFWwindow *window, double xoffset, double yoffset )
{
    if( fov >= 1.0f && fov <= 45.0f )
    {
        fov -= yoffset;
    }
    if( fov <= 1.0f )
    {
        fov = 1.0f;
    }
    if( fov >= 45.0f )
    {
        fov = 45.0f;
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
        glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );    // 窗口大小改变回调
        glfwSetCursorPosCallback( window, mouse_callback );                     // 鼠标移动触发回调
        glfwSetScrollCallback( window, scroll_callback );                       // 鼠标滚轮回调
    }

    return window;
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
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );                // GL_LINE：线框模式；GL_FILL：填充模式

    glEnable( GL_DEPTH_TEST );                                  // 开启深度测试，默认是不开启的。
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

void loadVertexData1( GLuint *VAO, GLuint *VBO, GLuint *EBO )
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

void loadVertexData2( GLuint *VAO, GLuint *VBO, GLuint *EBO )
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

// 构建第一张图的纹理
void loadTextureData( const ShaderProgram &shaderProgram )
{
    string projDir = projectDir();

    GLuint texture;
    glGenTextures( 1, &texture );

    glActiveTexture( GL_TEXTURE0 + 0 );         // 纹理数据由GL_TEXTURE0纹理单元管理
    glBindTexture( GL_TEXTURE_2D, texture );

    shaderProgram.use();                        // 设置uniform值之前别忘记glUseProgram
    shaderProgram.setInt( "texture0", 0 );      // 对应着色器中的Uniform变量，uniform sampler2D texture0;

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );  // “X”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );  // “Y”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略

    GLint width, height, nrChannels;

    // 加载图片到内存中
    unsigned char *data = stbi_load( ( projDir + "\\res\\awesomeface.png" ).c_str(),    // 图片路径
                                     &width, &height,                                   // 图片宽高
                                     &nrChannels, 0 );                                  // 颜色通道数量
    if( data )
    {
        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,        // 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      GL_RGBA,              // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      GL_RGBA,              // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );  // 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free( data );                // 纹理数据已经上传到显存中，内存中的数据可以删除了。
}

void loadMatrix1( ShaderProgram &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );

    // OpenGL是右手坐标系，此旋转效果是：从X轴正方向朝负方向看，逆时针旋转。
    modelMatrix = glm::rotate( modelMatrix,                                         // 旋转变换
                               ( float ) glfwGetTime() * glm::radians( 50.0f ),     // 旋转角度，正负号是方向
                               glm::vec3( 1.0f, 0.0f, 0.0f ) );                     // 旋转轴：X轴，是一个向量

    // 视图变换矩阵
    glm::mat4 viewMatrix( 1.0f );
    viewMatrix = createViewMatrix( );

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) fov ),            // 就是FOV，field of view，视角广度
                                         ( float )ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

void loadMatrix2( ShaderProgram &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。

    glm::mat4 viewMatrix( 1.0f );
    viewMatrix = createViewMatrix( );

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) fov ),            // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,     // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}

glm::mat4 createViewMatrix( const CameraView &view/*= CameraView::FIX_XZ */ )
{
    cameraView = view;

    glm::mat4 viewMatrix( 1.0f );

    switch( cameraView )
    {
        case CameraView::FIX:     // 摄像机角度1：设置一个摄像机视角
        {
            viewMatrix = glm::lookAt( cameraPos,            // 摄像机位置
                                      cameraTarget,         // 目标位置（摄像机的注视点）
                                      upVecWorld );         // 世界空间的上向量
            break;
        }
        case CameraView::ROTATE_Y:     // 摄像机角度2：绕y轴旋转的摄像机视角
        {
            float radius = 10.0f;
            float camX   = ( float )sin( glfwGetTime() ) * radius;
            float camZ   = ( float )cos( glfwGetTime() ) * radius;
            viewMatrix = glm::lookAt( glm::vec3( camX, 0.0f, camZ ),    // 摄像机坐标
                                      glm::vec3( 0.0f, 0.0f, 0.0f ),    // 目标位置（摄像机的注视点）
                                      glm::vec3( 0.0f, 1.0f, 0.0f ) );  // 世界空间中的上向量
            break;
        }
        case CameraView::FREE:     //摄像机角度3：自由移动的摄像机视角
        {
            viewMatrix = glm::lookAt( cameraPos,
                                      cameraPos + cameraFront,          // 摄像机一直朝向它的正前方cameraFront，这好像是一句废话，但是camerFront一直在计算。
                                      upVecWorld );
            //viewMatrix = glm::lookAt( cameraPos,
            //                          cameraFront,                    // 摄像机移动，注视点不改变，摄像机一直朝向该注视点
            //                          upVecWorld );
            break;
        }
        case CameraView::FREE_LOOKAT:     //摄像机角度3：不用lookat函数，自己手动创建一个LookAt矩阵，达到对lookAt矩阵的掌握。
        {
            glm::vec3 z = glm::normalize( -cameraFront );               // 摄像机的朝向刚好是Z轴的相反方向。朝向一直在计算。
            glm::vec3 x = glm::normalize( glm::cross( upVecWorld, z ) );
            glm::vec3 y = glm::cross( z, x );

            glm::mat4 translateMatrix( 1.0f );      // 平移矩阵，平移到摄像机位置
            glm::mat4 rotateMatrix( 1.0f );

            // 注意，OpenGL采用列主序，所以[3][0]表示第三列第一行，要特别注意。
            translateMatrix[3][0] = -cameraPos.x;   // 实际是平移场景不是平移摄像机，所以去负值
            translateMatrix[3][1] = -cameraPos.y;
            translateMatrix[3][2] = -cameraPos.z;

            rotateMatrix[0][0] = x.x;
            rotateMatrix[1][0] = x.y;
            rotateMatrix[2][0] = x.z;

            rotateMatrix[0][1] = y.x;
            rotateMatrix[1][1] = y.y;
            rotateMatrix[2][1] = y.z;

            rotateMatrix[0][2] = z.x;
            rotateMatrix[1][2] = z.y;
            rotateMatrix[2][2] = z.z;

            // 因为是列主序矩阵，变换顺序要从右往左看。比如A*B*C*v，对位置v先进行C变换，再B变换，再C变换。
            viewMatrix = rotateMatrix * translateMatrix;
            break;
        }
        case FIX_XZ: //一个真正的FPS摄像机（也就是说不能够随意飞行），你只能够呆在xz平面上。
        {
            viewMatrix = glm::lookAt( cameraPos,
                                      cameraPos + cameraFront,          // 摄像机一直朝向它的正前方cameraFront，这好像是一句废话，但是camerFront一直在计算。
                                      upVecWorld );
            break;
        }
        default:
            break;
    }

    return viewMatrix;
}