#include "utils.h"
#include <string>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

string projectDir()
{
    string ret;
    char *tmpProjectDir = _getcwd( NULL, 0 );
    if( tmpProjectDir == nullptr )
    {
        perror( "getcwd error" );
        cout << "Failed to get project dir" << endl;
    }
    else
    {
        ret = tmpProjectDir;
        free( tmpProjectDir );
    }
    return ret;
}

GLuint createTexture( const std::string &path,
                      std::function<void()> paramCallback /*=nullptr*/ )
{
    GLuint textureID;
    glGenTextures( 1, &textureID );

    glBindTexture( GL_TEXTURE_2D, textureID );      // 暂时的绑定，目的是让后续的开辟纹理内存生效（glTexImage2D），最后应该恢复成默认绑定

    if( paramCallback != nullptr )
    {
        paramCallback();
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );                     // “X”轴方向上的Wrapping方式，纹理坐标不在[0,1]之间时，如何采样
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );                     // “Y”轴方向上的Wrapping方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );   // 纹理缩小时，采用GL_LINEAR策略
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );                 // 纹理放大时，采用GL_LINEAR策略
    }

    GLint width, height, nrChannels;

    // 加载图片到内存中
    unsigned char *data = stbi_load( path.c_str(),      // 图片路径
                                     &width, &height,   // 图片宽高
                                     &nrChannels, 0 );  // 颜色通道数量
    if( data )
    {
        GLenum format = nrChannels == 1 ? GL_RED :
                        nrChannels == 3 ? GL_RGB :
                        nrChannels == 4 ? GL_RGBA :
                        GL_RGB;

        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,        // 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      format,               // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      format,               // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );  // 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }

    stbi_image_free( data );                // 纹理数据已经上传到显存中，内存中的数据可以删除了。
    glBindTexture( GL_TEXTURE_2D, 0 );      // 恢复成默认

    return textureID;
}

void createVertexBuffer( const GLfloat *vertices,
                         const int &len,
                         const std::string &components,
                         GLuint *VAO,
                         GLuint *VBO,
                         const GLuint *indices /*= nullptr*/,
                         const int &lenIndices /*= 0*/,
                         GLuint *EBO /*= nullptr */ )
{
    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW );

    if( EBO )
    {
        glGenBuffers( 1, EBO );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *EBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, lenIndices, indices, GL_STATIC_DRAW );
    }

    GLint   componentNum;
    GLsizei stride  = 0;
    GLint   tmpSize = 0;

    for( size_t i = 0; i < components.size(); i++ )
    {
        stride += components.at( i ) - '0';
    }

    for( size_t i = 0; i < components.size(); i++ )
    {
        componentNum = components.at( i ) - '0';
        glEnableVertexAttribArray( i );
        glVertexAttribPointer( i,           // vertex attribute的位置值，这个值对应在顶点着色器中的location=index的属性
                               componentNum,        // 顶点属性的分量数量
                               GL_FLOAT,    // 分量数据类型
                               GL_FALSE,    // 数据是否需要被标准化
                               stride * sizeof( float ),  // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                               ( void * )( tmpSize * sizeof( float ) ) );          // 数据在缓冲中起始位置的偏移量
        tmpSize += componentNum;
    }

    glBindVertexArray( 0 );
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

bool initGLAD()
{
    if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
    {
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }
    return true;
}

GLFWwindow *createWindow( const int         width,
                          const int         height,
                          const std::string &windowName,
                          GLFWcursorposfun  mouse_move_callback     /*= nullptr*/,
                          GLFWscrollfun     mouse_scroll_callback   /*= nullptr */ )
{
    GLFWwindow *window = glfwCreateWindow( width, height,           // 窗口宽高
                                           windowName.c_str(),      // 窗口名称
                                           NULL, NULL );
    if( window == NULL )
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    else
    {
        glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );      // 隐藏光标，并捕捉。
        glfwMakeContextCurrent( window );                                   // 设置当前线程的上下文为window的上下文。
        if( mouse_move_callback )
        {
            glfwSetCursorPosCallback( window, mouse_move_callback );        // 鼠标移动触发回调
        }
        if( mouse_scroll_callback )
        {
            glfwSetScrollCallback( window, mouse_scroll_callback );         // 鼠标滚轮回调
        }

        // glfw: whenever the window size changed (by OS or user resize) this callback function executes
        // ---------------------------------------------------------------------------------------------
        glfwSetFramebufferSizeCallback( window, []( GLFWwindow * window, int width, int height )    // 窗口大小改变回调
        {
            // make sure the viewport matches the new window dimensions; note that width and
            // height will be significantly larger than specified on retina displays.
            glViewport( 0, 0,               // 视口左下角位置
                        width, height );    // 视口宽高
        } );
    }
    return window;
}
