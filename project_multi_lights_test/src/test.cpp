#include "project_lib.h"
#include "other.h"

using namespace std;

Camera *pCamera = nullptr;                         // 摄像机
glm::vec3 pointLightPos;                                // 点光源位置

void loadMatrixLight( ShaderProgram &shaderProgram );
void loadMatrixTarget( ShaderProgram &shaderProgram );
void loadVertexDataObject( GLuint *VAO, GLuint *VBO );
GLint loadTextureDiffuse( const ShaderProgram &shaderProgram );
GLint loadTextureSpecular( const ShaderProgram &shaderProgram );

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

    GLint sampler1 = loadTextureDiffuse( shaderProgram );           // 加载漫反射贴图
    GLint sampler2 = loadTextureSpecular( shaderProgram );          // 加载镜面反射贴图

    glm::vec3 cubePositions[] =                     // 每个物体模型的位移。
    {
        glm::vec3( 0.0f,  0.0f,  0.0f ),
        glm::vec3( 2.0f,  5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f, -2.5f ),
        glm::vec3( -3.8f, -2.0f, -12.3f ),
        glm::vec3( 2.4f, -0.4f, -3.5f ),
        glm::vec3( -1.7f,  3.0f, -7.5f ),
        glm::vec3( 1.3f, -2.0f, -2.5f ),
        glm::vec3( 1.5f,  2.0f, -2.5f ),
        glm::vec3( 1.5f,  0.2f, -1.5f ),
        glm::vec3( -1.3f,  1.0f, -1.5f )
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] =
    {
        glm::vec3( 0.7f,  0.2f,  2.0f ),
        glm::vec3( 2.3f, -3.3f, -4.0f ),
        glm::vec3( -4.0f,  2.0f, -12.0f ),
        glm::vec3( 0.0f,  0.0f, -3.0f )
    };

    renderLoop( window, [&]                                         // 渲染循环
    {
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );                     // 状态设置函数：设置清空屏幕后，颜色缓冲区填充的颜色
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );       // 状态使用函数：清空颜色缓冲区和深度缓冲区

        loadMatrixLight( shaderProgramLight );
        glBindVertexArray( VAO2 );
        for( int i = 0; i < 4; i++ )
        {
            shaderProgramLight.use();
            glm::mat4 modelMatrix( 1.0f );
            modelMatrix = glm::translate( modelMatrix, pointLightPositions[i] );
            modelMatrix = glm::scale( modelMatrix, glm::vec3( 0.3f ) );
            shaderProgramLight.setMat4( "modelMatrix", modelMatrix );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        loadMatrixTarget( shaderProgramCoord );
        glBindVertexArray( VAO1 );
        glDrawArrays( GL_LINES, 0, 6 );

        shaderProgram.use();
        // 1：平行光（太阳光）
        // 2：点光源
        // 3：聚光（SpotLight），如手电筒
        shaderProgram.setInt( "lightType", 3 );
        shaderProgram.setVec3( "viewPos", pCamera->pos() );             // 观察位置
        shaderProgram.setInt( "material.diffuse", sampler1 );
        shaderProgram.setInt( "material.specular", sampler2 );
        shaderProgram.setFloat( "material.shininess", 64.0f );

        shaderProgram.setVec3( "sunLight.dir", glm::vec3( 0.0f, -1.0f, 0.0f ) );
        shaderProgram.setVec3( "sunLight.ambient", glm::vec3( 0.1f ) );
        shaderProgram.setVec3( "sunLight.diffuse", glm::vec3( 0.5f ) );        // 将光照调暗了一些以搭配场景
        shaderProgram.setVec3( "sunLight.specular", glm::vec3( 1.0f ) );

        // point light 1
        glm::vec3 ambient( 0.05f, 0.05f, 0.05f );
        glm::vec3 diffuse( 0.1f, 0.8f, 0.1f );
        glm::vec3 specular( 1.0f, 1.0f, 1.0f );
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        ostringstream ostrstream;
        for( int i = 0; i < 4; i++ )
        {
            ostrstream << "pointLights[" << i << "].pos";
            shaderProgram.setVec3( ostrstream.str(), pointLightPositions[i] );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].ambient";
            shaderProgram.setVec3( ostrstream.str(), ambient );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].diffuse";
            shaderProgram.setVec3( ostrstream.str(), diffuse );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].specular";
            shaderProgram.setVec3( ostrstream.str(), specular );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].constant";
            shaderProgram.setFloat( ostrstream.str(), constant );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].linear";
            shaderProgram.setFloat( ostrstream.str(), linear );
            ostrstream.str( "" );
            ostrstream << "pointLights[" << i << "].quadratic";
            shaderProgram.setFloat( ostrstream.str(), quadratic );
            ostrstream.str( "" );
        }

        shaderProgram.setVec3( "spotLight.pos", pCamera->pos() );
        shaderProgram.setVec3( "spotLight.dir", pCamera->front() );

        shaderProgram.setVec3( "spotLight.ambient", ambient );
        shaderProgram.setVec3( "spotLight.diffuse", diffuse );
        shaderProgram.setVec3( "spotLight.specular", specular );

        shaderProgram.setFloat( "spotLight.constant", 1.0f );
        shaderProgram.setFloat( "spotLight.linear", 0.09f );
        shaderProgram.setFloat( "spotLight.quadratic", 0.032f );

        shaderProgram.setFloat( "spotLight.cutoff", glm::cos( glm::radians( 12.5f ) ) );
        shaderProgram.setFloat( "spotLight.cutoff1", glm::cos( glm::radians( 18.0f ) ) );

        loadMatrixTarget( shaderProgram );

        glBindVertexArray( VAO );
        for( int i = 0; i < 10; i++ )
        {
            glm::mat4 modelMatrix( 1.0f );
            modelMatrix = glm::translate( modelMatrix, cubePositions[i] );
            float angle = 20.0f * i;
            modelMatrix = glm::rotate( modelMatrix, glm::radians( angle ), glm::vec3( 1.0f, 0.3f, 0.5f ) );

            shaderProgram.use();
            shaderProgram.setMat4( "modelMatrix", modelMatrix );

            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

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

void loadMatrixLight( ShaderProgram &shaderProgram )
{
    // 模型变换矩阵，glm0.9.9版本之后，注意一定要显式初始化为单位矩阵，很容易错误默认初始化为零矩阵。
    glm::mat4 modelMatrix( 1.0f );      // 显示坐标轴，不需要进行平移和旋转。

    pointLightPos = glm::vec3( 0.0f, 1.0f, 0.0f ) +
                    glm::vec3( 3 * sin( 2.0f * glfwGetTime() ),
                               0,
                               3 * cos( 2.0f * glfwGetTime() ) );
    modelMatrix = glm::translate( modelMatrix, pointLightPos );
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

// 构建第一张图的纹理
GLint loadTextureDiffuse( const ShaderProgram &shaderProgram )
{
    string projDir = projectDir();

    GLuint texture;
    glGenTextures( 1, &texture );

    glActiveTexture( GL_TEXTURE0 + 0 );         // 纹理数据由GL_TEXTURE0纹理单元管理
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );  // “X”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );  // “Y”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略

    GLint width, height, nrChannels;

    // 加载图片到内存中
    unsigned char *data = stbi_load( ( projDir + "\\res\\container2_diffuse.png" ).c_str(),    // 图片路径
                                     &width, &height,                                   // 图片宽高
                                     &nrChannels, 0 );                                  // 颜色通道数量
    if( data )
    {
        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,        // 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      GL_RGBA,              // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      GL_RGBA,              // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );  // 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free( data );                // 纹理数据已经上传到显存中，内存中的数据可以删除了。
    return  0;
}

// 构建第一张图的纹理
GLint loadTextureSpecular( const ShaderProgram &shaderProgram )
{
    string projDir = projectDir();

    GLuint texture;
    glGenTextures( 1, &texture );

    glActiveTexture( GL_TEXTURE0 + 1 );         // 纹理数据由GL_TEXTURE0纹理单元管理
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );  // “X”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );  // “Y”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略

    GLint width, height, nrChannels;

    // 加载图片到内存中
    //unsigned char *data = stbi_load( ( projDir + "\\res\\container2_specular_color.png" ).c_str(),    // 图片路径
    unsigned char *data = stbi_load( ( projDir + "\\res\\container2_specular.png" ).c_str(),    // 图片路径
                                     &width, &height,                                   // 图片宽高
                                     &nrChannels, 0 );                                  // 颜色通道数量

    if( data )
    {
        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,        // 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      GL_RGBA,              // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      GL_RGBA,              // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );  // 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free( data );                // 纹理数据已经上传到显存中，内存中的数据可以删除了。
    return 1;
}

void loadVertexDataObject( GLuint *VAO, GLuint *VBO )
{
    // 6个顶点（6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点）
    // 这里没有使用索引缓冲对象
    float vertices[] =
    {
        // 顶点坐标             // 法向量            // 纹理坐标
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glGenVertexArrays( 1, VAO );
    glBindVertexArray( *VAO );

    glGenBuffers( 1, VBO );
    glBindBuffer( GL_ARRAY_BUFFER, *VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0,                       // 顶点属性（3D坐标）位置值，指定vertex attribute的位置值，这个值对应在顶点着色器中的location=0的属性
                           3,                       // 顶点属性的分量数量（number of component）
                           GL_FLOAT,                // 分量数据类型（data type of component）
                           GL_FALSE,                // 数据是否需要被标准化
                           8 * sizeof( float ),     // 步长Stride，属性间隔，注意是两个属性值相同位置的间隔，不是首尾间隔。
                           ( void * ) 0 );          // 数据在缓冲中起始位置的偏移量

    glVertexAttribPointer( 1,                       // 顶点属性（法向量）位置值
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           8 * sizeof( float ),
                           ( void * )( 3 * sizeof( float ) ) );

    glVertexAttribPointer( 2,                       // 顶点属性（纹理坐标）位置值
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           8 * sizeof( float ),
                           ( void * )( 6 * sizeof( float ) ) );

    glEnableVertexAttribArray( 0 );     // 顶点属性配置默认是关闭的，要记得打开，0对应layout(location=0)顶点属性
    glEnableVertexAttribArray( 1 );     //
    glEnableVertexAttribArray( 2 );     //

    glBindVertexArray( 0 );
}