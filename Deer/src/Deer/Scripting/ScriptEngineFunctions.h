#pragma once
#include <string>

struct asSMessageInfo;
class asIScriptEngine;

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param);
    void print(std::string& msg);

    void registerDeerFunctions(asIScriptEngine* scriptEngine);
}