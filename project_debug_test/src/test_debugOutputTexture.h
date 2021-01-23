#pragma once

#ifndef __TEST_DEBUGOUTPUTTEXTURE_H__
#define __TEST_DEBUGOUTPUTTEXTURE_H__

#include "data.h"

void test_debugOutputTexture()
{
    initGLFW();                                                                                                                                                     // ≥ı ºªØGLFW
    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );
    initGLAD();
    initGLState();
    loadData();

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        debugOutputTexture( texture_debugOutput );

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