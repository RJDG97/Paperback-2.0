/**********************************************************************************
* \file ShaderProgram.h
* \brief This is the shader program class used to handle which shader is being used
* and sending uniforms to that shader
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "glew/inc/glew.h"
#include "glm/inc/glm.hpp"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(GLuint ShaderHandle);
	ShaderProgram(const ShaderProgram& ShaderProgram);
	~ShaderProgram() = default;

	ShaderProgram& operator=(const ShaderProgram& ShaderProgram);

	// Setting shader state
	void Use();
	void UnUse();

	// Delete shader
	void DeleteShader();

	// Functions to send shader uniforms
	void SetUniform(GLchar const* Name, GLboolean Val);
	void SetUniform(GLchar const* Name, GLint Val);
	void SetUniform(GLchar const* Name, GLuint Val);
	void SetUniform(GLchar const* Name, GLfloat Val);
	void SetUniform(GLchar const* Name, GLfloat X, GLfloat Y);
	void SetUniform(GLchar const* Name, GLfloat X, GLfloat Y, GLfloat Z);
	void SetUniform(GLchar const* Name, GLfloat X, GLfloat Y, GLfloat Z, GLfloat W);
	void SetUniform(GLchar const* Name, glm::vec2& Val);
	void SetUniform(GLchar const* Name, glm::vec3& Val);
	void SetUniform(GLchar const* Name, glm::vec4& Val);
	void SetUniform(GLchar const* Name, glm::mat3& Val);
	void SetUniform(GLchar const* Name, glm::mat4& Val);

	GLuint GetShaderHandle() { return m_ShaderHandle; }

private:
	// Shader handle
	GLuint m_ShaderHandle;
};

#endif