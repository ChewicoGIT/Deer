#include "ScriptEngineFunctions.h"

#include "angelscript.h"
#include "Deer/Core/Log.h"

#include "glm/glm.hpp"

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param) {
        if (msg->type == asMSGTYPE_WARNING) {
            DEER_SCRIPT_WARN("({0} {1}) : {2} \n {3}", msg->row, msg->col, msg->message, msg->section);
        }
        else if (msg->type == asMSGTYPE_ERROR) {
            DEER_SCRIPT_ERROR("({0} {1}) : {2} \n {3}", msg->row, msg->col, msg->message, msg->section);
        }
        else if (msg->type == asMSGTYPE_INFORMATION) {
            DEER_SCRIPT_INFO("({0} {1}) : {2} \n {3}", msg->row, msg->col, msg->message, msg->section);
        }
        else {
            DEER_SCRIPT_WARN("({0} {1}) : {2} \n {3}", msg->row, msg->col, msg->message, msg->section);
        }
    }

    void print(std::string& msg) {
        DEER_SCRIPT_INFO(msg.c_str());
    }

    void registerVec3(asIScriptEngine* engine) {
        engine->RegisterObjectType("Vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::vec3>());

        engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR([](void* memory) {
            new (memory) glm::vec3();
            }, (void*), void), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(float = 0, float = 0, float = 0)", asFUNCTIONPR([](float x, float y, float z, void* memory) {
            new (memory) glm::vec3(x, y, z);
            }, (float, float, float, void*), void), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectProperty("Vec3", "float x", asOFFSET(glm::vec3, x));
        engine->RegisterObjectProperty("Vec3", "float y", asOFFSET(glm::vec3, y));
        engine->RegisterObjectProperty("Vec3", "float z", asOFFSET(glm::vec3, z));

        engine->RegisterObjectMethod("Vec3", "Vec3 opAdd(const Vec3 &in) const",
            asFUNCTIONPR([](const glm::vec3& a, const glm::vec3& b) {
                return a + b;
                }, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "Vec3 opSub(const Vec3 &in) const",
            asFUNCTIONPR([](const glm::vec3& a, const glm::vec3& b) {
                return a - b;
                }, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "Vec3 opMul(float) const",
            asFUNCTIONPR([](const glm::vec3& a, float scalar) {
                return a * scalar;
                }, (const glm::vec3&, float), glm::vec3), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "Vec3 opDiv(float) const",
            asFUNCTIONPR([](const glm::vec3& a, float scalar) {
                return a / scalar;
                }, (const glm::vec3&, float), glm::vec3), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "Vec3 normalize() const",
            asFUNCTIONPR(glm::normalize, (const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "float getMagnitude() const",
            asFUNCTIONPR(glm::length, (const glm::vec3&), float), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "Vec3 opNeg() const",
            asFUNCTIONPR([](const glm::vec3& a) {
                return -a;
                }, (const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "bool opEquals(const Vec3 &in) const",
            asFUNCTIONPR([](const glm::vec3& a, const glm::vec3& b) {
                return a == b;
                }, (const glm::vec3&, const glm::vec3&), bool), asCALL_CDECL_OBJFIRST);

        engine->RegisterObjectMethod("Vec3", "string opImplConv() const",
            asFUNCTIONPR([](const glm::vec3& a) {
                // Example string conversion using glm (you may need to format it)
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)", a.x, a.y, a.z);
                return std::string(buffer);
                }, (const glm::vec3&), std::string), asCALL_CDECL_OBJFIRST);
    }

    void registerDeerFunctions(asIScriptEngine* scriptEngine) {
        int r = scriptEngine->SetMessageCallback(asFUNCTION(Deer::messageCallback), 0, asCALL_CDECL);
        DEER_SCRIPT_ASSERT(r >= 0, "Error in seting up angel script");

        r = scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(Deer::print), asCALL_CDECL);
        DEER_SCRIPT_ASSERT(r >= 0, "Error in seting up void print(const string &in)");
    }

}