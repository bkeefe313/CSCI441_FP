#ifndef FP_PLAYER_H
#define FP_PLAYER_H


#include <glm/ext/scalar_constants.hpp>
#include <CSCI441/ShaderProgram.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <CSCI441/objects.hpp>
#include <CSCI441/ModelLoader.hpp>


/// \desc essntially an object that is controlled by the user, contains all necessary data to render and move around
class Player {
public:
    // Variables
    CSCI441::ModelLoader *_model;
    CSCI441::ModelLoader *_limbs;
    glm::vec3 _position;
    glm::vec3 _forward;
    GLfloat _angle;
    GLfloat _limbAngle;
    bool _falling;
    GLfloat _walkSpeed;
    GLfloat _strafeSpeed;

    Player() {
        //instantiate model
        _model = new CSCI441::ModelLoader();
        _limbs = new CSCI441::ModelLoader();
        _position = glm::vec3(0);
        _forward = glm::vec3(0, 0, 1);
        _strafeSpeed = 0;
        _walkSpeed = 0;
        _falling = false;
        _modelMtx = glm::mat4(1);
        _axisOfRotation = glm::vec3(0, 1, 0);
    }

    /// \desc load model data and set attribute locations
    void initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
        _model->loadModelFile("assets/saul.obj");
        _model->setAttributeLocations(posAttr, normAttr, texAttr);
        _limbs->loadModelFile("assets/limbs.obj");
        _limbs->setAttributeLocations(posAttr, normAttr, texAttr);
    }

    static int dirOfCross(glm::vec3 a, glm::vec3 b) {
        return glm::cross(a, b).y > 0 ? 1 : -1;
    }

    /// \desc move player based on current strafe and walk speed
    void updatePosition() {
        // If the player is falling, update their position and rotation
        if (_falling) {
            _position += glm::vec3(0, -0.5, 0);
            _angle += 0.05f;
            _axisOfRotation = glm::vec3(rand(), rand(), rand());
            return;
        }
        glm::vec3 posZ = glm::vec3(0, 0, 1);
        if (abs(_strafeSpeed) > 0 || abs(_walkSpeed) > 0) {
            _angle = (-(float) dirOfCross(_forward, posZ)) * acos(glm::dot(_forward, posZ));
            _limbAngle += 0.01f;
            if (_limbAngle >= 6.28) {
                _limbAngle = 0;
            }
        } else {
            _limbAngle = 0;
        }
        _position += (_strafeSpeed * (glm::cross(_forward, glm::vec3(0, 1, 0)))) + (_walkSpeed * _forward);
    }

    /// \desc change the movement direction from camera position
    void updateDirection(glm::vec3 v) {
        _forward = _position - v;
        _forward.y = 0;
        _forward = glm::normalize(_forward);
    }

    /// \desc Falling player
    void fallOffEdge() {
        _falling = true;
    }

    /// \desc render the player
    void draw(CSCI441::ShaderProgram *shader) {
        shader->useProgram();
        CSCI441::setVertexAttributeLocations(shader->getAttributeLocation("vPos"),
                                             shader->getAttributeLocation("vNormal"),
                                             shader->getAttributeLocation("vTexCoord"));

        _model->draw(shader->getShaderProgramHandle(), -1, -1, -1, -1, GL_TEXTURE0);

        if(abs(_walkSpeed) + abs(_strafeSpeed) > 0) {
            _modelMtx = glm::rotate(_modelMtx, 0.5f*(float)sin(glfwGetTime()), glm::cross(_axisOfRotation, glm::vec3(0, 0, 1)));
            shader->setProgramUniform("modelMtx", _modelMtx);
        }

        _limbs->draw(shader->getShaderProgramHandle(), -1, -1, -1, -1, GL_TEXTURE0);
    }

    /// \desc get the model matrix for the player
    glm::mat4 getModelMatrix() {
        _modelMtx = glm::translate(glm::mat4(1), _position);
        _modelMtx = glm::rotate(_modelMtx, _angle, _axisOfRotation);
        _modelMtx = glm::rotate(_modelMtx, -glm::pi<GLfloat>() / 2, glm::vec3(1, 0, 0));
        return _modelMtx;
    }

private:
    glm::mat4 _modelMtx;
    glm::vec3 _axisOfRotation;
};


#endif //FP_PLAYER_H
