#include "Renderer.h"
#include "paperback_camera.h"

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

	glBindVertexArray(0);

	glCreateVertexArrays(1, &m_DebugVAO);

	glEnableVertexArrayAttrib(m_DebugVAO, 0);
	glVertexArrayAttribFormat(m_DebugVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_DebugVAO, 0, 0);

	glBindVertexArray(0);

	m_Resources.LoadShader("Light", "../../resources/shaders/Lighting.vert", "../../resources/shaders/Lighting.frag");
	m_Resources.LoadShader("Debug", "../../resources/shaders/Debug.vert", "../../resources/shaders/Debug.frag");
	m_Resources.Load3DMesh("Backpack", "../../resources/models/backpack.obj");
	m_Resources.Load3DMesh("Box", "../../resources/models/box.fbx");
	m_Resources.Load3DMesh("Plane", "../../resources/models/plane.obj");

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	// Enable face cullling
	glEnable(GL_CULL_FACE);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_DebugVAO);
}

void Renderer::Render(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects)
{
	// Bind shader
	m_Resources.m_Shaders["Light"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	glm::mat4 view = Camera::GetInstanced().GetView();
	glm::mat4 projection = Camera::GetInstanced().GetProjection();

	m_Resources.m_Shaders["Light"].SetUniform("uView", const_cast<glm::mat4&>(view));
	m_Resources.m_Shaders["Light"].SetUniform("uProjection", const_cast<glm::mat4&>(projection));

	m_Resources.m_Shaders["Light"].SetUniform("uLight.Direction", 0.f, -1.f, 0.f);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Ambient", 0.2f, 0.2f, 0.2f);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Diffuse", 0.5f, 0.5f, 0.5f);
	m_Resources.m_Shaders["Light"].SetUniform("uLight.Specular", 1.0f, 1.0f, 1.0f);

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

void Renderer::DebugRender(const std::vector<glm::vec3>& Points)
{
	// Create vbo for debug lines
	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(glm::vec3) * Points.size(), Points.data(), GL_DYNAMIC_STORAGE_BIT);

	// Bind shader
	m_Resources.m_Shaders["Debug"].Use();

	// Bind vao
	glBindVertexArray(m_DebugVAO);
	// Bind vbo to vao
	glVertexArrayVertexBuffer(m_DebugVAO, 0, vbo, 0, sizeof(glm::vec3));

	glm::mat4 transform = Camera::GetInstanced().GetProjection() * Camera::GetInstanced().GetView();

	m_Resources.m_Shaders["Debug"].SetUniform("uTransform", transform);

	glLineWidth(5.f);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Points.size()));

	glLineWidth(1.f);

	// Destroy vbo
	glDeleteBuffers(1, &vbo);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Debug"].UnUse();
}

void Renderer::StartFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat one = 1.f;
	glClearBufferfv(GL_DEPTH, 0, &one);
}

Renderer& Renderer::GetInstanced()
{
	static Renderer renderer;

	return renderer;
}