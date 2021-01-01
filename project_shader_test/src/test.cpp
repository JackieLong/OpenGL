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
    initGLFW();                                                             // ��ʼ��GLFW

    GLFWwindow *window = createWindow();                                    // ��������
    if( window == NULL )
    {
        return -1;
    }

    loadGLFuncPointer();                                                    // glad: load all OpenGL function pointers

    ShaderProgram shaderProgram = buildShaderProgram();                     // build and compile our shader program

    GLuint VAO = 0, VBO = 0, EBO = 0;
    buildVertexData( &VAO, &VBO, &EBO );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE���߿�ģʽ��GL_FILL�����ģʽ

    renderLoop( window, [&]                         // ��Ⱦѭ��
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // ״̬���ú��������������Ļ����ɫ������������ɫ
        glClear( GL_COLOR_BUFFER_BIT );             // ״̬ʹ�ú�����ִ�������״̬���á�

        shaderProgram.use();                        // ������ɫ������

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
    glViewport( 0, 0,               // �ӿ����½�λ��
                width, height );    // �ӿڿ��
}

void initGLFW()
{
    //--------------------------------------------------------- 1����ʼ��glfw
    glfwInit();                                                                                                                                                       // ��ʼ��GLFW

    //--------------------------------------------------------- 2������gflw������Ҫ��������OpenGL 3.3���ϡ�
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );                    // ���汾�ţ�3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );                    // �ΰ汾�ţ�3
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );    // ʹ��opengl core profile����ģʽ�����±���ʹ��VAO��

    #ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
}

GLFWwindow *createWindow()
{
    GLFWwindow *window = glfwCreateWindow( 800, 600,        // ���ڿ��
                                           "LearnOpenGL",   // ��������
                                           NULL, NULL );
    if( window == NULL )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    else
    {
        glfwMakeContextCurrent( window );                                       // ���õ�ǰ�̵߳�������Ϊwindow�������ġ�
        glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );    // ���ڴ�С�ı�ص�
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
        processInput( window );                     // �������룺���̡�����¼��ȡ�

        if( renderCallback != nullptr )             // render loop
        {
            renderCallback();
        }

        glfwSwapBuffers( window );  // ˫����

        // IO��ѯ�������û�д���ʲô�¼���keys pressed/released, mouse moved etc.��,
        // �����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�,���´���״̬��
        glfwPollEvents();
    }
}

void buildVertexData( GLuint *VAO, GLuint *VBO, GLuint *EBO )
{
    // 3���������һ��������
    float vertices[] =
    {
        // λ��                  // ��ɫ
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, 1.0f, // ����
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, // ����
        0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f, // ����
    };

    unsigned int indices[] =    // �����εĶ�������
    {
        0, 1, 2,                // ��һ��������
    };

    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glGenBuffers( 1, EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0,                       // ָ��vertex attribute��λ��ֵ�����ֵ��Ӧ�ڶ�����ɫ���е�location=0������
                           3,                       // �������Եķ���������number of component��
                           GL_FLOAT,                // �����������ͣ�data type of component��
                           GL_FALSE,                // �����Ƿ���Ҫ����׼��
                           7 * sizeof( float ),     // ����Stride�����Լ����ע������������ֵ��ͬλ�õļ����������β�����
                           ( void * ) 0 );          // �����ڻ�������ʼλ�õ�ƫ����

    glVertexAttribPointer( 1,                       // ָ��vertex attribute��λ��ֵ�����ֵ��Ӧ�ڶ�����ɫ���е�location=1������
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           7 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );

    glEnableVertexAttribArray( 0 );     // ������������Ĭ���ǹرյģ�Ҫ�ǵô򿪣�0��Ӧlayout(location=0)��������
    glEnableVertexAttribArray( 1 );     //

    glBindVertexArray( 0 );
}