#include "data.h"
#include "test_VAO.h"
#include "test_VBO.h"
#include "test_EBO.h"

void initGLState();
void renderLoop( GLFWwindow *window, std::function<void()> renderCallback );

int main()
{
    glfwInit();                                                                                                                                                       // 初始化GLFW
    glfwDefaultWindowHints();
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

    GLFWwindow *window = createWindow( ScreenWidth, ScreenHeight, "LearnOpenGL" );
    initGLAD();     // 初始化GLAD
    initGLState();
    initDebugOutputCallback();

    loadData();

    //test_VBO( window );
    //test_EBO( window );
    test_VAO( window );

    return 0;
}

void initGLState()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    // GL_LINE：线框模式；GL_FILL：填充模式
}