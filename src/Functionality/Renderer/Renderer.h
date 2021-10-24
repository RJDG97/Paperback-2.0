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

	// Render object
	void Render(const std::unordered_map<std::string_view, std::vector<std::pair<glm::mat4, std::vector<glm::mat4>*>>>& Objects, const std::array<std::vector<glm::vec3>, 2>* Points = nullptr);

	// Helper function to create framebuffers
	void SetUpFramebuffer(int Width, int Height);
	// Update the resized framebuffers
	void UpdateFramebufferSize(int Width, int Height);

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

	// Render debug objects
	void DebugRender(const std::array<std::vector<glm::vec3>, 2>& Points);

	void SkyBoxRender();

	void ShadowPass(const std::unordered_map<std::string_view, std::vector<std::pair<glm::mat4, std::vector<glm::mat4>*>>>& Objects);
	void RenderPass(const std::unordered_map<std::string_view, std::vector<std::pair<glm::mat4, std::vector<glm::mat4>*>>>& Objects);
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

	int m_Width;
	int m_Height;

	RenderResourceManager& m_Resources;
};

#endif