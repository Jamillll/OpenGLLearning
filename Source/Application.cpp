#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    const int flarryCount = 6;
    float flarray[6] =
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (flarryCount * sizeof(float)), flarray, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), 0);
    glEnableVertexAttribArray(0);

    ShaderSource shaders = parseShader("Resources\\Shaders\\BasicVertex.shader", "Resources\\Shaders\\BasicFragment.shader");
    std::cout << shaders.VertexSource << std::endl << shaders.FragmentSource << std::endl;

    unsigned int shader = CreateShader(shaders.VertexSource, shaders.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}