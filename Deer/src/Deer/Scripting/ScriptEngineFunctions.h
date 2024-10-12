#pragma once
#include <string>
#include "glm/glm.hpp"
#include "Deer/Core/Core.h"

struct asSMessageInfo;
class asIScriptEngine;

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param);
    void print(std::string& msg);
    
    // Entity Transformations -------------
    glm::vec3 getEntityPosition(uid& entityUID);
    void setEntityPosition(glm::vec3 position, uid& entityUID);
    glm::vec3 getEntityScale(uid& entityUID);
    void setEntityScale(glm::vec3 scale, uid& entityUID);
    // Entity Transformations -------------

    void registerVec3(asIScriptEngine* engine);
    void registerEntity(asIScriptEngine* engine);

    void registerDeerFunctions(asIScriptEngine* scriptEngine);
    void registerEntityTransformFunctions(asIScriptEngine* scriptEngine);
}