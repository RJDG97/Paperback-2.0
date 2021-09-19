/**********************************************************************************
* \file TextureLoader.h
* \brief This is the texture loader class which provides helper functions to load texture
* images for use.
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "glew/inc/glew.h"

class TextureLoader
{
public:
	TextureLoader() = default;
	TextureLoader(const TextureLoader&) = delete;
	~TextureLoader() = default;

	TextureLoader& operator=(const TextureLoader&) = delete;

	static GLuint LoadTexture(const std::string File);
	static void FreeTexture(unsigned char* Pixels);
};

#endif