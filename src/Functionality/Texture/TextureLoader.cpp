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
#define TINYDDSLOADER_IMPLEMENTATION
#include "tinydds/inc/tinyddsloader.h"

GLuint TextureLoader::LoadTexture(const std::string File, const bool& GammaCorrect)
{
	int width, height, channels;
	GLfloat anisotropy;

	// Load image data
	unsigned char* data = SOIL_load_image(File.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		GLenum format = GL_RGB, internalformat = GL_RGB;

		switch (channels)
		{
		case 1:
		{
			format = GL_RED;
			internalformat = GL_R8;
		}
		break;
		case 3:
		{
			format = GL_RGB;
			internalformat = GammaCorrect ? GL_SRGB8 : GL_RGB8;
		}
		break;
		case 4:
		{
			format = GL_RGBA;
			internalformat = GammaCorrect ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		}
		break;
		}

		// Generate texture
		GLuint texture;
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, internalformat, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

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

GLuint TextureLoader::LoadSkyboxTexture(const std::vector<std::string>& Files)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height, channels;

	for (int face = 0; face < Files.size(); ++face)
	{
		// Load image data
		unsigned char* data = SOIL_load_image(Files[face].c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			SOIL_free_image_data(data);
		}
		else
		{
			return 0;
		}
	}

	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}

GLuint TextureLoader::LoadDDSTexture(const std::string& File, const bool& GammaCorrect)
{
	tinyddsloader::DDSFile file;
	file.Load(File.c_str());

	GLenum internalformat = GL_RGB;

	if (file.GetFormat() == tinyddsloader::DDSFile::DXGIFormat::BC1_UNorm && file.GetTextureDimension() == tinyddsloader::DDSFile::TextureDimension::Texture2D)
	{
		internalformat = GammaCorrect ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT : GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

		// Generate texture
		GLuint texture;
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		glTextureParameteri(texture, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, file.GetMipCount() - 1);

		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLfloat anisotropy;

		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
		glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureStorage2D(texture, file.GetMipCount(), internalformat, file.GetWidth(), file.GetHeight());

		//file.Flip();

		for (size_t level = 0; level < file.GetMipCount(); ++level)
		{
			auto data = file.GetImageData(level, 0);

			auto width = data->m_width;
			auto height = data->m_height;

			glCompressedTextureSubImage2D(texture, level, 0, 0, width, height, internalformat, data->m_memSlicePitch, data->m_mem);
		}

		return texture;
	}

	return 0;
}

GLuint TextureLoader::LoadDDSSkyboxTexture(const std::vector<std::string>& Files)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	bool first = true;

	tinyddsloader::DDSFile file;

	for (size_t face = 0; face < Files.size(); ++face)
	{
		file.Load(Files[face].c_str());

		if (first)
		{
			glTextureParameteri(texture, GL_TEXTURE_BASE_LEVEL, 0);
			glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, file.GetMipCount() - 1);

			glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTexStorage2D(GL_TEXTURE_CUBE_MAP, file.GetMipCount(), GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, file.GetWidth(), file.GetHeight());

			first = false;
		}

		for (size_t level = 0; level < file.GetMipCount(); ++level)
		{
			auto data = file.GetImageData(level, 0);

			auto width = data->m_width;
			auto height = data->m_height;

			glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, 0, 0, width, height, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, data->m_memSlicePitch, data->m_mem);
		}
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture;
}

void TextureLoader::FreeTexture(unsigned char* Pixels)
{
	SOIL_free_image_data(Pixels);
}