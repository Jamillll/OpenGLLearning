#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderHandler.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the current context the window and set its refresh rate to sync with the monitors
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    const int vertexArrayCount = 30;
    float vertexArray[30] =
    {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    const int indexArrayCount = 6;
    unsigned int indexArray[] =
    {
        0, 1, 2, 
        1, 3, 2
    };

    //Parsing and using shaders
    std::string vertexShader = "Resources\\Shaders\\BasicVertex.shader";
    std::string fragmentShader = "Resources\\Shaders\\BasicFragment.shader";
    ShaderHandler shaderHandler(vertexShader, fragmentShader);

    //Creating and binding a Vertex Attrib Array
    unsigned int vertexAttribArrayObjectID;
    glGenVertexArrays(1, &vertexAttribArrayObjectID);
    glBindVertexArray(vertexAttribArrayObjectID);

    //Vertex Buffer
    VertexBuffer vertexBuffer(vertexArray, (sizeof(float) * vertexArrayCount));
    
    //Position Attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), 0);
    glEnableVertexAttribArray(0);
    //Colour Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    //Index Buffer
    IndexBuffer indexBuffer(indexArray, indexArrayCount);

    int location = shaderHandler.GetUniformLocation("u_Scale");
    float scale = 0.5f;
    float scaleUp = 0.05f;

    //Main Loop
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        if (scale >= 2)
        {
            scaleUp = -0.05f;
        }
        else if (scale <= 0.5)
        {
            scaleUp = 0.05f;
        }

        scale += scaleUp;
        glUniform1f(location, scale);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}