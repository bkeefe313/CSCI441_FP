//
// Created by Ben Keefe on 11/30/22.
//

#ifndef LAB04_ENEMY_H
#define LAB04_ENEMY_H


#include <CSCI441/ShaderProgram.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../include/CSCI441/ModelLoader.hpp"
#include <iostream>

class Enemy {
public:
    Enemy(CSCI441::ModelLoader* model, glm::vec3 position, glm::vec3 scale = glm::vec3(1)) {
        _model = model;
        _position = position;

        _scale = scale;
        _heading = glm::vec3(0,0,1);
        _health = 100;
        _maxHealth = 100;
        _speed = 0.3f;
        _targetPosition = glm::vec3(0);
        _attackDmg = 1;
        _rotation = 0;
        _axisOfRotation = glm::vec3(0, 1, 0);
        _offset = glm::vec3(0, 1, 0);
    }

    /// spatial information
    glm::vec3 _position;
    glm::vec3 _scale;
    glm::vec3 _heading;

    /// data
    float _health;
    float _maxHealth;
    glm::vec3 _targetPosition;
    float _attackDmg;
    float _speed;

    glm::mat4 getModelMatrix() {

        _modelMtx = glm::translate(glm::mat4(1), _position + _offset);
        _modelMtx = glm::rotate(_modelMtx, _rotation, _axisOfRotation);
        _modelMtx = glm::scale(_modelMtx, _scale);

        return _modelMtx;
    }

    void draw(CSCI441::ShaderProgram* shader) {
        _offset = glm::vec3(0, 1.5f, 0) * _scale.y;
        _model->draw(shader->getShaderProgramHandle());
    }

    void calculateTrajectory(glm::vec3 target) {
        _targetHeading = glm::normalize(glm::vec3((target - _position).x, 0, (target - _position).z));
        _targetRotation = acos(glm::dot(_initHeading, _targetHeading));
    }

    void move() {
        if(_rotation != _targetRotation)
            _rotation += 0.01f*(_targetRotation - _rotation);

        _axisOfRotation = glm::cross(_initHeading, _targetHeading);

        if(_axisOfRotation.y > 0)
            _heading = glm::vec3(sin(_rotation), 0, cos(_rotation));
        else if(_axisOfRotation.y < 0)
            _heading = glm::vec3(-sin(_rotation), 0, cos(_rotation));

        _position += _speed * _heading;
    }

    void handleCollision(Enemy* other) {
        this->_scale += glm::log(other->_scale);
        this->_maxHealth += 100;
        this->_attackDmg += 1;
        this->_speed *= 0.9;

        delete(other);
    }

private:
    CSCI441::ModelLoader* _model;
    GLfloat _rotation;
    glm::vec3 _axisOfRotation;
    glm::mat4 _modelMtx;
    glm::vec3 _targetHeading;
    GLfloat _targetRotation;
    glm::vec3 _offset;

    glm::vec3 _initHeading = glm::vec3(0, 0, 1);
};


#endif //LAB04_ENEMY_H
