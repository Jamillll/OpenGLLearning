#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderHandler.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>  

bool setup(GLFWwindow** window, unsigned int *height, unsigned int *width)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    *window = glfwCreateWindow(*width, *height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    // Make the current context the window and set its refresh rate to sync with the monitors
    glfwMakeContextCurrent(*window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return false;
    }

    return true;
}

struct Vertex
{
    float x,y;
    float r,g,b;
};

struct Cell
{
    Vertex vertices[6];
};

int main(void)
{
    unsigned int windowWidth = 900;
    unsigned int windowHeight = windowWidth;

    GLFWwindow* window;
    if (!setup(&window, &windowWidth, &windowHeight)) return -1;

    const int gridAxisLength = 90;

    const int cellArrayCount = gridAxisLength * gridAxisLength;
    Cell cellArray[cellArrayCount];

    const float cellSize = 2.0 / (gridAxisLength + (gridAxisLength / 10));
    const float gapSize = cellSize / 10;

    int testCounter = 1;
    float currentY = 1.0f - gapSize;
    for (int y = 1; y != gridAxisLength; y++)
    {
        float currentX = -1.0f + gapSize;
        for (int x = 1; x != gridAxisLength; x++)
        {
            unsigned int index = (y * gridAxisLength) + x;
            float colours[3];

            switch (testCounter)
            {
            case 1:
                colours[0] = 1.0f;
                colours[1] = 1.0f;
                colours[2] = 1.0f;
                testCounter++;
                break;

            case 2:
                colours[0] = 1.0f;
                colours[1] = 1.0f;
                colours[2] = 1.0f;
                testCounter = 1;
                break;
            }

            cellArray[index].vertices[0].x = currentX;
            cellArray[index].vertices[0].y = currentY;

            cellArray[index].vertices[1].x = currentX + cellSize;
            cellArray[index].vertices[1].y = currentY;

            cellArray[index].vertices[2].x = currentX;
            cellArray[index].vertices[2].y = currentY - cellSize;

            cellArray[index].vertices[3].x = currentX;
            cellArray[index].vertices[3].y = currentY - cellSize;

            cellArray[index].vertices[4].x = currentX + cellSize;
            cellArray[index].vertices[4].y = currentY - cellSize;

            cellArray[index].vertices[5].x = currentX + cellSize;
            cellArray[index].vertices[5].y = currentY;

            for (int i = 0; i < 6; i++)
            {
                cellArray[index].vertices[i].r = colours[0];
                cellArray[index].vertices[i].g = colours[1];
                cellArray[index].vertices[i].b = colours[2];

                std::cout << cellArray[index].vertices[i].x << ", " << cellArray[index].vertices[i].y << ", " << cellArray[index].vertices[i].r << std::endl;
            }

            std::cout << std::endl;
            
            currentX += cellSize + gapSize;
        }

        currentY -= cellSize + gapSize;
    }

    //const int vertexArrayCount = 4;
    //Vertex vertexArray[4] =
    //{
    //    {-0.5f, -0.5f, 0.9f, 0.8f, 0.5f},
    //    {0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
    //    { -0.5f,  0.5f, 0.9f, 0.8f, 0.5f},
    //    {0.5f,  0.5f, 0.0f, 0.0f, 1.0f}
    //};

    //const int indexArrayCount = 6;
    //unsigned int indexArray[] =
    //{
    //    0, 1, 2, 
    //    1, 3, 2
    //};

    //Parsing and using shaders
    std::string vertexShader = "Resources\\Shaders\\BasicVertex.shader";
    std::string fragmentShader = "Resources\\Shaders\\BasicFragment.shader";
    ShaderHandler shaderHandler(vertexShader, fragmentShader);

    //Creating and binding a Vertex Attrib Array
    unsigned int vertexAttribArrayObjectID;
    glGenVertexArrays(1, &vertexAttribArrayObjectID);
    glBindVertexArray(vertexAttribArrayObjectID);

    //Vertex Buffer
    VertexBuffer vertexBuffer(cellArray, (sizeof(Cell) * cellArrayCount));
    
    //Position Attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), 0);
    glEnableVertexAttribArray(0);
    //Colour Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    int location = shaderHandler.GetUniformLocation("u_Scale");
    float scale = 0.5f;
    const float defaulScaleUp = 0.01f;
    float scaleUp = defaulScaleUp;

    //Main Loop
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        if (scale >= 2)
        {
            scaleUp = -defaulScaleUp;
        }
        else if (scale <= 0.5)
        {
            scaleUp = defaulScaleUp;
        }

        scale += scaleUp;
        glUniform1f(location, scale);

        glDrawArrays(GL_TRIANGLES, 0, cellArrayCount * 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}