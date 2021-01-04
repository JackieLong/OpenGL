#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <direct.h>
#include "shader.h"

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


#endif