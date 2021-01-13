#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window - system specific input methods

enum CameraView             // 摄像机视角
{
    FIX = 0,                // 固定视角
    ROTATE_Y,               // 绕Y轴旋转视角
    FREE,                   // 可自由移动/旋转的视角
    FREE_LOOKAT,            // 手动创建LookAt矩阵
    FIX_XZ
};

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  5.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles,
// Vectors and Matrices for use in OpenGL
class Camera
{
public:
    Camera();

    // constructor with vectors
    Camera( glm::vec3   position,
            glm::vec3   target,
            glm::vec3   up,
            float       yaw   = YAW,
            float       pitch = PITCH );

    // constructor with scalar values
    Camera( float posX,    float posY,    float posZ,
            float targetX, float targetY, float targetZ,
            float upX,     float upY,     float upZ,
            float yaw      = YAW,
            float pitch    = PITCH );

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix();
    float fov() const;
    glm::vec3 pos() const;
    glm::vec3 front()const;

    // processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard( GLFWwindow *window, float deltaTime );

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement( GLFWwindow *window, double xpos, double ypos );

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll( GLFWwindow *window, double xoffset, double yoffset );

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

public:
    //camera Attributes
    glm::vec3 mPos;
    glm::vec3 mFront;
    glm::vec3 mTarget;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mVecWorldUp;

    //euler Angles
    float     mYaw;
    float     mPitch;

    //camera options
    float     mMovSpeed;
    float     mMouseSensitivity;
    float     mFOV;

    CameraView mViewType;
};

#endif