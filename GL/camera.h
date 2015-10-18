#include "stdgl.h"


class ModelCamera{
    // Two normalized vectors define direction.
    // Three component vector..  but it's normalized hence only two degrees of freedom
    glm::vec3 vLookAt;
    // an additional two components... note that not only is this normalized but it's also constrained to be perpendicular to lookAt. hence only one degree of freedom is offered by this component
    glm::vec3 vUp;

    // describes spatial point position of the camera.
    glm::vec3 vPos;

    glm::mat4 mView;
    // Additional control variables..
    // Handle rotation speed in degrees of rotation/screen space pixel.
    float fSpeedX;
    float fSpeedY;
    // Clamp to a radius around the center of the world.
    float fRadius;
    // Single vector denoting the line towards which the camera is to be oriented.
    // Used to recalculate vUp vector;
    glm::vec3 vOrientation;

    // UI Binder variables.
    GLFWwindow* glWindow;

    public:
    ModelCamera( GLFWwindow* );
    void setSpeedX( float );
    void setSpeedY( float );
    float getSpeedX();
    float getSpeedY();
    void setRadius( float );
    void setOrientation( glm::vec3 );
    void reset( glm::vec3 );
    glm::mat4 getViewMatrix();
    void glfwHandleCursor();

};
