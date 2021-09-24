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

	m_Resources.LoadShader("Simple", "../../resources/shaders/Simple.vert", "../../resources/shaders/Simple.frag");
	m_Resources.Load3DMesh("Backpack", "../../resources/models/backpack.obj");

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void Renderer::Render(const std::unordered_map<std::string, std::vector<glm::mat4>>& Objects)
{
	// Bind shader
	m_Resources.m_Shaders["Simple"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Sent view and projection matrix
	glm::mat4 view = Camera::GetInstanced().GetView();
	glm::mat4 projection = Camera::GetInstanced().GetProjection();

	//RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uModel", const_cast<glm::mat4&>(model));
	RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uView", const_cast<glm::mat4&>(view));
	RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uProjection", const_cast<glm::mat4&>(projection));

	for (const auto& model : Objects)
	{
		auto& SubMeshes = RenderResourceManager::GetInstanced().m_Models[model.first].GetSubMeshes();

		for (auto& submesh : SubMeshes)
		{
			Model::Material& material = RenderResourceManager::GetInstanced().m_Materials[submesh.m_Material];
			// Send textures
			if (!material.m_Diffuse.empty())
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedDiffuse", true);

				glBindTextureUnit(0, RenderResourceManager::GetInstanced().m_Textures[material.m_Diffuse]);
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uDiffuse", 0);
			}
			else
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedDiffuse", false);
			}

			/*if (!material.m_Ambient.empty())
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedAmbient", true);

				glBindTextureUnit(1, RenderResourceManager::GetInstanced().m_Textures[material.m_Ambient]);
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uAmbient", 1);
			}
			else
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedAmbient", false);
			}

			if (!material.m_Specular.empty())
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedSpecular", true);

				glBindTextureUnit(2, RenderResourceManager::GetInstanced().m_Textures[material.m_Specular]);
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uSpecular", 2);
			}
			else
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedSpecular", false);
			}

			if (!material.m_Normal.empty())
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedNormal", true);

				glBindTextureUnit(3, RenderResourceManager::GetInstanced().m_Textures[material.m_Normal]);
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uMat.Normal", 3);
			}
			else
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uTexturedNormal", false);
			}*/

			// Set model vbo handle to vao
			glVertexArrayVertexBuffer(m_VAO, 0, submesh.m_VBO, 0, sizeof(Model::Vertex));
			glVertexArrayElementBuffer(m_VAO, submesh.m_EBO);

			for (const auto& transform : model.second)
			{
				RenderResourceManager::GetInstanced().m_Shaders["Simple"].SetUniform("uModel", const_cast<glm::mat4&>(transform));

				// Draw object
				glDrawElements(RenderResourceManager::GetInstanced().m_Models[model.first].GetPrimitive(), submesh.m_DrawCount, GL_UNSIGNED_SHORT, NULL);
			}
		}
	}

	// Model transformation
	//glm::mat4 model{ 1.0f };
	//model = glm::translate(model, Position);
	//model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.f, 0.f, 0.f));
	//model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.f, 1.f, 0.f));
	//model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.f, 0.f, 1.f));
	//model = glm::scale(model, Scale);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Simple"].UnUse();
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