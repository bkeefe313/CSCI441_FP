#ifndef FP_STATICOBJECT_H
#define FP_STATICOBJECT_H


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <CSCI441/ShaderProgram.hpp>
#include "../include/CSCI441/ModelLoader.hpp"

class StaticObject {
public:
    /// \desc initialize a static object
    StaticObject(CSCI441::ModelLoader *model, glm::vec3 position, glm::vec3 scale = glm::vec3(1),
                 GLfloat rotation = 0) {
        _position = position;
        _scale = scale;
        _rotation = rotation;
        _axisOfRotation = glm::vec3(0, 1, 0);
        _modelMtx = glm::mat4(1);
        _model = model;
    }

    // Variables
    glm::vec3 _position;
    glm::vec3 _scale;
    GLfloat _rotation;
    glm::vec3 _axisOfRotation;
    glm::mat4 _modelMtx;
    CSCI441::ModelLoader *_model;

    /// \desc render the object
    void draw(CSCI441::ShaderProgram *shader) {
        _modelMtx = glm::translate(glm::mat4(1), _position);
        _modelMtx = glm::rotate(_modelMtx, _rotation, _axisOfRotation);
        _modelMtx = glm::scale(_modelMtx, _scale);

        _model->draw(shader->getShaderProgramHandle(), -1, -1, -1, -1, GL_TEXTURE0);
    }

    /// \desc rotates the object
    void updateRotation(float dTheta) {
        _rotation += dTheta;
        if (abs(_rotation) > M_PI * 2)
            _rotation = 0;
    }

    /// \desc returns the model matrix
    glm::mat4 getModelMatrix() {
        return _modelMtx;
    }
};


#endif //FP_STATICOBJECT_H