#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <functional>
#include <direct.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 项目目录，如D:/project/fuck
std::string projectDir();

// 加载纹理到GPU
GLuint createTexture( const std::string &path,                            // 纹理路径
                      std::function<void()> paramCallback = nullptr );    // 自定义设置纹理参数

// 加载顶点数据到GPU缓冲中
void createVertexBuffer( const GLfloat *vertices,               // 顶点数组
                         const int &len,                        // 顶点数组长度（字节数
                         const std::string &components,         // 顶点数据分量组成，如“332”，表示3个分量，2个3float，一个2float
                         GLuint *VAO,                           // VAO
                         GLuint *VBO,                           // VBO
                         const GLuint *indices = nullptr,       // 顶点索引数组
                         const int &lenIndices = 0,             // 顶点索引数组长度（字节数）
                         GLuint *EBO = nullptr );               // Element Buffer Object

// 初始化GLFW库
void initGLFW();

// load all OpenGL function pointers
bool initGLAD();

// 创建窗口
GLFWwindow *createWindow( const int width,                                      // 屏幕宽高
                          const int height,
                          const std::string &windowName,                        // 窗口标题
                          GLFWcursorposfun mouse_move_callback = nullptr,       // 鼠标移动回调
                          GLFWscrollfun mouse_scroll_callback  = nullptr );     // 鼠标滚轮回调

#endif

