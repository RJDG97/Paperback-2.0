/**********************************************************************************
* \file TextureLoader.cpp
* \brief This is the texture loader class which provides helper functions to load texture
* images for use.
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#include "../build/Paperback_V2/paperback_pch.h"
#include "TextureLoader.h"
#include "soil/inc/SOIL2.h"

GLuint TextureLoader::LoadTexture(const std::string File)
{
	int width, height, channels;
	GLfloat anisotropy;

	// Load image data
	unsigned char* data = SOIL_load_image(File.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		// Generate texture
		GLuint texture;
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
		glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenerateTextureMipmap(texture);

		SOIL_free_image_data(data);

		return texture;
	}
	else
	{
		return 0;
	}
}

void TextureLoader::FreeTexture(unsigned char* Pixels)
{
	SOIL_free_image_data(Pixels);
}