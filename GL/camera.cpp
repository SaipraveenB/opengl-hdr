#include "camera.h"
// model camera written for GLFW.
// Automatically captures input from a GLFW service.
    // Two normalized vectors define direction.
    ModelCamera::ModelCamera( GLFWwindow* window ):glWindow(window){
        glfwSetInputMode( window, GLFW_CURSOR ,GLFW_CURSOR_DISABLED );
    }

    void ModelCamera::setSpeedX( float speed ){
        fSpeedX = speed;
    }
    
    void ModelCamera::setSpeedY( float speed ){
        fSpeedY = speed;
    }

    float ModelCamera::getSpeedX(){
        return fSpeedX;
    }

    float ModelCamera::getSpeedY(){
        return fSpeedY;
    }

    void ModelCamera::setRadius( float f ){
        fRadius = f;
    }

    void ModelCamera::setOrientation( glm::vec3 vec ){
        vOrientation = vec;
    }

    glm::mat4 ModelCamera::getViewMatrix(){
        return mView;
    }

    void ModelCamera::reset( glm::vec3 vDir ){
        vPos = glm::normalize( vDir ) * fRadius;

        vLookAt = glm::normalize( -vPos );

        // Do norm of lookAt x ( lookAt x orientation_base_vector ) to get the new Up vector.
        vUp = glm::normalize( glm::cross( vLookAt, glm::cross( vLookAt, vOrientation ) ) );

        mView = glm::lookAt( vPos, vLookAt, vUp );
    }

    void ModelCamera::glfwHandleCursor(){
        double x,y;
        glfwGetCursorPos( glWindow, &x, &y );
        glfwSetCursorPos( glWindow, 0, 0 );

        // Scale components to obtain degree space projections.
        double degX = fSpeedX * x;
        double degY = fSpeedY * y;

        // vUp is used as axis for rotating the point about virtual X
        // vUp x vLookAt isusedas pivot axis for virtual Y.

        // First compute the new differentially modified point.
        glm::vec3 newPos = vPos;
        newPos = glm::rotate( newPos, static_cast<float>(degX), vUp );
        newPos = glm::rotate( newPos, static_cast<float>(degY), glm::cross( vUp, vLookAt ) );
        vPos = newPos;

        // Now compute vUp and vLookAt.

        // just the inverse of the current position vector.
        vLookAt = glm::normalize( -vPos );

        // Do norm of lookAt x ( lookAt x orientation_base_vector ) to get the new Up vector.
        vUp = glm::normalize( glm::cross( vLookAt, glm::cross( vLookAt, vOrientation ) ) );

        mView = glm::lookAt( vPos, vLookAt, vUp );
    }
