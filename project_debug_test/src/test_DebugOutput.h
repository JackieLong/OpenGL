#pragma once

#ifndef __TEST_DEBUGOUTPUT_H__
#define __TEST_DEBUGOUTPUT_H__

#include "data.h"

void APIENTRY debugOutputCallback( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam );
void initDebugOutput();

int test_debugOutput()
{
    initGLFW();                                                                                                                                                     // ��ʼ��GLFW

    // **********************************
    // ��һ����������������������
    // **********************************
    // ��OpenGL����һ��������������ģ��������������Ϣ
    // �ڵ�����������ʹ��OpenGL�����Ը�����һ�㣬���Ե������Ż����߷�������֮ǰ�뽫��һGLFW���������ע�͵���
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );
    initGLAD();
    initGLState();

    // **********************************
    // �ڶ�������ʼ���������
    // **********************************
    initDebugOutput();

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,    // ����һ��GL_INVALID_OPERATION���󣬴���debugOutputCallback
                      ScreenWidth, ScreenHeight,
                      0, GL_RGBA,
                      GL_UNSIGNED_BYTE_3_3_2,
                      NULL );

        glBindBuffer( GL_VERTEX_ARRAY, -1 );        // ����һ��GL_INVALID_ENUM���󣬴���debugOutputCallback

        glDebugMessageInsert(                       // �ֶ����һ���Զ���������������debugOutputCallback
                        GL_DEBUG_SOURCE_APPLICATION,
                        GL_DEBUG_TYPE_ERROR,
                        0,
                        GL_DEBUG_SEVERITY_MEDIUM,
                        -1,
                        "error message here" );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void initDebugOutput()
{
    int flags;
    glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
    if( flags & GL_CONTEXT_FLAG_DEBUG_BIT )         // ����Ƿ�ɹ���ʼ���˵���������
    {
        // ��ʼ���������
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );    // makes sure errors are displayed synchronously
        glDebugMessageCallback(                     // ���õ�������ص�������������������ʱ���ᴥ���ú���
                        debugOutputCallback,        // �ص�������ַ
                        nullptr );                  // �����Զ���Ĳ�������Ӧ�ص������е�userParam
        glDebugMessageControl(                      // ���˵������������ѡ����˳���Ҫ�Ĵ�������
                        GL_DONT_CARE,
                        GL_DONT_CARE,
                        GL_DONT_CARE,
                        0,
                        nullptr,
                        GL_TRUE );
    }
}

void APIENTRY debugOutputCallback(
                GLenum source,
                GLenum type,
                unsigned int id,
                GLenum severity,
                GLsizei length,
                const char *message,
                const void *userParam )
{
    if( id == 131169 || id == 131185 || id == 131218 || id == 131204 )
    {
        // ��Щ�ǲ���Ҫ�Ĵ��󣬿��Ժ���
        return;
    }

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch( source )
    {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch( type )
    {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch( severity )
    {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

#endif