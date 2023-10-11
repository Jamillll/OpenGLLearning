#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != 0)
    {

    }
}

bool GLCheckError(const char* fileName, const char* functionName, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "!!Error!! " << error << ":" << fileName << ", " << functionName << ", Line " << line << std::endl;
        return false;
    }

    return true;
}