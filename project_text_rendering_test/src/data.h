#ifndef __DATA_H__
#define __DATA_H__

#include "project_lib.h"

using namespace std;

const GLint ScreenWidth = 800;
const GLint ScreenHeight = 600;

//const string TTF_PATH = projectDir() + "\\res\\Antonio-Bold.ttf";       // FreeType字体
//const string TTF_PATH = projectDir() + "\\res\\Antonio-Light.ttf";      // FreeType字体
//const string TTF_PATH = projectDir() + "\\res\\Antonio-Regular.ttf";    // FreeType字体
const string TTF_PATH = projectDir() + "\\res\\arial.ttf";              // FreeType字体
//const string TTF_PATH = projectDir() + "\\res\\OCRAEXT.ttf";            // FreeType字体

const GLenum TEXTURE_CELL_TEXT = GL_TEXTURE0;                       // 绘制字符时，激活的纹理单元

std::vector<GLubyte> charSet;                                       // 支持字符集，

struct CharRenderInfo       // 绘制一个字符需要的数据：纹理、度量信息（指定绘制的矩形区域：大小、位置），度量相关知识：https://www.processon.com/view/link/5f75475207912906db11dd4c
{
    GLuint TextureID;       // 字形纹理ID
    glm::ivec2 Size;        // 字形大小
    glm::ivec2 Bearing;     // 从基准线到字形左部/顶部的偏移值
    GLuint Advance;         // 原点距下一个字形原点的距离
};

map<GLchar, CharRenderInfo> charDataMap;  // ChartData集

GLuint     VAO_quad, VBO_quad;      // 绘制一个字符的VAO、VBO，就是绘制一个矩形区域

Shader     shaderText;              // 绘制字符的着色器

glm::mat4  projectionMatrix;        // 投影矩阵：文字一般都是2D显示，也就只需要正交投影即可。

FT_Library freeTypeLib;             // FreeType库
FT_Face    face;                    // 一个face对应一种字体

void loadGlyphTexture( map<GLchar, CharRenderInfo> &charDataSet, FT_Face face, const std::vector<GLubyte> &charSet, const GLuint width, const GLuint height );
bool initFreeType( FT_Library &freeType );
bool createFace( const FT_Library &freeType, FT_Face &face, const std::string &ttfPath );
void createVAO_VBO( GLuint &VAO, GLuint &VBO );
void initCharSet( std::vector<GLubyte> &charSet );
void initTextShader();

void loadData()
{
    initCharSet( charSet );                     // 设置字符集，可能需要显示的所有字符

    initFreeType( freeTypeLib );                // 初始化FreeType库

    createFace( freeTypeLib, face, TTF_PATH );  // 创建字体

    loadGlyphTexture( charDataMap,
                      face,                     // 加载一套字符集的指定字体大小的字符纹理，
                      charSet,                  // 字符集
                      0, 48 );                  // 字体大小,0表示由高度来自动生成对应的宽度

    FT_Done_Face( face );                       // 字符纹理加载完毕，可以及时清理
    FT_Done_FreeType( freeTypeLib );            // FreeType库不再使用，及时清理

    createVAO_VBO( VAO_quad, VBO_quad );        // 创建绘制文字的VAO、VBO，包括文字的坐标、颜色

    initTextShader();
}

void initTextShader()
{
    // 渲染文字的着色器
    shaderText = Shader( projectDir() + "/src/shader/shader_vertex",
                         projectDir() + "/src/shader/shader_fragment" );

    shaderText.use();
    // 正交投影，文字一般就是2D显示。
    shaderText.setMat4( "projectionMatrix",
                        glm::ortho( 0.0f,                                       // 屏幕的左边
                                    static_cast< GLfloat >( ScreenWidth ),      // 屏幕的右边
                                    0.0f,                                       // 屏幕的底边
                                    static_cast< GLfloat >( ScreenHeight ) ) ); // 屏幕的顶部
    shaderText.setInt( "text", TEXTURE_CELL_TEXT - GL_TEXTURE0 );
}

void createVAO_VBO( GLuint &VAO_quad, GLuint &VBO_quad )
{
    //顶点数据格式
    //   {
    //       0.0f, 0.0f,       // 顶点坐标（坐标系由投影矩阵决定）
    //       0.0f, 1.0f        // 纹理坐标
    //   },
    const GLsizei vertex_Size = sizeof( GLfloat ) * 2 + sizeof( GLfloat ) * 2;
    glGenVertexArrays( 1, &VAO_quad );
    glGenBuffers( 1, &VBO_quad );
    glBindVertexArray( VAO_quad );
    glBindBuffer( GL_ARRAY_BUFFER, VBO_quad );
    glBufferData( GL_ARRAY_BUFFER,
                  vertex_Size * 6,              // 一个矩形2个三角形，6个顶点
                  NULL,                         // 开辟空的VBO
                  GL_DYNAMIC_DRAW );            // VBO的数据会频繁更新
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( GLfloat ), ( GLvoid * )( 0 * sizeof( GLfloat ) ) );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void initCharSet( std::vector<GLubyte> &charSet )
{
    // 字符集：这里以为ASCII字符集做简单示例，即只支持英文
    charSet.reserve( 2 << 7 );
    for( char tmpChar = 0; tmpChar < 127; tmpChar++ )
    {
        charSet.push_back( tmpChar );
    }
}

bool initFreeType( FT_Library &freeType )
{
    if( FT_Init_FreeType( &freeType ) )
    {
        cout << "ERROR::FREETYPE::failed to init FreeType Library." << endl;
        return false;
    }
    return true;
}

bool createFace( const FT_Library &freeType, FT_Face &face, const std::string &ttfPath )
{
    if( FT_New_Face( freeType, ttfPath.c_str(), 0, &face ) )
    {
        cout << "ERROR::FREETYPE::failed to load font(" << ttfPath.c_str() << ")" << endl;
        return false;
    }
    return true;
}

void loadGlyphTexture( map<GLchar, CharRenderInfo> &charDataMap,    // 存储字符纹理信息
                       FT_Face face,                                // 字体
                       const std::vector<GLubyte> &charSet,         // 字符集：生成这些字符的纹理
                       const GLuint nominalFontWidth,               // 字体大小
                       const GLuint nominalFontHeight )
{
    // 生成纹理之前，我们需要设置好字体大小，所以“为什么不要乱用字体大小？”
    // 注意这只是名义大小（nominal），别指望它是最终实际大小。
    // 一般我们可以设置width=0，字体的宽度会根据高度自动生成。
    FT_Set_Pixel_Sizes( face, nominalFontWidth, nominalFontHeight );

    // 字符的纹理我们是用的8 bit灰度图，因此一个像素就是1个byte，要取消内存对齐限制
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    for( const auto &_char : charSet )
    {
        if( FT_Load_Char( face, _char, FT_LOAD_RENDER ) )   // 激活_char字符的字形，这样可以获得该字符的度量信息
        {
            cout << "ERROR::FREETYPE::failed to load glyph(" << _char << ")" << endl;
            continue;
        }

        // *********************** 生成纹理
        GLuint texture;
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      GL_RED,                       // 8bit灰度图，在着色器中指定颜色
                      face->glyph->bitmap.width,    // 度量信息获取宽高
                      face->glyph->bitmap.rows,
                      0,
                      GL_RED,
                      GL_UNSIGNED_BYTE,
                      face->glyph->bitmap.buffer );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );  // 环绕方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );    // 纹理缩小采样
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );    // 纹理放大采样
        glBindTexture( GL_TEXTURE_2D, 0 );

        charDataMap.emplace( _char, CharRenderInfo
        {
            texture,
            glm::ivec2( face->glyph->bitmap.width,
                        face->glyph->bitmap.rows ),
            glm::ivec2( face->glyph->bitmap_left,
                        face->glyph->bitmap_top ),
            static_cast<GLuint>( face->glyph->advance.x )
        } );
    }
}

void renderText( const Shader    &shader,   // 文本渲染着色器
                 const string    &text,     // 文本内容
                 const glm::vec2 &pos,      // 位置
                 const GLfloat   &scale,    // 缩放
                 const glm::vec3 &color )   // 文本颜色
{
    GLfloat tmp_x = pos.x;

    shader.use();
    shader.setVec3( "textColor", color );

    glActiveTexture( TEXTURE_CELL_TEXT );
    glBindVertexArray( VAO_quad );
    for( const auto &_char : text )
    {
        // 获取渲染_char字符所需要的信息
        CharRenderInfo character = charDataMap[_char];

        // 字符纹理
        glBindTexture( GL_TEXTURE_2D, character.TextureID );

        // 纹理矩形区域左下角的位置
        GLfloat   xpos      = tmp_x + character.Bearing.x * scale;
        GLfloat   ypos      = pos.y - ( character.Size.y - character.Bearing.y ) * scale;

        // 矩形区域的宽高
        GLfloat   width     = character.Size.x * scale;
        GLfloat   height    = character.Size.y * scale;

        GLfloat tmpvertices[ 6 * 4] =
        {
            // 三角形1
            xpos,         ypos + height,  0.0f, 0.0f,   // 左上角
            xpos,         ypos,           0.0f, 1.0f,   // 左下角
            xpos + width, ypos,           1.0f, 1.0f,   // 右下角

            // 三角形2
            xpos,         ypos + height,  0.0f, 0.0f,   // 左上角
            xpos + width, ypos,           1.0f, 1.0f,   // 右下角
            xpos + width, ypos + height,  1.0f, 0.0f    // 右上角
        };
        glBindBuffer( GL_ARRAY_BUFFER, VBO_quad );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( tmpvertices ), tmpvertices );  // 更新VBO数据
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        glDrawArrays( GL_TRIANGLES, 0, 6 );
        tmp_x += ( character.Advance >> 6 ) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray( 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

#endif