#ifndef FP_ENEMY_H
#define FP_ENEMY_H


#include <CSCI441/ShaderProgram.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../include/CSCI441/ModelLoader.hpp"
#include <iostream>

class Enemy {
public:
    // Initialize the enemy with a model, position, and scale
    Enemy(CSCI441::ModelLoader *model, glm::vec3 position, glm::vec3 scale = glm::vec3(1)) {
        _model = model;
        _position = position;

        _scale = scale;
        _heading = glm::vec3(0, 0, 1);
        _speed = 0.2f;
        _targetPosition = glm::vec3(0);
        _rotation = 0;
        _axisOfRotation = glm::vec3(0, 1, 0);
        _offset = glm::vec3(0, 1, 0);
    }

    /// spatial information
    glm::vec3 _position;
    glm::vec3 _scale;
    glm::vec3 _heading;

    /// data
    glm::vec3 _targetPosition;
    float _speed;

    glm::mat4 getModelMatrix() {
        _modelMtx = glm::translate(glm::mat4(1), _position + _offset);
        _modelMtx = glm::rotate(_modelMtx, _rotation, _axisOfRotation);
        _modelMtx = glm::scale(_modelMtx, _scale);

        return _modelMtx;
    }

    void draw(CSCI441::ShaderProgram *shader) {
        // Makes the enemy float
        _offset = glm::vec3(0, 1.5f, 0) * _scale.y;
        _model->draw(shader->getShaderProgramHandle(), -1, -1, -1, -1, GL_TEXTURE0);
    }

    void calculateTrajectory(glm::vec3 target) {
        // Calculate the heading and rotation to the target
        _targetHeading = glm::normalize(glm::vec3((target - _position).x, 0, (target - _position).z));
        _targetRotation = acos(glm::dot(_initHeading, _targetHeading));
    }

    void move() {
        // rotates enemy to face player
        _rotation = _targetRotation;

        _axisOfRotation = glm::cross(_initHeading, _targetHeading);

        if (_axisOfRotation.y > 0)
            _heading = glm::vec3(sin(_rotation), 0, cos(_rotation));
        else if (_axisOfRotation.y < 0)
            _heading = glm::vec3(-sin(_rotation), 0, cos(_rotation));

        // moves enemy towards player
        _position += _speed * _heading;
    }

private:
    CSCI441::ModelLoader *_model;
    GLfloat _rotation;
    glm::vec3 _axisOfRotation;
    glm::mat4 _modelMtx;
    glm::vec3 _targetHeading;
    GLfloat _targetRotation;
    glm::vec3 _offset;

    glm::vec3 _initHeading = glm::vec3(0, 0, 1);
};


#endif //FP_ENEMY_H
