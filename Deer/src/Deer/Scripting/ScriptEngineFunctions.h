#pragma once


#include <string>
#include "glm/glm.hpp"

struct asSMessageInfo;
class asIScriptEngine;

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param);
    void print(std::string& msg);
    
    // Entity Transformations -------------
    glm::vec3 getEntityPosition(uint32_t& entityUID);
    void setEntityPosition(glm::vec3 position, uint32_t& entityUID);
    glm::vec3 getEntityScale(uint32_t& entityUID);
    void setEntityScale(glm::vec3 scale, uint32_t& entityUID);
    // Entity Transformations -------------

    // Entity Relationship ------------
    uint32_t getEntityParent(uint32_t& entityUID);
    bool isEntityValid(uint32_t& entityUID);
    // Entity Relationship ------------

    void registerVec3(asIScriptEngine* engine);
    void registerEntity(asIScriptEngine* engine);

    void registerDeerFunctions(asIScriptEngine* scriptEngine);
    void registerInputFunctions(asIScriptEngine* scriptEngine);
    void registerEntityTransformFunctions(asIScriptEngine* scriptEngine);
}