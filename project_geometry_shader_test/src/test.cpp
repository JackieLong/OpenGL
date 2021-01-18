#include "data.h"

using namespace std;

void loadShaderData_obj();
void loadShaderData_normal();
void loadShaderData_coord();
void showExplode( Model &model );
void showNormal( Model &model );

bool enableExplode = true;

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

    //stbi_set_flip_vertically_on_load( true );

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        loadShaderData_coord();
        glBindVertexArray( VAO_coord );
        glDrawArrays( GL_LINES, 0, 6 );

        //showExplode(model);

        showNormal( model );

        return true;
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteProgram( shader.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void showExplode( Model &model )
{
    enableExplode = true;
    loadShaderData_obj();
    model.draw( shader );
}
void showNormal( Model &model )
{
    enableExplode = false;
    loadShaderData_obj();
    model.draw( shader );

    loadShaderData_normal();
    model.draw( shadernormal );
}

void loadShaderData_obj()
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

    shader.use();
    shader.setInt( "enableExplode", enableExplode );
    shader.setFloat( "time", ( float )glfwGetTime() );
    shader.setMat4( "modelMatrix", modelMatrix );
    shader.setMat4( "viewMatrix", viewMatrix );
    shader.setMat4( "projectionMatrix", projectionMatrix );
}

void loadShaderData_normal()
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
    shadernormal.use();
    shadernormal.setMat4( "modelMatrix", modelMatrix );
    shadernormal.setMat4( "viewMatrix", viewMatrix );
    shadernormal.setMat4( "projectionMatrix", projectionMatrix );
}

void loadShaderData_coord()
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

    shader_coord.use();
    shader_coord.setMat4( "modelMatrix", modelMatrix );
    shader_coord.setMat4( "viewMatrix", viewMatrix );
    shader_coord.setMat4( "projectionMatrix", projectionMatrix );
}