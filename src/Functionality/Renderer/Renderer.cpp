#include "Renderer.h"
#include "paperback_camera.h"
#include <glm/inc/gtx/transform.hpp>

// Converted to std::string_view from std::string on these lines:
// std::string( model.first ) - Performs an implicit conversion from std::string_view to std::string - Basic String Constructor (10)
// 190, 245, 267, 292, 306, 342, 418

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

	std::vector<std::string> files = { "../../resources/textures/right.dds",
								   "../../resources/textures/left.dds",
								   "../../resources/textures/top.dds",
								   "../../resources/textures/bottom.dds",
								   "../../resources/textures/front.dds",
								   "../../resources/textures/back.dds" };

	RenderResourceManager::GetInstanced().LoadSkyboxTexture(files);

	m_Resources.LoadShader("Shadow", "../../resources/shaders/Shadow.vert", "../../resources/shaders/Shadow.frag");
	m_Resources.LoadShader("Light", "../../resources/shaders/Lighting.vert", "../../resources/shaders/Lighting.frag");
	m_Resources.LoadShader("Blur", "../../resources/shaders/SimplePassthrough.vert", "../../resources/shaders/Blur.frag");
	m_Resources.LoadShader("Composite", "../../resources/shaders/SimplePassthrough.vert", "../../resources/shaders/Composite.frag");
	m_Resources.LoadShader("Final", "../../resources/shaders/SimplePassthrough.vert", "../../resources/shaders/Final.frag");
	m_Resources.LoadShader("Skybox", "../../resources/shaders/Skybox.vert", "../../resources/shaders/Skybox.frag");
	m_Resources.LoadShader("Debug", "../../resources/shaders/Debug.vert", "../../resources/shaders/Debug.frag");
	m_Resources.LoadShader("UI", "../../resources/shaders/UI.vert", "../../resources/shaders/UI.frag");
	m_Resources.LoadShader("Text", "../../resources/shaders/Text.vert", "../../resources/shaders/Text.frag");

	m_Resources.Load3DMeshNUI("RedUnitAnimated", "../../resources/models/nui/RedUnitAnimated.nui");
	m_Resources.Load3DMeshNUI("BlueUnitAnimated", "../../resources/models/nui/BlueUnitAnimated.nui");

	m_Resources.Load3DMeshNUI("BadmintonRacketAnimated_Blue", "../../resources/models/nui/BadmintonRacketAnimated_Blue.nui");
	m_Resources.Load3DMeshNUI("BadmintonRacketAnimated_Red", "../../resources/models/nui/BadmintonRacketAnimated_Red.nui");

	m_Resources.Load3DMeshNUI("TableTennisRacket_Red", "../../resources/models/nui/TableTennisRacket_Red.nui");
	m_Resources.Load3DMeshNUI("TableTennisRacket_Blue", "../../resources/models/nui/TableTennisRacket_Blue.nui");
	
	m_Resources.Load3DMeshNUI("SwimmingBoardShield_Red", "../../resources/models/nui/SwimmingBoardShield_Red.nui");
	m_Resources.Load3DMeshNUI("SwimmingBoardShield_Blue", "../../resources/models/nui/SwimmingBoardShield_Blue.nui");
	
	m_Resources.Load3DMeshNUI("HelmetShield_Red", "../../resources/models/nui/HelmetShield_Red.nui");
	m_Resources.Load3DMeshNUI("HelmetShield_Blue", "../../resources/models/nui/HelmetShield_Blue.nui");
	
	m_Resources.Load3DMeshNUI("FrisbeeShield_Blue", "../../resources/models/nui/FrisbeeShield_Blue.nui");
	m_Resources.Load3DMeshNUI("FrisbeeShield_Red", "../../resources/models/nui/FrisbeeShield_Red.nui");
	
	m_Resources.Load3DMeshNUI("FloorballStickAnimated_Red", "../../resources/models/nui/FloorballStickAnimated_Red.nui");
	m_Resources.Load3DMeshNUI("FloorballStickAnimated_Blue", "../../resources/models/nui/FloorballStickAnimated_Blue.nui");
	
	m_Resources.Load3DMeshNUI("TopPathSlope", "../../resources/models/nui/TopPathSlope.nui");
	m_Resources.Load3DMeshNUI("TopPath", "../../resources/models/nui/TopPath.nui");

	m_Resources.Load3DMeshNUI("Sandpit", "../../resources/models/nui/Sandpit.nui");
	m_Resources.Load3DMeshNUI("PlayerBaseMound", "../../resources/models/nui/PlayerBaseMound.nui");

	m_Resources.Load3DMeshNUI("NationalStadium", "../../resources/models/nui/NationalStadium.nui");
	m_Resources.Load3DMeshNUI("HDBBlock", "../../resources/models/nui/HDBBlock.nui");

	m_Resources.Load3DMeshNUI("DragonTailRight", "../../resources/models/nui/DragonTailRight.nui");
	m_Resources.Load3DMeshNUI("DragonTailLeft", "../../resources/models/nui/DragonTailLeft.nui");

	m_Resources.Load3DMeshNUI("DragonSpineSupport", "../../resources/models/nui/DragonSpineSupport.nui");
	m_Resources.Load3DMeshNUI("DragonSpine", "../../resources/models/nui/DragonSpine.nui");

	m_Resources.Load3DMeshNUI("DragonSlideLeft", "../../resources/models/nui/DragonSlideLeft.nui");
	m_Resources.Load3DMeshNUI("DragonSlideRight", "../../resources/models/nui/DragonSlideRight.nui");

	m_Resources.Load3DMeshNUI("DragonHead", "../../resources/models/nui/DragonHead.nui");
	m_Resources.Load3DMeshNUI("BottomPath", "../../resources/models/nui/BottomPath.nui");

	m_Resources.LoadFonts("arial", "../../resources/fonts/arial");

	// Cards
	m_Resources.LoadTextures("CardBack", "../../resources/textures/UI/Cards/CardBack.dds", true);
	m_Resources.LoadTextures("PaperWeapon_PaperShield", "../../resources/textures/UI/Cards/PaperWeapon_PaperShield.dds", true);
	m_Resources.LoadTextures("RockWeapon_RockShield", "../../resources/textures/UI/Cards/RockWeapon_RockShield.dds", true);
	m_Resources.LoadTextures("ScissorsWeapon_ScissorsShield", "../../resources/textures/UI/Cards/ScissorsWeapon_ScissorsShield.dds", true);

	// Health Bars
		// Enemy
	m_Resources.LoadTextures("EnemyHPBar", "../../resources/textures/UI/HealthBars/EnemyHPBar.dds", true);
	m_Resources.LoadTextures("EnemyHPBar_Background", "../../resources/textures/UI/HealthBars/EnemyHPBar_Background.dds", true);
	m_Resources.LoadTextures("EnemyHPBar_Fill", "../../resources/textures/UI/HealthBars/EnemyHPBar_Fill.dds", true);
	
		// Player
	m_Resources.LoadTextures("PlayerHPBar", "../../resources/textures/UI/HealthBars/PlayerHPBar.dds", true);
	m_Resources.LoadTextures("PlayerHPBar_Background", "../../resources/textures/UI/HealthBars/PlayerHPBar_Background.dds", true);
	m_Resources.LoadTextures("PlayerHPBar_Fill", "../../resources/textures/UI/HealthBars/PlayerHPBar_Fill.dds", true);

	// RPS Icons
	m_Resources.LoadTextures("BluePaperRedRock", "../../resources/textures/UI/RPSStatusIcons/BluePaperRedRock.dds", true);
	m_Resources.LoadTextures("BlueRockRedScissors", "../../resources/textures/UI/RPSStatusIcons/BlueRockRedScissors.dds", true);
	m_Resources.LoadTextures("BlueScissorsRedPaper", "../../resources/textures/UI/RPSStatusIcons/BlueScissorsRedPaper.dds", true);
	m_Resources.LoadTextures("NeutralPaper", "../../resources/textures/UI/RPSStatusIcons/NeutralPaper.dds", true);
	m_Resources.LoadTextures("NeutralRock", "../../resources/textures/UI/RPSStatusIcons/NeutralRock.dds", true);
	m_Resources.LoadTextures("NeutralScissors", "../../resources/textures/UI/RPSStatusIcons/NeutralScissors.dds", true);
	m_Resources.LoadTextures("RedBannerPaperPaper", "../../resources/textures/UI/RPSStatusIcons/RedBannerPaperPaper.dds", true);
	m_Resources.LoadTextures("RedBannerRockRock", "../../resources/textures/UI/RPSStatusIcons/RedBannerRockRock.dds", true);
	m_Resources.LoadTextures("RedBannerScissorsScissors", "../../resources/textures/UI/RPSStatusIcons/RedBannerScissorsScissors.dds", true);
	m_Resources.LoadTextures("RedPaperBlueRock", "../../resources/textures/UI/RPSStatusIcons/RedPaperBlueRock.dds", true);
	m_Resources.LoadTextures("RedRockBlueScissors", "../../resources/textures/UI/RPSStatusIcons/RedRockBlueScissors.dds", true);
	m_Resources.LoadTextures("RedScissorsBluePaper", "../../resources/textures/UI/RPSStatusIcons/RedScissorsBluePaper.dds", true);
	m_Resources.LoadTextures("RockPaperScissorsIcon", "../../resources/textures/UI/RPSStatusIcons/RockPaperScissorsIcon.dds", true);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable face cullling
	glEnable(GL_CULL_FACE);

	// Set up framebuffers
	//SetUpFramebuffer();

	m_Light.m_Position = glm::vec3{ 1.f, 1.f, 1.f };
	m_Light.m_Direction = glm::normalize(glm::vec3{ 0.f } - m_Light.m_Position);
	m_Light.m_Projection = glm::ortho(-50.f, 50.f, -50.f, 50.f, -50.f, 50.f);
	m_Light.m_View = glm::lookAt(m_Light.m_Position, glm::vec3{ 0.f }, glm::vec3{ 0.f, 1.f, 0.f });
	m_Light.m_Transform = m_Light.m_Projection * m_Light.m_View;
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_DebugVAO);
	
	glDeleteFramebuffers( static_cast<GLsizei>( m_ShadowBuffer.m_FrameBuffer.size() ), m_ShadowBuffer.m_FrameBuffer.data());
	glDeleteTextures( static_cast<GLsizei>( m_ShadowBuffer.m_BufferTexture.size() ), m_ShadowBuffer.m_BufferTexture.data());

	glDeleteFramebuffers( static_cast<GLsizei>( m_LightingBuffer.m_FrameBuffer.size() ), m_LightingBuffer.m_FrameBuffer.data());
	glDeleteTextures( static_cast<GLsizei>( m_LightingBuffer.m_BufferTexture.size() ), m_LightingBuffer.m_BufferTexture.data());
	glDeleteRenderbuffers(1, &m_LightingBuffer.m_RenderBuffer);

	glDeleteFramebuffers( static_cast<GLsizei>( m_BlurBuffer.m_FrameBuffer.size() ), m_BlurBuffer.m_FrameBuffer.data());
	glDeleteTextures( static_cast<GLsizei>( m_BlurBuffer.m_BufferTexture.size() ), m_BlurBuffer.m_BufferTexture.data());

	glDeleteFramebuffers( static_cast<GLsizei>( m_UIBuffer.m_FrameBuffer.size() ), m_UIBuffer.m_FrameBuffer.data());
	glDeleteTextures( static_cast<GLsizei>( m_UIBuffer.m_BufferTexture.size() ), m_UIBuffer.m_BufferTexture.data());
	glDeleteRenderbuffers(1, &m_UIBuffer.m_RenderBuffer);

	glDeleteFramebuffers( static_cast<GLsizei>( m_FinalBuffer.m_FrameBuffer.size() ), m_FinalBuffer.m_FrameBuffer.data());
	glDeleteTextures( static_cast<GLsizei>( m_FinalBuffer.m_BufferTexture.size() ), m_FinalBuffer.m_BufferTexture.data());
	glDeleteRenderbuffers(1, &m_FinalBuffer.m_RenderBuffer);
}

void Renderer::SetUpFramebuffer(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;

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

	// Create light framebuffer
	GLuint lightFbo;
	glCreateFramebuffers(1, &lightFbo);

	// Create light textures
	GLuint lightTexture[2];
	glCreateTextures(GL_TEXTURE_2D, 2, lightTexture);

	for (size_t i = 0; i < 2; ++i)
	{
		glTextureStorage2D(lightTexture[i], 1, GL_RGBA16F, Width, Height);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to framebuffer
		glNamedFramebufferTexture(lightFbo, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), lightTexture[i], 0);

		// Add texture to framebuffer object
		m_LightingBuffer.m_BufferTexture.push_back(lightTexture[i]);
	}

	// Create light render buffer 
	GLuint lightRbo;
	glCreateRenderbuffers(1, &lightRbo);
	glNamedRenderbufferStorage(lightRbo, GL_DEPTH_COMPONENT32F, Width, Height);
	glNamedFramebufferRenderbuffer(lightFbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightRbo);

	GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glNamedFramebufferDrawBuffers(lightFbo, 2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Add framebuffer and texture to framebuffer object
	m_LightingBuffer.m_FrameBuffer.push_back(lightFbo);
	m_LightingBuffer.m_RenderBuffer = lightRbo;

	// Create blur framebuffer
	GLuint blurFBO[2];
	glCreateFramebuffers(2, blurFBO);

	// Create blur texture
	GLuint blurTexture[2];
	glCreateTextures(GL_TEXTURE_2D, 2, blurTexture);

	for (size_t i = 0; i < 2; ++i)
	{
		glTextureStorage2D(blurTexture[i], 1, GL_RGBA16F, Width, Height);
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

	// Create ui framebuffer
	GLuint uiFbo;
	glCreateFramebuffers(1, &uiFbo);

	// Create ui texture
	GLuint uiTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &uiTexture);

	glTextureStorage2D(uiTexture, 1, GL_RGBA16F, Width, Height);
	glTextureParameteri(uiTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(uiTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(uiTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(uiTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach texture to framebuffer
	glNamedFramebufferTexture(uiFbo, GL_COLOR_ATTACHMENT0, uiTexture, 0);

	// Create ui render buffer 
	GLuint uiRbo;
	glCreateRenderbuffers(1, &uiRbo);
	glNamedRenderbufferStorage(uiRbo, GL_DEPTH_COMPONENT32F, Width, Height);
	glNamedFramebufferRenderbuffer(uiFbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, uiRbo);

	m_UIBuffer.m_FrameBuffer.push_back(uiFbo);
	m_UIBuffer.m_BufferTexture.push_back(uiTexture);
	m_UIBuffer.m_RenderBuffer = uiRbo;

	// Create final framebuffer
	GLuint finalFbo;
	glCreateFramebuffers(1, &finalFbo);

	// Create final texture
	GLuint finalTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &finalTexture);

	glTextureStorage2D(finalTexture, 1, GL_RGBA16F, Width, Height);
	glTextureParameteri(finalTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(finalTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(finalTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(finalTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach texture to framebuffer
	glNamedFramebufferTexture(finalFbo, GL_COLOR_ATTACHMENT0, finalTexture, 0);

	// Create final render buffer 
	GLuint finalRbo;
	glCreateRenderbuffers(1, &finalRbo);
	glNamedRenderbufferStorage(finalRbo, GL_DEPTH_COMPONENT32F, m_Width, m_Height);
	glNamedFramebufferRenderbuffer(finalFbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, finalRbo);

	m_FinalBuffer.m_FrameBuffer.push_back(finalFbo);
	m_FinalBuffer.m_BufferTexture.push_back(finalTexture);
	m_FinalBuffer.m_RenderBuffer = finalRbo;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::UpdateFramebufferSize(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;

	// Delete old light buffer textures
	glDeleteTextures(static_cast<GLsizei>(m_LightingBuffer.m_BufferTexture.size()), m_LightingBuffer.m_BufferTexture.data());
	m_LightingBuffer.m_BufferTexture.clear();

	// Delete old light buffer rbo
	glDeleteRenderbuffers(1, &m_LightingBuffer.m_RenderBuffer);
	m_LightingBuffer.m_RenderBuffer = 0;

	// Create light textures
	GLuint lightTexture[2];
	glCreateTextures(GL_TEXTURE_2D, 2, lightTexture);

	for (size_t i = 0; i < 2; ++i)
	{
		glTextureStorage2D(lightTexture[i], 1, GL_RGBA16F, Width, Height);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(lightTexture[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to framebuffer
		glNamedFramebufferTexture(m_LightingBuffer.m_FrameBuffer[0], static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), lightTexture[i], 0);

		// Add texture to framebuffer object
		m_LightingBuffer.m_BufferTexture.push_back(lightTexture[i]);
	}

	// Create light render buffer 
	GLuint lightRbo;
	glCreateRenderbuffers(1, &lightRbo);
	glNamedRenderbufferStorage(lightRbo, GL_DEPTH_COMPONENT32F, Width, Height);
	glNamedFramebufferRenderbuffer(m_LightingBuffer.m_FrameBuffer[0], GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightRbo);
	m_LightingBuffer.m_RenderBuffer = lightRbo;

	// Delete old blur buffer textures
	glDeleteTextures(static_cast<GLsizei>(m_BlurBuffer.m_BufferTexture.size()), m_BlurBuffer.m_BufferTexture.data());
	m_BlurBuffer.m_BufferTexture.clear();

	// Create blur texture
	GLuint blurTexture[2];
	glCreateTextures(GL_TEXTURE_2D, 2, blurTexture);

	for (size_t i = 0; i < 2; ++i)
	{
		glTextureStorage2D(blurTexture[i], 1, GL_RGBA16F, Width, Height);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(blurTexture[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to framebuffer
		glNamedFramebufferTexture(m_BlurBuffer.m_FrameBuffer[i], GL_COLOR_ATTACHMENT0, blurTexture[i], 0);

		// Add texture to framebuffer object
		m_BlurBuffer.m_BufferTexture.push_back(blurTexture[i]);
	}

	// Delete old ui buffer textures
	glDeleteTextures(static_cast<GLsizei>(m_UIBuffer.m_BufferTexture.size()), m_UIBuffer.m_BufferTexture.data());
	m_UIBuffer.m_BufferTexture.clear();

	// Delete old ui buffer rbo
	glDeleteRenderbuffers(1, &m_UIBuffer.m_RenderBuffer);
	m_UIBuffer.m_RenderBuffer = 0;

	// Create ui texture
	GLuint uiTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &uiTexture);

	glTextureStorage2D(uiTexture, 1, GL_RGBA16F, Width, Height);
	glTextureParameteri(uiTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(uiTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(uiTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(uiTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach texture to framebuffer
	glNamedFramebufferTexture(m_UIBuffer.m_FrameBuffer[0], GL_COLOR_ATTACHMENT0, uiTexture, 0);

	// Add texture to framebuffer object
	m_UIBuffer.m_BufferTexture.push_back(uiTexture);

	// Create ui render buffer 
	GLuint uiRbo;
	glCreateRenderbuffers(1, &uiRbo);
	glNamedRenderbufferStorage(uiRbo, GL_DEPTH_COMPONENT32F, Width, Height);
	glNamedFramebufferRenderbuffer(m_UIBuffer.m_FrameBuffer[0], GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, uiRbo);
	m_UIBuffer.m_RenderBuffer = uiRbo;

	// Delete old final buffer textures
	glDeleteTextures(static_cast<GLsizei>(m_FinalBuffer.m_BufferTexture.size()), m_FinalBuffer.m_BufferTexture.data());
	m_FinalBuffer.m_BufferTexture.clear();

	// Delete old final buffer rbo
	glDeleteRenderbuffers(1, &m_FinalBuffer.m_RenderBuffer);
	m_FinalBuffer.m_RenderBuffer = 0;

	// Create final texture
	GLuint finalTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &finalTexture);

	glTextureStorage2D(finalTexture, 1, GL_RGBA16F, Width, Height);
	glTextureParameteri(finalTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(finalTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(finalTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(finalTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach texture to framebuffer
	glNamedFramebufferTexture(m_FinalBuffer.m_FrameBuffer[0], GL_COLOR_ATTACHMENT0, finalTexture, 0);
	
	// Add texture to framebuffer object
	m_FinalBuffer.m_BufferTexture.push_back(finalTexture);

	// Create final render buffer 
	GLuint finalRbo;
	glCreateRenderbuffers(1, &finalRbo);
	glNamedRenderbufferStorage(finalRbo, GL_DEPTH_COMPONENT32F, Width, Height);
	glNamedFramebufferRenderbuffer(m_FinalBuffer.m_FrameBuffer[0], GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, finalRbo);
	m_FinalBuffer.m_RenderBuffer = finalRbo;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Render(const std::unordered_map<std::string_view, std::vector<Renderer::TransformInfo>>& Objects, const std::unordered_map<std::string_view, std::vector<glm::mat4>>& UIs, const std::unordered_map<std::string_view, std::vector<std::pair<std::string, glm::mat4>>>& Texts, const std::array<std::vector<glm::vec3>, 2>* Points)
{
	// Bind to ui frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_UIBuffer.m_FrameBuffer[0]);
	UIPass(UIs);
	TextPass(Texts);

	// Bind shadow frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowBuffer.m_FrameBuffer[0]);
	// Render shadows
	ShadowPass(Objects);

	// Bind to lighting frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightingBuffer.m_FrameBuffer[0]);
	// Render objects
	RenderPass(Objects);	
	// Render skybox
	SkyBoxRender();

	// Disable for post processing
	glDisable(GL_DEPTH_TEST);

	// Blur bright image
	BlurPass();
	// Merge blur and original image
	CompositePass();

	// Enable for UI and debug rendering
	glEnable(GL_DEPTH_TEST);

	// Render debug points
	if (Points)
	{
		DebugRender(*Points);
	}

	// Disable for post processing
	glDisable(GL_DEPTH_TEST);

	// Merge 3D scene and 2D ui
	MergePass();

	// Enable for UI and debug rendering
	glEnable(GL_DEPTH_TEST);

	// Bind default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Display final image to screen
	FinalPass();
}

void Renderer::DebugRender(const std::array<std::vector<glm::vec3>, 2>& Points)
{
	// Bind shader
	m_Resources.m_Shaders["Debug"].Use();

	// Bind vao
	glBindVertexArray(m_DebugVAO);

	glm::mat4 transform = Camera3D::GetInstanced().GetProjection() * Camera3D::GetInstanced().GetView();

	m_Resources.m_Shaders["Debug"].SetUniform("uTransform", transform);

	for (int i = 0; i < Points.size(); ++i)
	{
		if (Points[i].empty())
			continue;

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

void Renderer::SkyBoxRender()
{
	// Bind shader
	m_Resources.m_Shaders["Skybox"].Use();

	// Bind vao
	glBindVertexArray(m_DebugVAO);

	glDepthFunc(GL_LEQUAL);

	glm::mat4 view = glm::mat4(glm::mat3(Camera3D::GetInstanced().GetView()));
	glm::mat4 projection = Camera3D::GetInstanced().GetProjection();

	m_Resources.m_Shaders["Skybox"].SetUniform("uView", const_cast<glm::mat4&>(view));
	m_Resources.m_Shaders["Skybox"].SetUniform("uProjection", const_cast<glm::mat4&>(projection));

	glBindTextureUnit(0, RenderResourceManager::GetInstanced().m_Skybox);
	m_Resources.m_Shaders["Skybox"].SetUniform("uSkybox", 0);

	// Bind vbo to vao
	glVertexArrayVertexBuffer(m_DebugVAO, 0, m_Resources.m_SkyboxVBO, 0, sizeof(glm::vec3));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Skybox"].UnUse();
}

void Renderer::UIPass(const std::unordered_map<std::string_view, std::vector<glm::mat4>>& UIs)
{
	// Clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader
	m_Resources.m_Shaders["UI"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Set quad model
	const auto& quad = m_Resources.m_Models["Quad"];
	glVertexArrayVertexBuffer(m_VAO, 0, quad.GetSubMeshes()[0].m_VBO, 0, sizeof(Model::Vertex));
	glVertexArrayElementBuffer(m_VAO, quad.GetSubMeshes()[0].m_EBO);

	for (const auto& ui : UIs)
	{
		std::string name{ ui.first };

		if (!name.empty())
		{
			const auto& texture = m_Resources.m_Textures.find(name);

			if (texture != m_Resources.m_Textures.end())
			{
				m_Resources.m_Shaders["UI"].SetUniform("uTexturedDiffuse", true);

				glBindTextureUnit(0, texture->second);
				m_Resources.m_Shaders["UI"].SetUniform("uDiffuse", 0);
			}
			else
			{
				m_Resources.m_Shaders["UI"].SetUniform("uTexturedDiffuse", false);
			}
		}
		else
		{
			m_Resources.m_Shaders["UI"].SetUniform("uTexturedDiffuse", false);
		}

		for (const auto& transform : ui.second)
		{
			m_Resources.m_Shaders["UI"].SetUniform("uModel", const_cast<glm::mat4&>(transform));

			glDrawElements(quad.GetPrimitive(), quad.GetSubMeshes()[0].m_DrawCount, GL_UNSIGNED_SHORT, NULL);
		}
	}

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["UI"].UnUse();
}

void Renderer::TextPass(const std::unordered_map<std::string_view, std::vector<std::pair<std::string, glm::mat4>>>& Texts)
{
	// Bind shader
	RenderResourceManager::GetInstanced().m_Shaders["Text"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Create the array to store vertice indexes
	std::vector<GLushort> index = { 0,1,2,3,4,5 };

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	glVertexArrayElementBuffer(m_VAO, ebo);

	glm::vec2 uv0{ 0.f, 1.f };
	glm::vec2 uv1{ 0.f, 0.f };
	glm::vec2 uv2{ 1.f, 0.f };
	glm::vec2 uv3{ 1.f, 1.f };

	for (const auto& fonttype : Texts)
	{
		std::string name{ fonttype.first };
		auto& font = m_Resources.m_Fonts[name];

		m_Resources.m_Shaders["Text"].SetUniform("uTextured", true);

		// Bind font atlas
		glBindTextureUnit(0, font.GetTexture());
		m_Resources.m_Shaders["Text"].SetUniform("uFontAtlas", 0);
		m_Resources.m_Shaders["Text"].SetUniform("uImageSize", static_cast<float>(font.GetImageSize()));

		for (const auto& text : fonttype.second)
		{
			m_Resources.m_Shaders["Text"].SetUniform("uTransform", const_cast<glm::mat4&>(text.second));

			float advance = 0;

			for (size_t i = 0; i < text.first.size(); ++i)
			{
				const auto& letter = font.GetLetter(text.first[i]);

				float xpos = (advance + letter.m_Offset.x) / m_Width;
				float ypos = -(letter.m_LetterSize.y + letter.m_Offset.y) / m_Height;

				float w = letter.m_LetterSize.x / m_Width;
				float h = letter.m_LetterSize.y / m_Height;

				glm::vec3 p0{ xpos, ypos, 0 };
				glm::vec3 p1{ xpos + w, ypos, 0 };
				glm::vec3 p2{ xpos, ypos + h, 0 };
				glm::vec3 p3{ xpos + w, ypos + h, 0 };

				std::vector<Model::Vertex> vertices;

				vertices.push_back({ p2, glm::vec3{0,0,1}, uv1, glm::vec3{0}, glm::vec3{0} });
				vertices.push_back({ p0, glm::vec3{0,0,1}, uv0, glm::vec3{0}, glm::vec3{0} });
				vertices.push_back({ p1, glm::vec3{0,0,1}, uv3, glm::vec3{0}, glm::vec3{0} });
				vertices.push_back({ p2, glm::vec3{0,0,1}, uv1, glm::vec3{0}, glm::vec3{0} });
				vertices.push_back({ p1, glm::vec3{0,0,1}, uv3, glm::vec3{0}, glm::vec3{0} });
				vertices.push_back({ p3, glm::vec3{0,0,1}, uv2, glm::vec3{0}, glm::vec3{0} });

				// Create a handle for vbo
				GLuint vbo;
				glCreateBuffers(1, &vbo);

				// Set size of vbo
				glNamedBufferStorage(vbo, sizeof(Model::Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

				glVertexArrayVertexBuffer(m_VAO, 0, vbo, 0, sizeof(Model::Vertex));

				m_Resources.m_Shaders["Text"].SetUniform("uAtlasPosition", const_cast<glm::vec2&>(letter.m_LetterPosition));
				m_Resources.m_Shaders["Text"].SetUniform("uAtlasSize", const_cast<glm::vec2&>(letter.m_LetterSize));

				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index.size()), GL_UNSIGNED_SHORT, NULL);

				glDeleteBuffers(1, &vbo);

				advance += letter.m_Advance;
			}
		}
	}

	glDeleteBuffers(1, &ebo);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	RenderResourceManager::GetInstanced().m_Shaders["Text"].UnUse();
}

void Renderer::ShadowPass(const std::unordered_map<std::string_view, std::vector<TransformInfo>>& Objects)
{
	// Change to fit shadow texture size
	glViewport(0, 0, 1024, 1024);

	// Clear shadow depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);
	// Bind shader
	m_Resources.m_Shaders["Shadow"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	m_Resources.m_Shaders["Shadow"].SetUniform("uLightSpaceMatrix", const_cast<glm::mat4&>(m_Light.m_Transform));

	for (const auto& model : Objects)
	{
		auto& SubMeshes = m_Resources.m_Models[ std::string( model.first ) ].GetSubMeshes();
		
		for (const auto& instance : model.second)
		{
			m_Resources.m_Shaders["Shadow"].SetUniform("uModel", const_cast<glm::mat4&>(instance.m_Transform));

			if (instance.m_BoneTransforms)
			{
				m_Resources.m_Shaders["Shadow"].SetUniform("uHasBones", true);
				m_Resources.m_Shaders["Shadow"].SetUniform("uFinalBonesMatrices", *instance.m_BoneTransforms, 100);
			}
			else
			{
				m_Resources.m_Shaders["Shadow"].SetUniform("uHasBones", false);
			}

			if (instance.m_ParentSocketTransform)
			{
				m_Resources.m_Shaders["Shadow"].SetUniform("uHasSocketed", true);
				m_Resources.m_Shaders["Shadow"].SetUniform("uParentSocketTransform", *instance.m_ParentSocketTransform);
			}
			else
			{
				m_Resources.m_Shaders["Shadow"].SetUniform("uHasSocketed", false);
			}

			for (auto& submesh : SubMeshes)
			{
				// Set model vbo handle to vao
				glVertexArrayVertexBuffer(m_VAO, 0, submesh.m_VBO, 0, sizeof(Model::Vertex));
				glVertexArrayElementBuffer(m_VAO, submesh.m_EBO);

				// Draw object
				glDrawElements(m_Resources.m_Models[std::string(model.first)].GetPrimitive(), submesh.m_DrawCount, GL_UNSIGNED_SHORT, NULL);
			}
		}
	}

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Shadow"].UnUse();

	//glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

void Renderer::RenderPass(const std::unordered_map<std::string_view, std::vector<TransformInfo>>& Objects)
{
	// Change to fit window size
	glViewport(0, 0, m_Width, m_Height);

	// Clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader
	m_Resources.m_Shaders["Light"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	glm::mat4 view = Camera3D::GetInstanced().GetView();
	glm::mat4 projection = Camera3D::GetInstanced().GetProjection();
	glm::vec3 position = Camera3D::GetInstanced().GetPosition();

	m_Resources.m_Shaders["Light"].SetUniform("uShadowBias", 0.05f);

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
		auto& SubMeshes = m_Resources.m_Models[ std::string( model.first ) ].GetSubMeshes();
		
		for (const auto& instance : model.second)
		{
			m_Resources.m_Shaders["Light"].SetUniform("uModel", const_cast<glm::mat4&>(instance.m_Transform));

			if (instance.m_BoneTransforms)
			{
				m_Resources.m_Shaders["Light"].SetUniform("uHasBones", true);
				m_Resources.m_Shaders["Light"].SetUniform("uFinalBonesMatrices", *instance.m_BoneTransforms, 100);
			}
			else
			{
				m_Resources.m_Shaders["Light"].SetUniform("uHasBones", false);
			}

			if (instance.m_ParentSocketTransform)
			{
				m_Resources.m_Shaders["Light"].SetUniform("uHasSocketed", true);
				m_Resources.m_Shaders["Light"].SetUniform("uParentSocketTransform", *instance.m_ParentSocketTransform);
			}
			else
			{
				m_Resources.m_Shaders["Light"].SetUniform("uHasSocketed", false);
			}

			for (auto& submesh : SubMeshes)
			{
				Model::Material& material = m_Resources.m_Materials[submesh.m_Material];

				// Send textures
				if (!material.m_Diffuse.empty())
				{
					const auto& texture = m_Resources.m_Textures.find(material.m_Diffuse);

					if (texture != m_Resources.m_Textures.end())
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedDiffuse", true);

						glBindTextureUnit(0, texture->second);
						m_Resources.m_Shaders["Light"].SetUniform("uMat.Diffuse", 0);
					}
					else
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedDiffuse", false);
					}
				}
				else
				{
					m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedDiffuse", false);
				}

				if (!material.m_Ambient.empty())
				{
					const auto& texture = m_Resources.m_Textures.find(material.m_Ambient);

					if (texture != m_Resources.m_Textures.end())
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedAmbient", true);

						glBindTextureUnit(1, texture->second);
						m_Resources.m_Shaders["Light"].SetUniform("uMat.Ambient", 1);
					}
					else
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedAmbient", false);
					}
				}
				else
				{
					m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedAmbient", false);
				}

				if (!material.m_Specular.empty())
				{
					const auto& texture = m_Resources.m_Textures.find(material.m_Specular);

					if (texture != m_Resources.m_Textures.end())
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedSpecular", true);

						glBindTextureUnit(2, texture->second);
						m_Resources.m_Shaders["Light"].SetUniform("uMat.Specular", 2);
					}
					else
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedSpecular", false);
					}
				}
				else
				{
					m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedSpecular", false);
				}

				if (!material.m_Normal.empty())
				{
					const auto& texture = m_Resources.m_Textures.find(material.m_Normal);

					if (texture != m_Resources.m_Textures.end())
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedNormal", true);

						glBindTextureUnit(3, texture->second);
						m_Resources.m_Shaders["Light"].SetUniform("uMat.Normal", 3);
					}
					else
					{
						m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedNormal", false);
					}
				}
				else
				{
					m_Resources.m_Shaders["Light"].SetUniform("uMat.TexturedNormal", false);
				}

				// Set model vbo handle to vao
				glVertexArrayVertexBuffer(m_VAO, 0, submesh.m_VBO, 0, sizeof(Model::Vertex));
				glVertexArrayElementBuffer(m_VAO, submesh.m_EBO);

				// Draw object
				glDrawElements(m_Resources.m_Models[std::string(model.first)].GetPrimitive(), submesh.m_DrawCount, GL_UNSIGNED_SHORT, NULL);
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_FinalBuffer.m_FrameBuffer[0]);

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

	// Blit depth buffer from light buffer to default framebuffer
	glBlitNamedFramebuffer(m_LightingBuffer.m_FrameBuffer[0], m_FinalBuffer.m_FrameBuffer[0], 0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void Renderer::MergePass()
{
	// Bind shader
	m_Resources.m_Shaders["Final"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Set screen model
	const auto& screen = m_Resources.m_Models["Screen"];
	glVertexArrayVertexBuffer(m_VAO, 0, screen.GetSubMeshes()[0].m_VBO, 0, sizeof(Model::Vertex));
	glVertexArrayElementBuffer(m_VAO, screen.GetSubMeshes()[0].m_EBO);

	glBindTextureUnit(0, m_UIBuffer.m_BufferTexture[0]);
	m_Resources.m_Shaders["Final"].SetUniform("uFinal", 0);

	glDrawElements(screen.GetPrimitive(), screen.GetSubMeshes()[0].m_DrawCount, GL_UNSIGNED_SHORT, NULL);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Final"].UnUse();
}

void Renderer::FinalPass()
{
	// Clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind shader
	m_Resources.m_Shaders["Final"].Use();

	// Bind vao
	glBindVertexArray(m_VAO);

	// Set screen model
	const auto& screen = m_Resources.m_Models["Screen"];
	glVertexArrayVertexBuffer(m_VAO, 0, screen.GetSubMeshes()[0].m_VBO, 0, sizeof(Model::Vertex));
	glVertexArrayElementBuffer(m_VAO, screen.GetSubMeshes()[0].m_EBO);

	glBindTextureUnit(0, m_FinalBuffer.m_BufferTexture[0]);
	m_Resources.m_Shaders["Final"].SetUniform("uFinal", 0);

	glDrawElements(screen.GetPrimitive(), screen.GetSubMeshes()[0].m_DrawCount, GL_UNSIGNED_SHORT, NULL);

	// Unbind vao
	glBindVertexArray(0);

	// Unbind shader
	m_Resources.m_Shaders["Final"].UnUse();
}

GLuint Renderer::GetUIOverlay()
{
	return m_UIBuffer.m_BufferTexture[0];
}

GLuint Renderer::GetFinalImage()
{
	return m_FinalBuffer.m_BufferTexture[0];
}

Renderer& Renderer::GetInstanced()
{
	static Renderer renderer;

	return renderer;
}