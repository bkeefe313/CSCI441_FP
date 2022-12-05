//
// Created by Ben Keefe on 10/6/22.
//

#ifndef LAB04_ARCBALLCAM_H
#define LAB04_ARCBALLCAM_H

#include <CSCI441/Camera.hpp>
#include "Player.h"

class ArcballCam: public CSCI441::Camera {
public:
    Player* _target;
    GLfloat _zoomSpeed;

    ArcballCam() : Camera(){
        _zoomSpeed = 0;
    }

    /// \desc place camera in cartesian worldspace based on its spherical coordinates
    void recomputeOrientation() {
        float x, y, z;
        x = _radius*cos(_theta)*sin(_phi);
        z = _radius*sin(_theta)*sin(_phi);
        y = _radius*cos(_phi);

        setPosition(glm::vec3(x, y, z) + _target->_position);
        setLookAtPoint(_target->_position + glm::vec3(0, 10, 0)
                        + 3.0f*glm::normalize(glm::cross(_target->_forward, glm::vec3(0, 1, 0))));
        computeViewMatrix();
    }

    void moveForward(GLfloat movementFactor) {
        //do nothing. this camera is tied to player position and doesn't translate on its own
    }
    void moveBackward(GLfloat movementFactor) {
        //do nothing. this camera is tied to player position and doesn't translate on its own
    }

    /// \desc zoom camera by updating spherical radius coord from current zoom speed
    void updateZoom() {
        _radius += _zoomSpeed;
        //clamp radius
        if(_radius < 1) _radius = 1;
        if(_radius > 100) _radius = 100;
    }

    /// \desc call to change current zoom speed of camera
    void zoom(GLfloat speed) {
        _zoomSpeed = speed;
    }
};


#endif //LAB04_ARCBALLCAM_H
