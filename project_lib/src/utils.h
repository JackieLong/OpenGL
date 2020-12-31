#include <iostream>
#include <direct.h>

std::string projectDir()
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