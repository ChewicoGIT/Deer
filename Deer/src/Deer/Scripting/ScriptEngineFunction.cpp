#include "ScriptEngineFunction.h"
#include "angelscript.h"
#include "Deer/Core/Log.h"

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param) {
        if (msg->type == asMSGTYPE_WARNING) {
            DEER_SCRIPT_WARN(msg->message);
        }
        else if (msg->type == asMSGTYPE_ERROR) {
            DEER_SCRIPT_ERROR(msg->message);
        }
        else if (msg->type == asMSGTYPE_INFORMATION) {
            DEER_SCRIPT_INFO(msg->message);
        }
    }

    void print(std::string& msg) {
        DEER_SCRIPT_INFO(msg.c_str());
    }
}