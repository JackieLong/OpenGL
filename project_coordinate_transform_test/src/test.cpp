#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <string>

#include "project_lib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"              // 图片加载库，支持大部分常见格式

using namespace std;

void framebuffer_size_callback( GLFWwindow *window, int width, int height );
void initGLFW();
GLFWwindow *createWindow();
bool loadGLFuncPointer();
ShaderProgram loadShaderProgram();
void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void loadVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO );
void loadTextureData( const ShaderProgram & );
void loadMatrix( ShaderProgram &shaderProgram );

const float ScreenWidth = 800;
const float ScreenHeight = 600;

int main()
{
    initGLFW();                                                             // 初始化GLFW

    GLFWwindow *window = createWindow();                                    // 创建窗口
    if( window == NULL )
    {
        return -1;
    }

    loadGLFuncPointer();                                                    // glad: load all OpenGL function pointers

    ShaderProgram shaderProgram = loadShaderProgram();                      // build and compile our shader program

    GLuint VAO = 0, VBO = 0, EBO = 0;
    loadVertexData( &VAO, &VBO, &EBO );             // 加载顶点数据到缓冲区

    loadTextureData( shaderProgram );               // 加载纹理数据到显存中

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE：线框模式；GL_FILL：填充模式

    glEnable( GL_DEPTH_TEST );                      // 开启深度测试，默认是不开启的。

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
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }
    return true;
}

ShaderProgram loadShaderProgram()
{
    string projDir = projectDir();
    return ShaderProgram( projDir + "\\src\\shader\\shader_vertex",     // 顶点着色器脚本
                          projDir + "\\src\\shader\\shader_fragment" ); // 片段着色器脚本
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

void loadVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO )
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

void loadMatrix( ShaderProgram &shaderProgram )
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
                                         ScreenWidth / ScreenHeight,     // aspectRatio，宽高比
                                         0.1f,                            // near plane，近平面位置
                                         100.0f );                       // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}
