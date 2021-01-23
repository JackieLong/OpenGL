#include "data.h"
#include "test_glGetError.h"
#include "test_debugOutput.h"
#include "test_debugOutputTexture.h"


int main()
{
    //test_glGetError();        // 调试方法一
    //test_debugOutput();       // 调试方法二
    test_debugOutputTexture();  // 调试方法三

    return 0;
}