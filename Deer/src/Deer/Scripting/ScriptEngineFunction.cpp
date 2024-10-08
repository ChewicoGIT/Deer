#include "ScriptEngineFunction.h"
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
}