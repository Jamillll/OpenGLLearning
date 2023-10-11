#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCheck(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(__FILE__, #x, __LINE__));

static void GLClearError() { while (glGetError() != 0) {} }

static bool GLCheckError(const char* fileName, const char* functionName, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "!!Error!! " << error << ":" << fileName << ", " << functionName << ", Line " << line << std::endl;
        return false;
    }

    return true;
}

struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderSource parseShader(const std::string vertexFilepath, const std::string fragmentFilepath)
{
    ShaderSource source;
    std::ifstream stream;
    stream.open(vertexFilepath);

    std::string currentLine = "";
    while (getline(stream, currentLine))
    {
        source.VertexSource += currentLine + "\n";
    }

    stream.close();
    stream.open(fragmentFilepath);
    
    currentLine = "";
    while (getline(stream, currentLine))
    {
        source.FragmentSource += currentLine + "\n";
    }

    stream.close();

    return source;
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* sourcePointer = source.c_str();
    glShaderSource(id, 1, &sourcePointer, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "FAILED TO COMPILE " << type << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    unsigned int program = glCreateProgram();

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
    glAttachShader(program, vertexShader);

    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

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

    const int indexBufferCount = 6;
    unsigned int indexBuffer[] =
    {
        0, 1, 2, 
        1, 3, 2
    };

    //Parsing and using shaders
    std::string vertexShaderPath = "Resources\\Shaders\\BasicVertex.shader";
    std::string fragmentShaderPath = "Resources\\Shaders\\BasicFragment.shader";

    ShaderSource shaders = parseShader(vertexShaderPath, fragmentShaderPath);
    unsigned int shader = CreateShader(shaders.VertexSource, shaders.FragmentSource);
    glUseProgram(shader);

    //Creating and binding a Vertex Attrib Array
    unsigned int vertexAttribArrayID;
    glGenVertexArrays(1, &vertexAttribArrayID);
    glBindVertexArray(vertexAttribArrayID);

    //Vertex Buffer
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * vertexArrayCount), vertexArray, GL_STATIC_DRAW);
    
    //Vertex Attributes
    //Position Attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), 0);
    glEnableVertexAttribArray(0);
    //Colour Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    //Index Buffer
    unsigned int indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(unsigned int) * indexBufferCount), indexBuffer, GL_STATIC_DRAW);

    int location = glGetUniformLocation(shader, "u_Scale");
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