#include "data.h"
#include "Model.h"

using namespace std;

glm::vec3 lightPos;

void loadMatrixTarget( Shader &shaderProgram );

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

    Model model( projectDir() + "/res/nanosuit/nanosuit.obj" );

    stbi_set_flip_vertically_on_load( true );

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        shader.use();

        shader.setVec3( "sunLight.dir", glm::vec3( 1.0f, 0.0f, 0.0f ) );
        shader.setVec3( "sunLight.ambient", ambientColor );
        shader.setVec3( "sunLight.diffuse", diffuseColor );         // 将光照调暗了一些以搭配场景
        shader.setVec3( "sunLight.specular", glm::vec3( 1.0f ) );

        shader.setVec3( "viewPos", pCamera->pos() );

        loadMatrixTarget( shader );

        model.draw( shader );

        loadMatrixTarget( shader_coord );
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

        return true;
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void loadMatrixTarget( Shader &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );

    // 视图变换矩阵
    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) pCamera->fov() ), // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}