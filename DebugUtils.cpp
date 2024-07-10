// DebugUtils.cpp

#include "DebugUtils.h"
#include <windows.h>

void PrintToOutputWindow(const std::string& message) {
    OutputDebugStringA(message.c_str());
}