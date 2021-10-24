/**********************************************************************************
* \file ShaderProgram.cpp
* \brief This is the shader program class used to handle which shader is being used
* and sending uniforms to that shader
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#include "ShaderProgram.h"
#include <string>
#include <exception>
#include <glm/inc/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram() :
	m_ShaderHandle{ 0 }
{

}

ShaderProgram::ShaderProgram(GLuint ShaderProgram) :
	m_ShaderHandle{ ShaderProgram }
{

}

ShaderProgram::ShaderProgram(const ShaderProgram& ShaderProgram) : 
	m_ShaderHandle{ ShaderProgram.m_ShaderHandle }
{

}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& ShaderProgram)
{
	m_ShaderHandle = ShaderProgram.m_ShaderHandle;

	return *this;
}

void ShaderProgram::Use()
{
	if (m_ShaderHandle)
		glUseProgram(m_ShaderHandle);
}

void ShaderProgram::UnUse()
{
	glUseProgram(0);
}

void ShaderProgram::DeleteShader()
{
	if (m_ShaderHandle)
		glDeleteProgram(m_ShaderHandle);
}

void ShaderProgram::SetUniform(GLchar const* Name, GLboolean Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform1i(Location, Val);
	else
		throw std::exception{ "Uniform not found" };
}

void ShaderProgram::SetUniform(GLchar const* Name, GLint Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform1i(Location, Val);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, GLuint Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform1i(Location, Val);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, GLfloat Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform1f(Location, Val);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, GLfloat X, GLfloat Y)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform2f(Location, X, Y);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, GLfloat X, GLfloat Y, GLfloat Z)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform3f(Location, X, Y, Z);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, GLfloat X, GLfloat Y, GLfloat Z, GLfloat W)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform4f(Location, X, Y, Z, W);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, glm::vec2& Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform2f(Location, Val.x, Val.y);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, glm::vec3& Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform3f(Location, Val.x, Val.y, Val.z);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, glm::vec4& Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniform4f(Location, Val.x, Val.y, Val.z, Val.w);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, glm::mat3& Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniformMatrix3fv(Location, 1, GL_FALSE, &Val[0][0]);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}

void ShaderProgram::SetUniform(GLchar const* Name, glm::mat4& Val)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniformMatrix4fv(Location, 1, GL_FALSE, &Val[0][0]);
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}


void ShaderProgram::SetUniform(GLchar const* Name, std::vector<glm::mat4>& Val, const size_t Size)
{
	GLint Location = glGetUniformLocation(m_ShaderHandle, Name);

	if (Location >= 0)
		glUniformMatrix4fv(Location, Size, GL_FALSE, glm::value_ptr(Val[0]));
	else
		throw std::exception{ ("Uniform not found: " + std::string{Name}).c_str() };
}