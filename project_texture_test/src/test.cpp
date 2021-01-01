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
ShaderProgram buildShaderProgram();
void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void buildVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO );
void buildTextureData1( const ShaderProgram & );
void buildTextureData2( const ShaderProgram & );

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
    buildVertexData( &VAO, &VBO, &EBO );            // 构建顶点数据

    // OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。此函数帮助我们翻转y轴
    stbi_set_flip_vertically_on_load( true );
    buildTextureData1( shaderProgram );
    buildTextureData2( shaderProgram );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE：线框模式；GL_FILL：填充模式

    renderLoop( window, [&]                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );  // 根据索引缓冲中的6个元素，绘制三角形，刚好得到2个三角形
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

ShaderProgram buildShaderProgram()
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

void buildVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO )
{
    // 4个绘制两个三角形，组成一个矩形。
    float vertices[] =
    {
        // 位置                // 颜色                  // 纹理坐标
        0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,   0.75f, 0.75f,  // 右上
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   0.75f, 0.65f,  // 右下
        -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f, 1.0f,   0.65f, 0.75f,  // 左上
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.65f, 0.65f,  // 左下
    };
    // OpenGL规定：顶点坐标（0.0，0.0）在中心点，而纹理坐标（0.0，0.0）在左下角
    // 在常见图片格式中，（0.0，0.0）可能是在图片左上角，因此可能导致图片倒转。

    unsigned int indices[] =    // 矩形顶点索引
    {
        0, 1, 2,                // 第一个三角形
        1, 2, 3,                // 第二个三角形
    };

    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glGenBuffers( 1, EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0,                       // 顶点属性（3D坐标）位置值
                           // 指定vertex attribute的位置值，这个值对应在顶点着色器中的location=0的属性
                           3,                       // 顶点属性的分量数量（number of component）
                           GL_FLOAT,                // 分量数据类型（data type of component）
                           GL_FALSE,                // 数据是否需要被标准化
                           9 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glVertexAttribPointer( 1,                       // 顶点属性（颜色）位置值
                           4,
                           GL_FLOAT,
                           GL_FALSE,
                           9 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );

    glVertexAttribPointer( 2,                       // 顶点属性（纹理坐标）位置值
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           9 * sizeof( float ),
                           ( void * )( 7 * sizeof( float ) ) );

    glEnableVertexAttribArray( 0 );     // 顶点属性配置默认是关闭的，要记得打开，0对应layout(location=0)顶点属性
    glEnableVertexAttribArray( 1 );     //
    glEnableVertexAttribArray( 2 );     //

    glBindVertexArray( 0 );
}

// 构建第一张图的纹理
void buildTextureData1( const ShaderProgram &shaderProgram )
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
    unsigned char *data = stbi_load( ( projDir + "\\res\\wall.jpg" ).c_str(),   // 图片路径
                                     &width, &height,       // 图片宽高
                                     &nrChannels, 0 );      // 颜色通道数量
    if( data )
    {
        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,		// 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      GL_RGB,               // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      GL_RGB,               // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );	// 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free( data );				// 纹理数据已经上传到显存中，内存中的数据可以删除了。
}

// 构建第二张图，同上
void buildTextureData2( const ShaderProgram &shaderProgram )
{
    string projDir = projectDir();

    GLuint texture;
    glGenTextures( 1, &texture );

    shaderProgram.use();

    shaderProgram.setInt( "texture1", 1 );

    glActiveTexture( GL_TEXTURE0 + 1 ); // 在绑定纹理之前先激活纹理单元
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    GLint width, height, nrChannels;
    unsigned char *data = stbi_load( ( projDir + "\\res\\awesomeface.png" ).c_str(), &width, &height, &nrChannels, 0 );
    if( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free( data );
}

