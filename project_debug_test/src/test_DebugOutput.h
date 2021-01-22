#pragma once

#ifndef __TEST_DEBUGOUTPUT_H__
#define __TEST_DEBUGOUTPUT_H__

#include "data.h"

void APIENTRY debugOutputCallback( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam );
void initDebugOutput();

int test_debugOutput()
{
    initGLFW();                                                                                                                                                     // 初始化GLFW

    // **********************************
    // 第一步：请求调试输出的上下文
    // **********************************
    // 向OpenGL请求一个调试输出上下文，用于输出调试信息
    // 在调试上下文中使用OpenGL会明显更缓慢一点，所以当你在优化或者发布程序之前请将这一GLFW调试请求给注释掉。
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );
    initGLAD();
    initGLState();

    // **********************************
    // 第二步：初始化调试输出
    // **********************************
    initDebugOutput();

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,    // 产生一个GL_INVALID_OPERATION错误，触发debugOutputCallback
                      ScreenWidth, ScreenHeight,
                      0, GL_RGBA,
                      GL_UNSIGNED_BYTE_3_3_2,
                      NULL );

        glBindBuffer( GL_VERTEX_ARRAY, -1 );        // 产生一个GL_INVALID_ENUM错误，触发debugOutputCallback

        glDebugMessageInsert(                       // 手动输出一条自定义调试输出，触发debugOutputCallback
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
    if( flags & GL_CONTEXT_FLAG_DEBUG_BIT )         // 检查是否成功初始化了调试上下文
    {
        // 初始化调试输出
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );    // makes sure errors are displayed synchronously
        glDebugMessageCallback(                     // 设置调试输出回调函数，到产生错误标记时，会触发该函数
                        debugOutputCallback,        // 回调函数地址
                        nullptr );                  // 我们自定义的参数，对应回调函数中的userParam
        glDebugMessageControl(                      // 过滤调试输出，可以选择过滤出需要的错误类型
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
        // 这些是不重要的错误，可以忽略
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