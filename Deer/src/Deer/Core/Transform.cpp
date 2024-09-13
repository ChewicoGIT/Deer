#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Deer {
    glm::mat4 Transform::getWorldMatrix()
    {
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 roatationMat = glm::mat4(rotation);
        glm::mat4 positionMat = glm::translate(glm::mat4(1.0f), position);

        return positionMat * roatationMat * scaleMat;
    }
}
