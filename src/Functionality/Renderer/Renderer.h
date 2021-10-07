#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <unordered_map>
#include "glm/inc/glm.hpp"
#include "../RenderResource/RenderResourceManager.h"

class Renderer
{
public:
	~Renderer();

	// Update the resized framebuffers
	//void UpdateFramebufferSize(int Width, int Height);

	// Render object
	void Render(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects);

	// Render debug objects
	void DebugRender(const std::vector<glm::vec3>& Points, bool isalt);

	// Prep the start of draw frame
	void StartFrame();

	// Draw frame
	//void Draw();

	// Used for hotswapping framebuffer output
	//void DebugDraw();

	// Default drawing without effects
	//void DefaultDraw(GameobjectFactory::Entity Object);

	// Singleton
	static Renderer& GetInstanced();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

private:
	Renderer();

	// Helper function to create framebuffers
	//void SetUpFramebuffer();

	// Frame buffer type
	//struct FrameBuffer
	//{
	//	std::vector<GLuint> m_FrameBuffer;
	//	std::vector<GLuint> m_BufferTexture;
	//};

	//// G-pass buffer
	//FrameBuffer m_GPassBuffer;
	//// Composite buffer
	//FrameBuffer m_CompositePassBuffer;
	//// Bright pass buffer
	//FrameBuffer m_BrightPassBuffer;
	//// Blur pass buffer
	//FrameBuffer m_BlurPassBuffer;

	// Attachments for framebuffer
	//unsigned int m_Attachments[3];

	// VAO for rendering
	GLuint m_VAO;
	// VAO for debug objects
	GLuint m_DebugVAO;

	RenderResourceManager& m_Resources;

};

#endif