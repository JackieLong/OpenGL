#include <iostream>
#include <fstream>
#include <functional>
#include "project_lib.h"

void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );
void createVertexBuffer( GLuint &VAO, GLuint &VBO, GLuint &EBO );

int main()
{
    initGLFW();     // ��ʼ��GLFW

    GLFWwindow *window = createWindow( 800, 600, "LearnOpenGL" );

    initGLAD();     // ��ʼ��GLAD

    Shader shader = Shader( projectDir() + "/src/shader/shader_vertex",
                            projectDir() + "/src/shader/shader_fragment" );

    GLuint VAO = 0, VBO = 0, EBO = 0;
    createVertexBuffer( VAO, VBO, EBO );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    // GL_LINE���߿�ģʽ��GL_FILL�����ģʽ

    renderLoop( window, [&]                         // ��Ⱦѭ��
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // ״̬���ú��������������Ļ����ɫ������������ɫ
        glClear( GL_COLOR_BUFFER_BIT );             // ״̬ʹ�ú�����ִ�������״̬���á�

        shader.use();                               // ������ɫ������
        shader.setVec4( "appColor", 0.0f, ( float )( sin( glfwGetTime() ) / 2.0f ) + 0.5f, 0.0f, 1.0f );

        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0 );
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

void createVertexBuffer( GLuint &VAO, GLuint &VBO, GLuint &EBO )
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

    createVertexBuffer( vertices, sizeof( vertices ), "34", &VAO, &VBO, indices, sizeof( indices ), &EBO );
}