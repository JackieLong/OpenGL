#include "Camera.h"

Camera::Camera( glm::vec3 position,
                glm::vec3 target,
                glm::vec3 up,
                float     yaw,
                float     pitch )
    : mFront( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
      mMovSpeed( SPEED ),
      mMouseSensitivity( SENSITIVITY ),
      mFOV( ZOOM ),
      mViewType( CameraView::FREE )
{
    mPos        = position;
    mTarget     = target;
    mVecWorldUp = up;
    mYaw        = yaw;
    mPitch      = pitch;
    updateCameraVectors();
}

Camera::Camera( float posX, float posY, float posZ,
                float targetX, float targetY, float targetZ,
                float upX,  float upY, float  upZ,
                float yaw, float pitch )
    : mFront( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
      mMovSpeed( SPEED ),
      mMouseSensitivity( SENSITIVITY ),
      mFOV( ZOOM ),
      mViewType( CameraView::FREE )
{
    mPos         = glm::vec3( posX, posY, posZ );
    mTarget      = glm::vec3( targetX, targetY, targetZ );
    mVecWorldUp  = glm::vec3( upX, upY, upZ );
    mYaw         = yaw;
    mPitch       = pitch;
    updateCameraVectors();
}

Camera::Camera()
    : mViewType( CameraView::FREE )
{
}

glm::mat4 Camera::getViewMatrix()
{
    glm::mat4 viewMatrix( 1.0f );

    switch( mViewType )
    {
        case CameraView::FIX:     // 摄像机角度1：设置一个摄像机视角
        {
            viewMatrix = glm::lookAt( mPos,            // 摄像机位置
                                      mTarget,         // 目标位置（摄像机的注视点）
                                      mVecWorldUp );           // 世界空间的上向量
            break;
        }
        case CameraView::ROTATE_Y:     // 摄像机角度2：绕y轴旋转的摄像机视角
        {
            float radius = 10.0f;
            float camX = ( float ) sin( glfwGetTime() ) * radius;
            float camZ = ( float ) cos( glfwGetTime() ) * radius;
            viewMatrix = glm::lookAt( glm::vec3( camX, 0.0f, camZ ),    // 摄像机坐标
                                      mTarget,                          // 目标位置（摄像机的注视点）
                                      mVecWorldUp );  // 世界空间中的上向量
            break;
        }
        case CameraView::FREE:     //摄像机角度3：自由移动的摄像机视角
        {
            viewMatrix = glm::lookAt( mPos,
                                      mPos + mFront,          // 摄像机一直朝向它的正前方cameraFront，这好像是一句废话，但是camerFront一直在计算。
                                      mVecWorldUp );
            //viewMatrix = glm::lookAt( mPos,
            //                          mFront,               // 摄像机移动，注视点不改变，摄像机一直朝向该注视点
            //                          mUp );
            break;
        }
        case CameraView::FREE_LOOKAT:     //摄像机角度3：不用lookat函数，自己手动创建一个LookAt矩阵，达到对lookAt矩阵的掌握。
        {
            glm::vec3 z = glm::normalize( -mFront );               // 摄像机的朝向刚好是Z轴的相反方向。朝向一直在计算。
            glm::vec3 x = glm::normalize( glm::cross( mVecWorldUp, z ) );
            glm::vec3 y = glm::cross( z, x );

            glm::mat4 translateMatrix( 1.0f );      // 平移矩阵，平移到摄像机位置
            glm::mat4 rotateMatrix( 1.0f );

            // 注意，OpenGL采用列主序，所以[3][0]表示第三列第一行，要特别注意。
            translateMatrix[3][0] = -mPos.x;   // 实际是平移场景不是平移摄像机，所以去负值
            translateMatrix[3][1] = -mPos.y;
            translateMatrix[3][2] = -mPos.z;

            rotateMatrix[0][0] = x.x;
            rotateMatrix[1][0] = x.y;
            rotateMatrix[2][0] = x.z;

            rotateMatrix[0][1] = y.x;
            rotateMatrix[1][1] = y.y;
            rotateMatrix[2][1] = y.z;

            rotateMatrix[0][2] = z.x;
            rotateMatrix[1][2] = z.y;
            rotateMatrix[2][2] = z.z;

            // 因为是列主序矩阵，变换顺序要从右往左看。比如A*B*C*v，对位置v先进行C变换，再B变换，再C变换。
            viewMatrix = rotateMatrix * translateMatrix;
            break;
        }
        case FIX_XZ: //一个真正的FPS摄像机（也就是说不能够随意飞行），你只能够呆在xz平面上。
        {
            viewMatrix = glm::lookAt( mPos,
                                      mPos + mFront,          // 摄像机一直朝向它的正前方cameraFront，这好像是一句废话，但是camerFront一直在计算。
                                      mVecWorldUp );
            break;
        }
        default:
            break;
    }

    return viewMatrix;
}

float Camera::fov() const
{
    return mFOV;
}

glm::vec3 Camera::pos() const
{
    return mPos;
}

void Camera::processKeyboard( GLFWwindow *window, float deltaTime )
{
    if( mViewType == CameraView::FIX )
    {
        return;
    }

    float velocity = mMovSpeed * deltaTime;

    if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
    {
        mPos += mFront * velocity;
    }
    if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
    {
        mPos -= mFront * velocity;
    }
    if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
    {
        mPos -= mRight * velocity;
    }
    if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
    {
        mPos += mRight * velocity;
    }
    if( glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS )
    {
        mPos += glm::vec3( 0.0f, 1.0f, 0.0f ) * velocity;
    }
    if( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS )
    {
        mPos += glm::vec3( 0.0f, -1.0f, 0.0f ) * velocity;
    }
}

void Camera::processMouseMovement( float xoffset, float yoffset, GLboolean constrainPitch /*= true */ )
{
    if( mViewType != CameraView::FREE && mViewType != CameraView::FIX_XZ && mViewType != CameraView::FREE_LOOKAT )
    {
        return;
    }
    xoffset *= mMouseSensitivity;
    yoffset *= mMouseSensitivity;

    mYaw += xoffset;
    mPitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if( constrainPitch )
    {
        if( mPitch > 89.0f )
        {
            mPitch = 89.0f;
        }
        if( mPitch < -89.0f )
        {
            mPitch = -89.0f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll( GLFWwindow *window, double xoffset, double yoffset )
{
    mFOV -= ( float ) yoffset;
    if( mFOV < 1.0f )
    {
        mFOV = 1.0f;
    }
    if( mFOV > 45.0f )
    {
        mFOV = 45.0f;
    }
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos( glm::radians( mYaw ) ) * cos( glm::radians( mPitch ) );
    front.y = sin( glm::radians( mPitch ) );
    front.z = sin( glm::radians( mYaw ) ) * cos( glm::radians( mPitch ) );

    // normalize the vectors, because their length gets closer to 0 the more you look up or down
    // which results in slower movement.
    mFront   = glm::normalize( front );

    // also re-calculate the Right and Up vector
    mRight   = glm::normalize( glm::cross( mFront, mVecWorldUp ) );
    mUp      = glm::normalize( glm::cross( mRight, mFront ) );

}
