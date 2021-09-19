/**********************************************************************************
* \file ShaderBuilder.h
* \brief This is the shader builder class which provides helper function to compile,
* link, validate and generate shader programs for use.
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "ShaderProgram.h"
#include <string>

class ShaderBuilder
{
public:
	ShaderBuilder() = delete;
	ShaderBuilder(const ShaderBuilder&) = delete;
	~ShaderBuilder() = delete;

	ShaderBuilder& operator=(const ShaderBuilder&) = delete;

	static ShaderProgram CreateShader(const std::string& VertexShader, const std::string& FragmentShader);

private:
	// Type of shaders
	enum class ShaderType 
	{
		VERTEX_SHADER = 0,
		FRAGMENT_SHADER,
	};

	// Helper functions to generate shader
	static void CompileShader(GLuint& ShaderProgram, const ShaderType Type, const std::string& ShaderFile);
	static void LinkShader(GLuint& ShaderProgram);
	static void ValidateShader(GLuint& ShaderProgram);

	static std::string ShaderErrorReport(GLuint& ShaderHandle);
	static std::string ProgramErrorReport(GLuint& Prorgramhandle);
};

#endif