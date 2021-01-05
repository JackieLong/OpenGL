#include "Camera.h"

Camera::Camera( glm::vec3 position,
                glm::vec3 up,
                glm::vec3 target,
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
                float upX,  float upY, float  upZ,
                float targetX, float targetY, float targetZ,
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
        case CameraView::FIX:     // ������Ƕ�1������һ��������ӽ�
        {
            viewMatrix = glm::lookAt( mPos,            // �����λ��
                                      mTarget,         // Ŀ��λ�ã��������ע�ӵ㣩
                                      mVecWorldUp );           // ����ռ��������
            break;
        }
        case CameraView::ROTATE_Y:     // ������Ƕ�2����y����ת��������ӽ�
        {
            float radius = 10.0f;
            float camX = ( float ) sin( glfwGetTime() ) * radius;
            float camZ = ( float ) cos( glfwGetTime() ) * radius;
            viewMatrix = glm::lookAt( glm::vec3( camX, 0.0f, camZ ),    // ���������
                                      mTarget,                          // Ŀ��λ�ã��������ע�ӵ㣩
                                      mVecWorldUp );  // ����ռ��е�������
            break;
        }
        case CameraView::FREE:     //������Ƕ�3�������ƶ���������ӽ�
        {
            viewMatrix = glm::lookAt( mPos,
                                      mPos + mFront,          // �����һֱ����������ǰ��cameraFront���������һ��ϻ�������camerFrontһֱ�ڼ��㡣
                                      mVecWorldUp );
            //viewMatrix = glm::lookAt( mPos,
            //                          mFront,               // ������ƶ���ע�ӵ㲻�ı䣬�����һֱ�����ע�ӵ�
            //                          mUp );
            break;
        }
        case CameraView::FREE_LOOKAT:     //������Ƕ�3������lookat�������Լ��ֶ�����һ��LookAt���󣬴ﵽ��lookAt��������ա�
        {
            glm::vec3 z = glm::normalize( -mFront );               // ������ĳ���պ���Z����෴���򡣳���һֱ�ڼ��㡣
            glm::vec3 x = glm::normalize( glm::cross( mVecWorldUp, z ) );
            glm::vec3 y = glm::cross( z, x );

            glm::mat4 translateMatrix( 1.0f );      // ƽ�ƾ���ƽ�Ƶ������λ��
            glm::mat4 rotateMatrix( 1.0f );

            // ע�⣬OpenGL��������������[3][0]��ʾ�����е�һ�У�Ҫ�ر�ע�⡣
            translateMatrix[3][0] = -mPos.x;   // ʵ����ƽ�Ƴ�������ƽ�������������ȥ��ֵ
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

            // ��Ϊ����������󣬱任˳��Ҫ�������󿴡�����A*B*C*v����λ��v�Ƚ���C�任����B�任����C�任��
            viewMatrix = rotateMatrix * translateMatrix;
            break;
        }
        case FIX_XZ: //һ��������FPS�������Ҳ����˵���ܹ�������У�����ֻ�ܹ�����xzƽ���ϡ�
        {
            viewMatrix = glm::lookAt( mPos,
                                      mPos + mFront,          // �����һֱ����������ǰ��cameraFront���������һ��ϻ�������camerFrontһֱ�ڼ��㡣
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
