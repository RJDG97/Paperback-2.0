/**********************************************************************************
* \file ShaderBuilder.cpp
* \brief This is the shader builder class which provides helper function to compile,
* link, validate and generate shader programs for use.
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#include "ShaderBuilder.h"
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram ShaderBuilder::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
	// Create Shader program handle
	GLuint shaderProgramHandle = glCreateProgram();

	if (!shaderProgramHandle)
		throw std::exception("Create shader handle failed");

	// Compile shaders
	CompileShader(shaderProgramHandle, ShaderType::VERTEX_SHADER, VertexShader);
	CompileShader(shaderProgramHandle, ShaderType::FRAGMENT_SHADER, FragmentShader);

	// Link shaders
	LinkShader(shaderProgramHandle);

	// Validate shaders
	ValidateShader(shaderProgramHandle);

	return ShaderProgram{ shaderProgramHandle };
}

void ShaderBuilder::CompileShader(GLuint& ShaderProgram, const ShaderType Type, const std::string& ShaderFile)
{
	// Open shader file
	std::ifstream ifs{ ShaderFile, std::ifstream::in };

	if (ifs.is_open())
	{
		// Read shader code into buffer
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();

		// Create Shader
		GLuint ShaderHandle = 0;
		switch (Type)
		{
		case ShaderType::VERTEX_SHADER: ShaderHandle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::FRAGMENT_SHADER: ShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}

		// Load shader source code
		std::string ShaderString{ buffer.str() };
		GLchar const* ShaderCode[] = { ShaderString.c_str() };
		glShaderSource(ShaderHandle, 1, ShaderCode, NULL);

		// Compile shader
		glCompileShader(ShaderHandle);

		// Check compilation result
		GLint CompilationResult;
		glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &CompilationResult);

		// Check status
		if (CompilationResult)
		{
			glAttachShader(ShaderProgram, ShaderHandle);
			glDeleteShader(ShaderHandle);
		}
		else
		{
			throw std::exception{ ShaderErrorReport(ShaderHandle).c_str() };
		}
	}
	else
	{
		throw std::exception("Shader file failed to open");
	}
}

void ShaderBuilder::LinkShader(GLuint& ShaderProgram)
{
	glLinkProgram(ShaderProgram);

	GLint linkResult = 0;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &linkResult);

	if (!linkResult)
		throw std::exception{ ShaderErrorReport(ShaderProgram).c_str() };
}

void ShaderBuilder::ValidateShader(GLuint& ShaderProgram)
{
	glValidateProgram(ShaderProgram);

	GLint validateResult = 0;
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &validateResult);

	if (!validateResult)
		throw std::exception{ ShaderErrorReport(ShaderProgram).c_str() };
}

std::string ShaderBuilder::ShaderErrorReport(GLuint& ShaderHandle)
{
	// Error log
	static std::string errorLog;
	errorLog = "Program Error Log: ";

	// Shader error log length
	GLint logLength = 0;
	glGetShaderiv(ShaderHandle, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength)
	{
		// Create log buffer and get log from shader
		GLchar* log = new GLchar[logLength];
		GLsizei writtenLogLength;
		glGetShaderInfoLog(ShaderHandle, logLength, &writtenLogLength, log);

		// Add log to error
		errorLog.append(log);
		delete[] log;
	}

	return errorLog;
}

std::string ShaderBuilder::ProgramErrorReport(GLuint& ProgramHandle)
{
	// Error log
	static std::string errorLog;
	errorLog = "Program Error Log: ";

	// Shader error log length
	GLint logLength = 0;
	glGetProgramiv(ProgramHandle, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength)
	{
		// Create log buffer and get log from shader
		GLchar* log = new GLchar[logLength];
		GLsizei writtenLogLength;
		glGetProgramInfoLog(ProgramHandle, logLength, &writtenLogLength, log);

		// Add log to error
		errorLog.append(log);
		delete[] log;
	}

	return errorLog;
}