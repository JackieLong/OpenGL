#include "project_lib.h"
#include "other.h"

using namespace std;

Camera *pCamera  = nullptr;     // 摄像机
glm::vec3 lightPos;             // 光源位置

const glm::vec3 objectColor = glm::vec3( 1.0f, 0.5f, 0.31f );       // 物体本身颜色
const glm::vec3 lightColor = glm::vec3( 1.0f, 1.0f, 1.0f );         // 光源颜色

void loadMatrixTarget( ShaderProgram &shaderProgram );
void loadMatrixLight( ShaderProgram &shaderProgram );

int main()
{
    pCamera = new Camera( glm::vec3( 1.0f, 1.0f, 7.0f ),            // 摄像机的初始位置
                          glm::vec3( 0.5f, 0.5f, 0.0f ),            // 摄像机注视点
                          glm::vec3( 0.0f, 1.0f, 0.0f ),            // 世界坐标中的向上向量
                          -90.f,                                    // Yaw
                          0.0f );                                   // Pitch
    GLFWwindow *window = init( pCamera );               // 鼠标滚轮回调

    GLuint VAO = 0, VBO = 0;                            // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    GLuint VAO1 = 0, VBO1 = 0;                          // VBO顶点缓冲对象，VAO顶点数组对象，EBO索引缓冲对象
    GLuint VAO2 = 0, VBO2 = 0;
    loadVertexDataObject( &VAO, &VBO );                 // 加载物体顶点
    loadVertexDataCoord( &VAO1, &VBO1 );                // 加载坐标轴顶点
    loadVertexDataLight( &VAO2, &VBO2 );                // 加载模拟光源物体顶点

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        shaderProgramLight.use();
        shaderProgramLight.setVec3( "lightColor", lightColor );
        loadMatrixLight( shaderProgramLight );
        glBindVertexArray( VAO2 );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        shaderProgramCoord.use();
        loadMatrixTarget( shaderProgramCoord );
        glBindVertexArray( VAO1 );
        glDrawArrays( GL_LINES, 0, 6 );

        shaderProgram.use();
        // 1：在世界坐标系中计算光照，Phong着色
        // 2：在观察坐标系中计算光照，Phong着色
        // 3：在顶点着色器中计算光照，Gourand着色
        shaderProgram.setInt( "caculateType", 2 );
        shaderProgram.setVec3( "objectColor", objectColor );        // 物体本身颜色（珊瑚红）
        shaderProgram.setVec3( "lightColor", lightColor );          // 光照颜色
        shaderProgram.setVec3( "lightPos", lightPos );              // 光源位置
        shaderProgram.setVec3( "viewPos", pCamera->pos() );         // 观察位置
        loadMatrixTarget( shaderProgram );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
    } );

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays( 1, &VAO );
    glDeleteVertexArrays( 1, &VAO1 );
    glDeleteVertexArrays( 1, &VAO2 );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &VBO1 );
    glDeleteBuffers( 1, &VBO2 );
    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &EBO1 );
    glDeleteBuffers( 1, &EBO2 );
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
                                         ( float )ScreenWidth / ScreenHeight,      // aspectRatio，宽高比
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

    lightPos = glm::vec3( 0.0f, 1.0f, 0.0f ) +
               glm::vec3( 2 * sin( 1.5f * glfwGetTime() ),
                          0,
                          2 * cos( 1.5f * glfwGetTime() ) );
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