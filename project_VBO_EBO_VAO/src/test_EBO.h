#ifndef __TEST_EBO_H__
#define __TEST_EBO_H__

#include "data.h"

static void drawEBO()
{
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
    glClear( GL_COLOR_BUFFER_BIT );             // 状态使用函数：执行上面的状态设置。

    shader.use();                               // 启用着色器程序
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
}

static void createEBO()
{
    glGenBuffers( 1, &VBO );                // 创建一个缓冲对象
    glBindBuffer( GL_ARRAY_BUFFER, VBO );   // 指定缓冲对象的目标类型为顶点缓冲对象。

    // 为顶点缓冲数组创建data store（内存）
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( vertices_EBO ),   // data store 字节数
                  vertices_EBO,             // 初始化数据
                  GL_STATIC_DRAW );         // 用于application绘制的数据源，只会修改一次，但会读取很多次

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0,                                           // vertex attribute的位置值，这个值对应在顶点着色器中的location=index的属性
                           3,                                           // 顶点属性的分量数量
                           GL_FLOAT,                                    // 分量数据类型
                           GL_FALSE,                                    // 数据是否需要被标准化
                           3 * sizeof( GL_FLOAT ),                      // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( GLvoid * )( 0 * sizeof( GL_FLOAT ) ) );    // 数据在缓冲中起始位置的偏移量

    glGenBuffers( 1, &EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  sizeof( indices_EBO ),
                  indices_EBO,
                  GL_STATIC_DRAW );
}

static void test_EBO( GLFWwindow *window )
{
    createEBO();

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );                     // 处理输入：键盘、鼠标事件等。

        drawEBO();

        glfwSwapBuffers( window );  // 双缓冲

        // IO轮询，检查有没有触发什么事件（keys pressed/released, mouse moved etc.）,
        // 并调用对应的回调函数（可以通过回调方法手动设置）,更新窗口状态，
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