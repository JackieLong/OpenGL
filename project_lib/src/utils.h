#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <functional>
#include <direct.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

float randomFloat();

// 项目目录，如D:/project/fuck
std::string projectDir();

// 加载纹理到GPU
GLuint createTexture( const std::string &path,                            // 纹理路径
                      std::function<void()> paramCallback = nullptr );    // 自定义设置纹理参数

GLuint createTextureCubemap( const std::vector<std::string> &path,
                             std::function<void()> paramCallback = nullptr );

// 加载顶点数据到GPU缓冲中
void createVertexBuffer( const GLfloat *vertices,               // 顶点数组
                         const int &len,                        // 顶点数组长度（字节数
                         const std::string &components,         // 顶点数据分量组成，如“332”，表示3个分量，2个3float，一个2float
                         GLuint *VAO,                           // VAO
                         GLuint *VBO,                           // VBO
                         const GLuint *indices = nullptr,       // 顶点索引数组
                         const int &lenIndices = 0,             // 顶点索引数组长度（字节数）
                         GLuint *EBO = nullptr );               // Element Buffer Object

void updateVertexBuffer( const GLuint *VAO,             // VAO
                         const GLfloat *vertices,       // 数据来源，交错布局方式，这里限制了都是float类型数组
                         const int &len,                // 数据字节数
                         const std::string &components, // 数据属性的分量组成，如“332”表示有三个顶点属性，第一、二个属性有3个分量、第三个有2个分量。
                         GLuint *VBO,                   // 创建的VBO保存地址
                         const GLuint startLocation = 0,    // 起始属性位置值
                         const GLuint *indices = nullptr,   // 不为空则表示要创建EBO
                         const int &lenIndices = 0,         // EBO的字节数
                         GLuint *EBO = nullptr,             // 创建的EBO的保存地址
                         std::function<void()> callback = nullptr );

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

