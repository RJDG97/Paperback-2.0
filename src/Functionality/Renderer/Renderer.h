#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <unordered_map>
#include <array>
#include "glm/inc/glm.hpp"
#include "../RenderResource/RenderResourceManager.h"

class Renderer
{
public:
	~Renderer();

	// Update the resized framebuffers
	//void UpdateFramebufferSize(int Width, int Height);

	// Render object
	void Render(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects, const std::array<std::vector<glm::vec3>, 2>* Points = nullptr, std::vector<glm::mat4>* bone_transforms = nullptr);

	// Prep the start of draw frame
	void StartFrame();

	// Singleton
	static Renderer& GetInstanced();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

private:
	Renderer();

	// Frame buffer type
	struct FrameBuffer
	{
		std::vector<GLuint> m_FrameBuffer;
		std::vector<GLuint> m_BufferTexture;
	};

	// Light
	struct Light
	{
		glm::vec3 m_Position;
		glm::vec3 m_Direction;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Transform;
	};

	// Helper function to create framebuffers
	void SetUpFramebuffer();

	// Render debug objects
	void DebugRender(const std::array<std::vector<glm::vec3>, 2>& Points);

	void ShadowPass(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects, std::vector<glm::mat4>* bone_transforms = nullptr);
	void RenderPass(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects, std::vector<glm::mat4>* bone_transforms = nullptr);
	void BlurPass();
	void CompositePass();

	// Shadow buffer
	FrameBuffer m_ShadowBuffer;
	// Lighting/BrightPass buffer
	FrameBuffer m_LightingBuffer;
	// Blur buffer
	FrameBuffer m_BlurBuffer;

	// Light source
	Light m_Light;

	// VAO for rendering
	GLuint m_VAO;
	// VAO for debug objects
	GLuint m_DebugVAO;

	RenderResourceManager& m_Resources;
};

#endif