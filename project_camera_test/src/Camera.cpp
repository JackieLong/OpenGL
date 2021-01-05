#include "Camera.h"

Camera::Camera( glm::vec3 position  /*= glm::vec3( 0.0f, 0.0f, 0.0f )*/,
                glm::vec3 up        /*= glm::vec3( 0.0f, 1.0f, 0.0f )*/,
                float     yaw       /*= YAW*/,
                float     pitch     /*= PITCH */ )
    : mFront( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
      mMovSpeed( SPEED ),
      mMouseSensitivity( SENSITIVITY ),
      mFOV( ZOOM )
{
    mPos = position;
    mVecWorldUp = up;
    mYaw = yaw;
    mPitch = pitch;
    updateCameraVectors();
}

Camera::Camera( float posX, float posY, float posZ,
                float upX,  float upY, float  upZ,
                float yaw, float pitch )
    : mFront( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
      mMovSpeed( SPEED ),
      mMouseSensitivity( SENSITIVITY ),
      mFOV( ZOOM )
{
    mPos = glm::vec3( posX, posY, posZ );
    mVecWorldUp  = glm::vec3( upX, upY, upZ );
    mYaw      = yaw;
    mPitch    = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt( mPos,
                        mPos + mFront,
                        mUp );
}

void Camera::processKeyboard( Camera_Movement direction, float deltaTime )
{
    float velocity = mMovSpeed * deltaTime;
    if( direction == FORWARD )
    {
        mPos += mFront * velocity;
    }
    if( direction == BACKWARD )
    {
        mPos -= mFront * velocity;
    }
    if( direction == LEFT )
    {
        mPos -= mRight * velocity;
    }
    if( direction == RIGHT )
    {
        mPos += mRight * velocity;
    }
}

void Camera::processMouseMovement( float xoffset, float yoffset, GLboolean constrainPitch /*= true */ )
{
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

void Camera::processMouseScroll( float yoffset )
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