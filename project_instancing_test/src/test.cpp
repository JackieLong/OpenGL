#include "data.h"

using namespace std;

void renderLoop( GLFWwindow *window, function<void()> renderCallback );
void mouse_move_callback( GLFWwindow *window, double xpos, double ypos );
void mouse_scroll_callback( GLFWwindow *window, double xoffset, double yoffset );
void showInstanceRenderHowToWork();
void showPlanetarybelt();
void showPlanetarybelt_Instance();
void updateMatrixData();

int main()
{
    initGLFW();

    GLFWwindow *window = createWindow( ScreenWidth,
                                       ScreenHeight,
                                       "LearnOpenGL",
                                       []( GLFWwindow * window, double xoffset, double yoffset )
    {
        pCamera->processMouseMovement( window, xoffset, yoffset );
    },
    []( GLFWwindow * window, double xoffset, double yoffset )
    {
        pCamera->processMouseScroll( window, xoffset, yoffset );
    } );

    initGLAD();

    initGLState();

    loadData();

    renderLoop( window, [&]
    {
        // 陨石自旋转
        rotateSelfMatrix   = glm::rotate( glm::mat4( 1.0f ),
                                          glm::radians( ( float )glfwGetTime() * 50.0f ),
                                          glm::vec3( 0.0f, 1.0f, 0.0f ) );
        // 陨石绕行星旋转
        rotatePlanetMatrix = glm::rotate( glm::mat4( 1.0f ),
                                          glm::radians( ( float ) glfwGetTime() * 5.0f ),
                                          glm::vec3( 0.0f, 1.0f, 0.0f ) );
        viewMatrix         = pCamera->getViewMatrix();
        projectionMatrix   = glm::perspective( glm::radians( pCamera->fov() ),            // 就是FOV，field of view，视角广度
                                               ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                               0.1f,                                      // near plane，近平面位置
                                               100.0f );                                  // far plane，远平面位置
        updateMatrixData();

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        // 展示实例化渲染的工作方式
        // 这里是渲染100个矩形，使用了相同的顶点数据。
        // 外加每个矩形（4个顶点）使用同一个顶点属性数据（位置偏移量），由glVertexAttribDivisor完成。
        //showInstanceRenderHowToWork();

        // 用最普通的方法，绘制一个由数以千记的陨石组成的行星带。
        //showPlanetarybelt();

        // 用实例化渲染的方法，绘制行星带，性能大大提高
        showPlanetarybelt_Instance();
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    //glDeleteVertexArrays( 1, &VAO_quad );
    //glDeleteVertexArrays( 1, &VAO_coord );
    //glDeleteBuffers( 1, &VBO_quad );
    //glDeleteBuffers( 1, &VBO_coord );
    //glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void showInstanceRenderHowToWork()
{
    shader_quad.use();
    glBindVertexArray( VAO_quad );
    glDrawArraysInstanced( GL_TRIANGLES, 0, 6, 100 );

    shader_coord.use();
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );
}

void showPlanetarybelt()
{
    shader_planet.use();
    shader_planet.setInt( "task", 0 );
    shader_planet.setMat4( "modelMatrix",
                           glm::rotate( glm::mat4( 1.0f ),
                                        ( float ) glm::radians( glfwGetTime() * 2 ),
                                        glm::vec3( 0.5f, 0.5f, 0.0f ) ) );
    modelPlanet->draw( shader_planet );

    shader_rock.use();
    for( auto &it : vModelMatrix_rock )
    {
        shader_rock.setMat4( "modelMatrix", it );
        shader_rock.setMat4( "rotateSelfMatrix", rotateSelfMatrix );
        shader_rock.setMat4( "rotatePlanetMatrix", rotatePlanetMatrix );

        modelRock->draw( shader_rock );
    }

    shader_coord.use();
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );
}

void showPlanetarybelt_Instance()
{
    shader_planet.use();
    shader_planet.setMat4( "modelMatrix",
                           glm::rotate( glm::mat4( 1.0f ),
                                        ( float ) glm::radians( glfwGetTime() * 2 ),
                                        glm::vec3( 0.5f, 0.5f, 0.0f ) ) );
    modelPlanet->draw( shader_planet );

    shader_rock.use();
    shader_rock.setInt( "task", 1 );
    shader_rock.setMat4( "rotateSelfMatrix", rotateSelfMatrix );
    shader_rock.setMat4( "rotatePlanetMatrix", rotatePlanetMatrix );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, modelRock->mTextures_loaded[0].id );

    for( unsigned int i = 0; i < modelRock->mMeshes.size(); i++ )
    {
        glBindVertexArray( modelRock->mMeshes[i].mVAO );
        glDrawElementsInstanced( GL_TRIANGLES,
                                 modelRock->mMeshes[i].mIndices.size(),
                                 GL_UNSIGNED_INT,
                                 ( GLvoid * )( 0 * sizeof( GLuint ) ),
                                 ROCK_NUM );
    }

    shader_coord.use();
    glBindVertexArray( VAO_coord );
    glDrawArrays( GL_LINES, 0, 6 );
}

void updateMatrixData()
{
    shader_quad.use();
    shader_quad.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader_quad.setMat4( "viewMatrix", viewMatrix );
    shader_quad.setMat4( "projectionMatrix", projectionMatrix );

    shader_coord.use();
    shader_coord.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader_coord.setMat4( "viewMatrix", viewMatrix );
    shader_coord.setMat4( "projectionMatrix", projectionMatrix );

    shader_planet.use();
    shader_planet.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader_planet.setMat4( "viewMatrix", viewMatrix );
    shader_planet.setMat4( "projectionMatrix", projectionMatrix );

    shader_rock.use();
    shader_rock.setMat4( "modelMatrix", glm::mat4( 1.0f ) );
    shader_rock.setMat4( "viewMatrix", viewMatrix );
    shader_rock.setMat4( "projectionMatrix", projectionMatrix );
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
    pCamera->processKeyboard( window, ( float ) deltaTime );
}

void renderLoop( GLFWwindow *window, function<void()> renderCallback )
{
    double currentFrame;
    while( !glfwWindowShouldClose( window ) )
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput( window );                     // 处理输入：键盘、鼠标事件等。

        if( renderCallback != nullptr )             // render loop
        {
            renderCallback();
        }

        glfwSwapBuffers( window );                  // 双缓冲

        // IO轮询，检查有没有触发什么事件（keys pressed/released, mouse moved etc.）,
        // 并调用对应的回调函数（可以通过回调方法手动设置）,更新窗口状态，
        glfwPollEvents();
    }
}
