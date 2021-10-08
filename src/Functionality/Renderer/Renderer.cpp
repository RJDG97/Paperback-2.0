#include "Renderer.h"
#include "paperback_camera.h"
#include "glm/inc/gtc/type_ptr.hpp"
#include <glm/inc/gtx/transform.hpp>

Renderer::Renderer() :
	m_Resources{ RenderResourceManager::GetInstanced() }
{
	// Create a handle for vao
	glCreateVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	// Set vao attribute values
	glEnableVertexArrayAttrib(m_VAO, 0);
	glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, m_Position));
	glVertexArrayAttribBinding(m_VAO, 0, 0);

	glEnableVertexArrayAttrib(m_VAO, 1);
	glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, m_Normal));
	glVertexArrayAttribBinding(m_VAO, 1, 0);

	glEnableVertexArrayAttrib(m_VAO, 2);
	glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, m_UV));
	glVertexArrayAttribBinding(m_VAO, 2, 0);

	glEnableVertexArrayAttrib(m_VAO, 3);
	glVertexArrayAttribFormat(m_VAO, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, m_Tangent));
	glVertexArrayAttribBinding(m_VAO, 3, 0);

	glEnableVertexArrayAttrib(m_VAO, 4);
	glVertexArrayAttribFormat(m_VAO, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, m_BiTangent));
	glVertexArrayAttribBinding(m_VAO, 4, 0);

	glEnableVertexArrayAttrib(m_VAO, 5);
	glVertexArrayAttribIFormat(m_VAO, 5, 4, GL_INT, offsetof(Model::Vertex, m_BoneIDs));
	glVertexArrayAttribBinding(m_VAO, 5, 0);

	glEnableVertexArrayAttrib(m_VAO, 6);
	glVertexArrayAttribFormat(m_VAO, 6, 4, GL_FLOAT, GL_FALSE, offsetof(Model::Vertex, m_Weights));
	glVertexArrayAttribBinding(m_VAO, 6, 0);

	glBindVertexArray(0);

	glCreateVertexArrays(1, &m_DebugVAO);

	glEnableVertexArrayAttrib(m_DebugVAO, 0);
	glVertexArrayAttribFormat(m_DebugVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_DebugVAO, 0, 0);

	glBindVertexArray(0);

	m_Resources.LoadShader("Shadow", "../../resources/shaders/Shadow.vert", "../../resources/shaders/Shadow.frag");
	m_Resources.LoadShader("Light", "../../resources/shaders/Lighting.vert", "../../resources/shaders/Lighting.frag");
	m_Resources.LoadShader("Blur", "../../resources/shaders/SimplePassthrough.vert", "../../resources/shaders/Blur.frag");
	m_Resources.LoadShader("Composite", "../../resources/shaders/SimplePassthrough.vert", "../../resources/shaders/Composite.frag");
	m_Resources.LoadShader("Debug", "../../resources/shaders/Debug.vert", "../../resources/shaders/Debug.frag");

	m_Resources.Load3DMesh("Backpack", "../../resources/models/backpack.obj");
	m_Resources.Load3DMesh("Box", "../../resources/models/box.fbx");
	m_Resources.Load3DMesh("Plane", "../../resources/models/plane2.obj");
	m_Resources.Load3DMesh("Character", "../../resources/models/mutant.fbx");

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable face cullling
	glEnable(GL_CULL_FACE);

	// Set up framebuffers
	SetUpFramebuffer();

	m_Light.m_Position = glm::vec3{ 1.f, 1.f, 1.f };
	m_Light.m_Direction = glm::normalize(glm::vec3{ 0.f } - m_Light.m_Position);
	m_Light.m_Projection = glm::ortho(-35.f, 35.f, -35.f, 35.f, -35.f, 35.f);
	m_Light.m_View = glm::lookAt(m_Light.m_Position, glm::vec3{ 0.f }, glm::vec3{ 0.f, 1.f, 0.f });
	m_Light.m_Transform = m_Light.m_Projection * m_Light.m_View;
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_DebugVAO);

	glDeleteFramebuffers(m_ShadowBuffer.m_FrameBuffer.size(), m_ShadowBuffer.m_FrameBuffer.data());
	glDeleteTextures(m_ShadowBuffer.m_BufferTexture.size(), m_ShadowBuffer.m_FrameBuffer.data());

	glDeleteFramebuffers(m_LightingBuffer.m_FrameBuffer.size(), m_LightingBuffer.m_FrameBuffer.data());
	glDeleteTextures(m_LightingBuffer.m_BufferTexture.size(), m_LightingBuffer.m_FrameBuffer.data());

	glDeleteFramebuffers(m_BlurBuffer.m_FrameBuffer.size(), m_BlurBuffer.m_FrameBuffer.data());
	glDeleteTextures(m_BlurBuffer.m_BufferTexture.size(), m_BlurBuffer.m_FrameBuffer.data());
}

void Renderer::SetUpFramebuffer()
{
	// Create shadow framebuffer
	GLuint shadowFBO;
	glCreateFramebuffers(1, &shadowFBO);

	// Create shadow depth texture
	GLuint shadowDepth;
	glCreateTextures(GL_TEXTURE_2D, 1, &shadowDepth);
	glTextureStorage2D(shadowDepth, 1, GL_DEPTH_COMPONENT32F, 1024, 1024);
	glTextureParameteri(shadowDepth, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(shadowDepth, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(shadowDepth, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(shadowDepth, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 1.f, 1.f, 1.f, 1.f };
	glTextureParameterfv(shadowDepth, GL_TEXTURE_BORDER_COLOR, color);

	// Attach texture to framebuffer
	glNamedFramebufferTexture(shadowFBO, GL_DEPTH_ATTACHMENT, shadowDepth, 0);
	glNamedFramebufferDrawBuffer(shadowFBO, GL_NONE);
	glNamedFramebufferReadBuffer(shadowFBO, GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Add framebuffer and texture to framebuffer object
	m_ShadowBuffer.m_FrameBuffer.push_back(shadowFBO);
	m_ShadowBuffer.m_BufferTexture.push_back(shadowDepth);

	int width = 1280, height = 720;

	// Create light framebuffer
	GLuint lightFbo;
	glCreateFramebuffers(1, &lightFbo);

	// Create light textures
	GLuint lightTexture[3];
	glCreateTextures(GL_TEXTURE_2D, 3, lightTexture);

	for (size_t i = 0; i < 2; ++i)
	{
		glTextureStorage2D(lightTexture[i], 1, GL_RGBA16F, width, height);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to framebuffer
		glNamedFramebufferTexture(lightFbo, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), lightTexture[i], 0);

		// Add texture to framebuffer object
		m_LightingBuffer.m_BufferTexture.push_back(lightTexture[i]);
	}

	// Attach depth texture to frmaebuffer
	glTextureStorage2D(lightTexture[2], 1, GL_DEPTH_COMPONENT32F, width, height);
	glNamedFramebufferTexture(lightFbo, GL_DEPTH_ATTACHMENT, lightTexture[2], 0);

	GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glNamedFramebufferDrawBuffers(lightFbo, 2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Add framebuffer and texture to framebuffer object
	m_LightingBuffer.m_BufferTexture.push_back(lightTexture[2]);
	m_LightingBuffer.m_FrameBuffer.push_back(lightFbo);

	// Create blur framebuffer
	GLuint blurFBO[2];
	glCreateFramebuffers(2, blurFBO);

	// Create blur texture
	GLuint blurTexture[2];
	glCreateTextures(GL_TEXTURE_2D, 2, blurTexture);

	for (size_t i = 0; i < 2; ++i)
	{
		glTextureStorage2D(blurTexture[i], 1, GL_RGBA16F, width, height);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to framebuffer
		glNamedFramebufferTexture(blurFBO[i], GL_COLOR_ATTACHMENT0, blurTexture[i], 0);

		// Add framebuffer and texture to framebuffer object
		m_BlurBuffer.m_FrameBuffer.push_back(blurFBO[i]);
		m_BlurBuffer.m_BufferTexture.push_back(blurTexture[i]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Render(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects, const std::array<std::vector<glm::vec3>, 2>* Points, std::vector<glm::mat4>* bone_transforms)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShadowPass(Objects, bone_transforms);
	RenderPass(Objects, bone_transforms);
	if (Points)
	{
		DebugRender(*Points);
	}
	BlurPass();
	CompositePass();
}

void Renderer::DebugRender(const std::array<std::vector<glm::vec3>, 2>& Points)
{
	// Bind shader
	m_Resources.m_Shaders["Debug"].Use();

	// Bind vao
	glBindVertexArray(m_DebugVAO);

	glm::mat4 transform = Camera::GetInstanced().GetProjection() * Camera::GetInstanced().GetView();

	m_Resources.m_Shaders["Debug"].SetUniform("uTransform", transform);

	for (int i = 0; i < Points.size(); ++i)
	{
		// Create vbo for debug lines
		GLuint vbo;
		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo, sizeof(glm::vec3) * Points[i].size(), Points[i].data(), GL_DYNAMIC_STORAGE_BIT);

		m_Resources.m_Shaders["Debug"].SetUniform("uRed", i);

		// Bind vbo to vao
		glVertexArrayVertexBuffer(m_DebugVAO, 0, vbo, 0, sizeof(glm::vec3));

		glLineWidth(5.f);

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Points[i].size()));

		glLineWidth(1.f);

		// Destroy vbo
		glDeleteBuffers(1, &vbo);
	}

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Debug"].UnUse();
}

void Renderer::ShadowPass(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects, std::vector<glm::mat4>* bone_transforms)
{
	// Bind shadow frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowBuffer.m_FrameBuffer[0]);

	// Change to fit shadow texture size
	glViewport(0, 0, 1024, 1024);

	// Clear shadow depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	// Bind shader
	m_Resources.m_Shaders["Shadow"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	m_Resources.m_Shaders["Shadow"].SetUniform("uLightSpaceMatrix", const_cast<glm::mat4&>(m_Light.m_Transform));

	for (const auto& model : Objects)
	{
		auto& SubMeshes = m_Resources.m_Models[model.first].GetSubMeshes();

		for (auto& submesh : SubMeshes)
		{
			// Set model vbo handle to vao
			glVertexArrayVertexBuffer(m_VAO, 0, submesh.m_VBO, 0, sizeof(Model::Vertex));
			glVertexArrayElementBuffer(m_VAO, submesh.m_EBO);

			for (const auto& transform : model.second)
			{
				m_Resources.m_Shaders["Shadow"].SetUniform("uModel", const_cast<glm::mat4&>(transform));

				if (bone_transforms && model.first == "Character")
				{
					m_Resources.m_Shaders["Shadow"].SetUniform("uHasBones", true);
					auto loc = glGetUniformLocation(m_Resources.m_Shaders["Shadow"].GetShaderHandle(), "uFinalBonesMatrices");
					glUniformMatrix4fv(loc, 100, GL_FALSE, glm::value_ptr((*bone_transforms)[0]));
				}

				else
				{
					m_Resources.m_Shaders["Shadow"].SetUniform("uHasBones", false);
				}

				// Draw object
				glDrawElements(m_Resources.m_Models[model.first].GetPrimitive(), submesh.m_DrawCount, GL_UNSIGNED_SHORT, NULL);
			}
		}
	}

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Shadow"].UnUse();

	glCullFace(GL_BACK);
}

void Renderer::RenderPass(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects, std::vector<glm::mat4>* bone_transforms)
{
	// Bind to lighting frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightingBuffer.m_FrameBuffer[0]);

	// Change to fit window size
	glViewport(0, 0, 1280, 720);

	// Clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader
	m_Resources.m_Shaders["Light"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	glm::mat4 view = Camera::GetInstanced().GetView();
	glm::mat4 projection = Camera::GetInstanced().GetProjection();
	glm::vec3 position = Camera::GetInstanced().GetPosition();

	m_Resources.m_Shaders["Light"].SetUniform("uView", const_cast<glm::mat4&>(view));
	m_Resources.m_Shaders["Light"].SetUniform("uProjection", const_cast<glm::mat4&>(projection));
	m_Resources.m_Shaders["Light"].SetUniform("uCameraPosition", const_cast<glm::vec3&>(position));

	glBindTextureUnit(4, m_ShadowBuffer.m_BufferTexture[0]);
	m_Resources.m_Shaders["Light"].SetUniform("uShadowMap", 4);

	m_Resources.m_Shaders["Light"].SetUniform("uLight.Direction", m_Light.m_Direction);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Ambient", 0.2f, 0.2f, 0.2f);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Diffuse", 0.5f, 0.5f, 0.5f);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Specular", 1.0f, 1.0f, 1.0f);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Transform", const_cast<glm::mat4&>(m_Light.m_Transform));

	for (const auto& model : Objects)
	{
		auto& SubMeshes = m_Resources.m_Models[model.first].GetSubMeshes();

		for (auto& submesh : SubMeshes)
		{
			Model::Material& material = m_Resources.m_Materials[submesh.m_Material];

			// Send textures
			if (!material.m_Diffuse.empty())
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedDiffuse", true);

				glBindTextureUnit(0, m_Resources.m_Textures[material.m_Diffuse]);
				m_Resources.m_Shaders["Light"].SetUniform("uMat.Diffuse", 0);
			}
			else
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedDiffuse", false);
			}

			if (!material.m_Ambient.empty())
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedAmbient", true);

				glBindTextureUnit(1, m_Resources.m_Textures[material.m_Ambient]);
				m_Resources.m_Shaders["Light"].SetUniform("uMat.Ambient", 1);
			}
			else
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedAmbient", false);
			}

			if (!material.m_Specular.empty())
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedSpecular", true);

				glBindTextureUnit(2, m_Resources.m_Textures[material.m_Specular]);
				m_Resources.m_Shaders["Light"].SetUniform("uMat.Specular", 2);
			}
			else
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedSpecular", false);
			}

			if (!material.m_Normal.empty())
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedNormal", true);

				glBindTextureUnit(3, m_Resources.m_Textures[material.m_Normal]);
				m_Resources.m_Shaders["Light"].SetUniform("uMat.Normal", 3);
			}
			else
			{
				m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedNormal", false);
			}

			// Set model vbo handle to vao
			glVertexArrayVertexBuffer(m_VAO, 0, submesh.m_VBO, 0, sizeof(Model::Vertex));
			glVertexArrayElementBuffer(m_VAO, submesh.m_EBO);

			for (const auto& transform : model.second)
			{
				m_Resources.m_Shaders["Light"].SetUniform("uModel", const_cast<glm::mat4&>(transform));

				if (bone_transforms && model.first == "Character")
				{
					m_Resources.m_Shaders["Light"].SetUniform("uHasBones", true);
					auto loc = glGetUniformLocation(m_Resources.m_Shaders["Light"].GetShaderHandle(), "uFinalBonesMatrices");
					glUniformMatrix4fv(loc, 100, GL_FALSE, glm::value_ptr((*bone_transforms)[0]));
				}

				else
				{
					m_Resources.m_Shaders["Light"].SetUniform("uHasBones", false);
				}

				// Draw object
				glDrawElements(m_Resources.m_Models[model.first].GetPrimitive(), submesh.m_DrawCount, GL_UNSIGNED_SHORT, NULL);
			}
		}

	}

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Light"].UnUse();
}

void Renderer::BlurPass()
{
	glDisable(GL_DEPTH_TEST);
	bool horizontal = true, first = true;

	// Bind shader
	m_Resources.m_Shaders["Blur"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Set screen model
	const auto& screen = m_Resources.m_Models["Screen"];
	glVertexArrayVertexBuffer(m_VAO, 0, screen.GetSubMeshes()[0].m_VBO, 0, sizeof(Model::Vertex));
	glVertexArrayElementBuffer(m_VAO, screen.GetSubMeshes()[0].m_EBO);

	for (size_t i = 0; i < 10; ++i)
	{
		// Bind blur frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_BlurBuffer.m_FrameBuffer[horizontal]);
		m_Resources.m_Shaders["Blur"].SetUniform("uHorizontal", horizontal);

		// Bind texture to blur
		glBindTextureUnit(0, first ? m_LightingBuffer.m_BufferTexture[1] : m_BlurBuffer.m_BufferTexture[!horizontal]);
		m_Resources.m_Shaders["Blur"].SetUniform("uImage", 0);

		glDrawElements(screen.GetPrimitive(), screen.GetSubMeshes()[0].m_DrawCount, GL_UNSIGNED_SHORT, NULL);

		// Flip orientation
		horizontal = !horizontal;

		if (first)
			first = false;
	}

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Blur"].UnUse();
}

void Renderer::CompositePass()
{
	// Bind default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader
	m_Resources.m_Shaders["Composite"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Set screen model
	const auto& screen = m_Resources.m_Models["Screen"];
	glVertexArrayVertexBuffer(m_VAO, 0, screen.GetSubMeshes()[0].m_VBO, 0, sizeof(Model::Vertex));
	glVertexArrayElementBuffer(m_VAO, screen.GetSubMeshes()[0].m_EBO);

	glBindTextureUnit(0, m_LightingBuffer.m_BufferTexture[0]);
	m_Resources.m_Shaders["Composite"].SetUniform("uImage", 0);

	glBindTextureUnit(1, m_BlurBuffer.m_BufferTexture[0]);
	m_Resources.m_Shaders["Composite"].SetUniform("uBlur", 1);

	m_Resources.m_Shaders["Composite"].SetUniform("uExposure", 1.f);

	glDrawElements(screen.GetPrimitive(), screen.GetSubMeshes()[0].m_DrawCount, GL_UNSIGNED_SHORT, NULL);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Composite"].UnUse();

	glEnable(GL_DEPTH_TEST);
}

void Renderer::StartFrame()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// Clear back buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GLfloat one = 1.f;
	//glClearBufferfv(GL_DEPTH, 0, &one);
}

Renderer& Renderer::GetInstanced()
{
	static Renderer renderer;

	return renderer;
}