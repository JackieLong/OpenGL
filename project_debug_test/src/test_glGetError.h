#pragma once

#ifndef __TEST_GLGETERROR_H__
#define __TEST_GLGETERROR_H__

#include "data.h"

void test_glGetError()
{
    initGLFW();                                                                                                                                                     // ��ʼ��GLFW
    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );
    initGLAD();
    initGLState();

    GLenum errorCode;

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );

        // **********************************
        // ����һ
        // **********************************
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,    // ����һ��GL_INVALID_OPERATION����
                      ScreenWidth, ScreenHeight,
                      0, GL_RGBA,
                      GL_UNSIGNED_BYTE_3_3_2,
                      NULL );
        errorCode = glCheckError();                 // GL_INVALID_OPERATION


        glBindBuffer( GL_VERTEX_ARRAY, -1 );        // ����һ��GL_INVALID_ENUM����
        errorCode = glCheckError();                 // GL_INVALID_ENUM

        errorCode = glCheckError();                 // GL_NO_ERROR

        // **********************************
        // ���ζ�
        // **********************************
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,    // ����һ��GL_INVALID_OPERATION����
                      ScreenWidth, ScreenHeight,
                      0, GL_RGBA,
                      GL_UNSIGNED_BYTE_3_3_2,
                      NULL );
        glBindBuffer( GL_VERTEX_ARRAY, -1 );        // ����һ��GL_INVALID_ENUM����

        errorCode = glCheckError();                 // GL_INVALID_OPERATION
        errorCode = glCheckError();                 // GL_NO_ERROR

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
}

#endif