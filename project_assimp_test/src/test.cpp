#include "project_lib.h"
#include "other.h"
#include "Model.h"

using namespace std;

glm::vec3 lightPos;

void loadMatrixTarget( ShaderProgram &shaderProgram );
void loadMatrixLight( ShaderProgram &shaderProgram );

int main()
{
    GLFWwindow *window = init();

    Model model( projectDir() + "/res/nanosuit/nanosuit.obj" );

    glm::vec3 lightColor = glm::vec3( 1.0f, 1.0f, 1.0f );           // 光源颜色
    glm::vec3 diffuseColor = lightColor   * glm::vec3( 0.5f );      // 降低影响
    glm::vec3 ambientColor = diffuseColor * glm::vec3( 0.2f );      // 很低的影响

    GLuint VAO1 = 0, VBO1 = 0;                          // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    GLuint VAO2 = 0, VBO2 = 0;
    loadVertexDataCoord( &VAO1, &VBO1 );                // 加载坐标轴顶点
    loadVertexDataLight( &VAO2, &VBO2 );                // 加载模拟光源物体顶点

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // GL_LINE：线框模式；GL_FILL：填充模式

    stbi_set_flip_vertically_on_load( true );

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        shaderProgram.use();
        shaderProgram.setVec3( "light.pos", lightPos );
        shaderProgram.setVec3( "light.ambient", ambientColor );
        shaderProgram.setVec3( "light.diffuse", diffuseColor );         // 将光照调暗了一些以搭配场景
        shaderProgram.setVec3( "light.specular", glm::vec3( 1.0f ) );
        shaderProgram.setFloat( "light.constant", 1.0f );
        shaderProgram.setFloat( "light.linear", 0.09f );
        shaderProgram.setFloat( "light.quadratic", 0.032f );

        shaderProgram.setVec3( "sunLight.dir", glm::vec3( 1.0f, 0.0f, 0.0f ) );
        shaderProgram.setVec3( "sunLight.ambient", ambientColor );
        shaderProgram.setVec3( "sunLight.diffuse", diffuseColor );         // 将光照调暗了一些以搭配场景
        shaderProgram.setVec3( "sunLight.specular", glm::vec3( 1.0f ) );

        shaderProgram.setVec3( "viewPos", pCamera->pos() );

        loadMatrixTarget( shaderProgram );

        model.draw( shaderProgram );

        loadMatrixTarget( shaderProgramCoord );
        glBindVertexArray( VAO1 );
        glDrawArrays( GL_LINES, 0, 6 );

        //loadMatrixLight( shaderProgramLight );
        //glBindVertexArray( VAO2 );
        //glDrawArrays( GL_TRIANGLES, 0, 36 );

        return true;
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteProgram( shaderProgram.id );

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void loadMatrixTarget( ShaderProgram &shaderProgram )
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

void loadMatrixLight( ShaderProgram &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。

    lightPos = glm::vec3( 0.0f, 5.0f, 0.0f ) +
               glm::vec3( 3 * sin( 2.0f * glfwGetTime() ),
                          0,
                          3 * cos( 2.0f * glfwGetTime() ) );
    modelMatrix = glm::translate( modelMatrix, lightPos );
    modelMatrix = glm::scale( modelMatrix, glm::vec3( 0.3f ) );

    glm::mat4 viewMatrix = pCamera->getViewMatrix();

    // 投影变换矩阵（透视投影）
    glm::mat4 projectionMatrix( 1.0f );
    projectionMatrix = glm::perspective( glm::radians( ( float ) pCamera->fov() ), // 就是FOV，field of view，视角广度
                                         ( float ) ScreenWidth / ScreenHeight,     // aspectRatio，宽高比
                                         0.1f,                                     // near plane，近平面位置
                                         100.0f );                                 // far plane，远平面位置

    shaderProgram.use();
    shaderProgram.setMat4( "modelMatrix", glm::value_ptr( modelMatrix ) );
    shaderProgram.setMat4( "viewMatrix", glm::value_ptr( viewMatrix ) );
    shaderProgram.setMat4( "projectionMatrix", glm::value_ptr( projectionMatrix ) );
}