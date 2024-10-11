#pragma once
#include <string>
#include "glm/glm.hpp"
#include "Deer/Core/Core.h"

struct asSMessageInfo;
class asIScriptEngine;

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param);
    void print(std::string& msg);

    glm::vec3 getEntityPosition(uid entityUID);
    void setEntityPosition(glm::vec3 position, uid entityUID);

    void registerVec3(asIScriptEngine* engine);
    void registerDeerFunctions(asIScriptEngine* scriptEngine);
    void registerEntityTransformFunctions(asIScriptEngine* scriptEngine);
}