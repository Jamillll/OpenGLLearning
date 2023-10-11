#pragma once
#include <string>

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderHandler
{
private:
	unsigned int shaderID;

public:
	ShaderHandler(const std::string vertexFilepath, const std::string fragmentFilepath);

	~ShaderHandler();

	int GetUniformLocation(const char* uniformName);

private:
	ShaderSource ParseShader(const std::string vertexFilepath, const std::string fragmentFilepath);

	unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
};