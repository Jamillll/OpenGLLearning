#pragma once
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCheck(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(__FILE__, #x, __LINE__));

void GLClearError();
bool GLCheckError(const char* fileName, const char* functionName, int line);