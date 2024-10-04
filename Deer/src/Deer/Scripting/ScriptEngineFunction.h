#pragma once
#include <string>

struct asSMessageInfo;

namespace Deer {
    void messageCallback(const asSMessageInfo* msg, void* param);
    void print(std::string& msg);
}