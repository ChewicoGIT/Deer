#include "ScriptEngineFunctions.h"

#include "angelscript.h"
#include "Deer/Core/Log.h"

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

    void registerDeerFunctions(asIScriptEngine* scriptEngine) {
        int r = scriptEngine->SetMessageCallback(asFUNCTION(Deer::messageCallback), 0, asCALL_CDECL);
        DEER_SCRIPT_ASSERT(r >= 0, "Error in seting up angel script");

        r = scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(Deer::print), asCALL_CDECL);
        DEER_SCRIPT_ASSERT(r >= 0, "Error in seting up void print(const string &in)");
    }
}