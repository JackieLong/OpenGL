#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <direct.h>
#include "shader.h"
#include "stb_image.h"

using namespace std;

static std::string projectDir()
{
    std::string ret;
    char *tmpProjectDir = _getcwd( NULL, 0 );
    if( tmpProjectDir == nullptr )
    {
        perror( "getcwd error" );
        std::cout << "Failed to get project dir" << std::endl;
    }
    else
    {
        ret = tmpProjectDir;
        free( tmpProjectDir );
    }
    return ret;
}

static ShaderProgram loadShaderProgram( const std::string &vertexShaderPath,
                                        const std::string &fragmentShaderPath )
{
    std::string projDir = projectDir();
    return ShaderProgram( vertexShaderPath,     // 顶点着色器脚本
                          fragmentShaderPath ); // 片段着色器脚本
}


// 构建第一张图的纹理
static GLuint loadTexture( const std::string &path )
{
    GLuint texture = -1;
    glGenTextures( 1, &texture );

    glActiveTexture( GL_TEXTURE0 + 0 );         // 纹理数据由GL_TEXTURE0纹理单元管理
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );  // “X”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );  // “Y”轴方向上的Wrapping方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );     // 纹理缩小时，采用GL_LINEAR策略
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // 纹理放大时，采用GL_LINEAR策略

    GLint width, height, nrChannels;

    // 加载图片到内存中
    unsigned char *data = stbi_load( path.c_str(),          // 图片路径
                                     &width, &height,       // 图片宽高
                                     &nrChannels, 0 );      // 颜色通道数量
    if( data )
    {
        GLenum format;
        if( nrChannels == 1 )
        {
            format = GL_RED;
        }
        else
            if( nrChannels == 3 )
            {
                format = GL_RGB;
            }
            else
                if( nrChannels == 4 )
                {
                    format = GL_RGBA;
                }

        // 上传纹理数据到显存中，
        glTexImage2D( GL_TEXTURE_2D,        // 纹理数据又当前绑定的纹理单元管理
                      0,                    // mipmap level，0表示一级，如果有多级，需要挨个单独设置
                      format,              // 告诉 OpenGL 我们希望把纹理储存为何种格式
                      width, height,        // 纹理宽高
                      0,                    // 总为0，历史遗留问题
                      format,              // 图像像素的分量组成
                      GL_UNSIGNED_BYTE,     // 每个像素分量的大小
                      data );               // 内存中的图像数据

        glGenerateMipmap( GL_TEXTURE_2D );  // 动态生成多级纹理，有性能损耗，可以由外部工具预生成。
    }
    else
    {
        cout << "Failed to load texture::" << path << endl;
    }
    stbi_image_free( data );                // 纹理数据已经上传到显存中，内存中的数据可以删除了。

    return  texture;
}

#endif