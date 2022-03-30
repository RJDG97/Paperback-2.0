#pragma once
#ifndef RENDERER_H
#define RENDERER_H


#include <string>
#include <unordered_map>
#include <array>
#include <glm/inc/glm.hpp>
#include "../RenderResource/RenderResourceManager.h"
#include "../RenderResource/RenderResourceLoader.h"
#include "../Camera/Camera.h"

class Renderer
{
public:

	struct TransformInfo
	{
		glm::mat4 m_Transform;
		glm::mat4* m_ParentSocketTransform;
		std::vector<glm::mat4>* m_BoneTransforms;
		int m_ShadowBias;
		bool m_CastShadow;
	};

	struct UIInfo
	{
		glm::mat4 m_Transform;
		std::string m_Texture;
	};

	struct TextInfo
	{
		glm::mat4 m_Transform;
		glm::vec3 m_Color;
		std::string m_Text;
	};

	struct PointLightInfo
	{
		glm::vec3 m_Position;

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};

	struct InstancedInfo
	{
		glm::mat4 m_Transform;
		float m_Opacity;
	};

	~Renderer();

	// Render object
	void Render(const std::unordered_map<std::string_view, std::vector<TransformInfo>>& Objects,
				const std::vector<PointLightInfo>& Lights,
				const std::unordered_map<std::string_view, std::vector<InstancedInfo>>& Instances,
				const Camera3D& SceneCamera,
				const bool Gamma,
				const std::map<float, std::vector<UIInfo>>& UIs,
				const std::unordered_map<std::string_view, std::vector<TextInfo>>& Texts,
				const Camera2D& UICamera,
				const std::array<std::vector<glm::vec3>, 2>* Points = nullptr);

	GLuint GetUIOverlay();
	GLuint GetFinalImage();

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
		GLuint m_RenderBuffer;
	};

	// Light
	struct DirectionalLight
	{
		glm::vec3 m_Position;
		glm::vec3 m_Direction;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Transform;
	};

	// Render debug objects
	void DebugRender(const std::array<std::vector<glm::vec3>, 2>& Points, const Camera3D& SceneCamera);

	void SkyBoxRender(const Camera3D& SceneCamera);

	void UIPass(const std::map<float, std::vector<UIInfo>>& UIs, const Camera2D& UICamera);
	void TextPass(const std::unordered_map<std::string_view, std::vector<TextInfo>>& Texts, const Camera2D& UICamera);
	void ShadowPass(const std::unordered_map<std::string_view, std::vector<TransformInfo>>& Objects);
	void RenderPass(const std::unordered_map<std::string_view, std::vector<TransformInfo>>& Objects, const Camera3D& SceneCamera);
	void GPass(const std::unordered_map<std::string_view, std::vector<TransformInfo>>& Objects, const Camera3D& SceneCamera);
	void SSAOPass(const Camera3D& SceneCamera);
	void LightPass(const std::vector<PointLightInfo>& Lights, const Camera3D& SceneCamera);
	void InstancedPass(const std::unordered_map<std::string_view, std::vector<InstancedInfo>>& Instances, const Camera3D& SceneCamera);
	void BlurPass();
	void CompositePass(const bool Gamma);
	void MergePass();
	void FinalPass();

	float Lerp(float a, float b, float f);

	// Gpass buffer
	FrameBuffer m_GBuffer;
	// Shadow buffer
	FrameBuffer m_ShadowBuffer;	
	// SSAO buffer
	FrameBuffer m_SSAOBuffer;
	// SSAO Blur buffer
	FrameBuffer m_SSAOBlurBuffer;
	// Lighting/BrightPass buffer
	FrameBuffer m_LightingBuffer;
	// Blur buffer
	FrameBuffer m_BlurBuffer;
	// UI buffer
	FrameBuffer m_UIBuffer;
	// Final buffer
	FrameBuffer m_FinalBuffer;

	// Light source
	DirectionalLight m_Light;

	// VAO for rendering
	GLuint m_VAO;
	// VAO for instanced rendering
	GLuint m_InstancedVAO;
	// VAO for debug objects
	GLuint m_DebugVAO;

	// Noise texture
	GLuint m_SSAONoise;

	// SSAO kernel
	std::vector<glm::vec3> m_SSAOKernel;
	
	int m_Width;
	int m_Height;

	RenderResourceManager& m_Resources;
	RenderResourceLoader& m_Loader;
};

#endif