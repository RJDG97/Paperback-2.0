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
	glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Position));
	glVertexArrayAttribBinding(m_VAO, 0, 0);

	glEnableVertexArrayAttrib(m_VAO, 1);
	glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Normal));
	glVertexArrayAttribBinding(m_VAO, 1, 0);

	glEnableVertexArrayAttrib(m_VAO, 2);
	glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_UV));
	glVertexArrayAttribBinding(m_VAO, 2, 0);

	glBindVertexArray(0);

	m_Resources.LoadShader("Simple", "Simple.vert", "Simple.frag");

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

	m_Resources.m_Shaders["Simple"].SetUniform("uView", const_cast<glm::mat4&>(view));
	m_Resources.m_Shaders["Simple"].SetUniform("uProjection", const_cast<glm::mat4&>(projection));

	// Loop through all models 
	for (const auto& object : Objects)
	{
		// Get model's submesh
		auto& SubMeshes = m_Resources.m_Models[object.first].GetSubMeshes();

		// Render all model's submeshes
		for (auto& submesh : SubMeshes)
		{
			m_Resources.m_Shaders["Simple"].SetUniform("uTexturedDiffuse", false);

			// Set model submesh's textures
			/*if (m_Resources.m_Textures.find(Texture) == m_Resources.m_Textures.end())
			{
				m_Resources.m_Shaders["Simple"].SetUniform("uTextured", false);
			}
			else
			{
				m_Resources.m_Shaders["Simple"].SetUniform("uTextured", true);
				glBindTextureUnit(0, m_Resources.m_Textures[Texture]);
				m_Resources.m_Shaders["Simple"].SetUniform("uTex", 0);
			}*/

			// Set model vbo handle to vao
			glVertexArrayVertexBuffer(m_VAO, 0, submesh.m_VBO, 0, sizeof(Vertex));
			glVertexArrayElementBuffer(m_VAO, submesh.m_EBO);

			// Loop through each unique transform of current model
			for (const auto& transform : object.second)
			{
				// Sent transform
				m_Resources.m_Shaders["Simple"].SetUniform("uModel", const_cast<glm::mat4&>(transform));

				// Draw object
				glDrawElements(m_Resources.m_Models[object.first].GetPrimitive(), submesh.m_DrawCount, GL_UNSIGNED_SHORT, NULL);
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