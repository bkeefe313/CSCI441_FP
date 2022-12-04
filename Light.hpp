//
// Created by Ben Keefe on 10/10/22.
//

#ifndef A3_LIGHT_HPP
#define A3_LIGHT_HPP

#include <glm/vec3.hpp>
#include <GL/glew.h>

enum LightType {
    POINT,
    DIRECTIONAL,
    SPOTLIGHT
};

class Light {
public:
    /// \desc initialize a point or directional light, based on type parameter
    /// \param posOrDir where the light is located in world space, or if making a directional light, the direction it is cast
    /// \param color the light's color in XYZ = RGB
    /// \param type the type of light. use POINT for point lights and DIRECTIONAL for directional lights
    Light(glm::vec3 posOrDir, glm::vec3 color, LightType type) {
        if(type == DIRECTIONAL) {
            _direction = posOrDir;
        } else {
            _position = posOrDir;
        }
        _type = type;
        _color = color;
    }

    /// \desc initialize a point light
    /// \param position where the light is located in world space
    /// \param color the light's color in XYZ = RGB
    Light( glm::vec3 position, glm::vec3 color) {
        _type = POINT;
        _position = position;
        _color = color;
    }

    /// \desc initialize a spotlight
    /// \param position where the light is located in world space
    /// \param direction the vector specifying which direction the light is cast from its position
    /// \param angle the cutoff angle of the spotlight
    /// \param color the light's color in XYZ = RGB
    Light(glm::vec3 position, glm::vec3 direction, GLfloat angle, glm::vec3 color) {
        _type = SPOTLIGHT;
        _position = position;
        _direction = direction;
        _angle = angle;
        _color = color;
    }

    /// \desc returns position of point or spotlight. DO NOT USE ON DIRECTIONAL LIGHT.
    glm::vec3 getPosition() {
        if(_type != DIRECTIONAL)
            return _position;
    }

    /// \desc returns cutoff angle of spotlight. DO NOT USE ON DIRECTIONAL OR POINT LIGHT.
    GLfloat getAngle() {
        if(_type == SPOTLIGHT)
            return _angle;
    }

    /// \desc returns direction of directional or spotlight. DO NOT USE ON POINT LIGHT.
    glm::vec3 getDirection() {
        if(_type != POINT)
            return _direction;
    }

    /// \desc returns color of a light
    glm::vec3 getColor() {
        return _color;
    }

private:
    LightType _type;
    glm::vec3 _position;
    glm::vec3 _direction;
    glm::vec3 _color;
    GLfloat _angle;
};


#endif //A3_LIGHT_HPP
