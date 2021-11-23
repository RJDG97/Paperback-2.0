#ifndef FONT_H
#define FONT_H

#include <map>

#include "glm/inc/glm.hpp"
#include "glew/inc/glew.h"

class Font
{
public:
	struct Letter
	{
		glm::vec2 m_LetterPosition;
		glm::vec2 m_LetterSize;
		glm::vec2 m_Offset;
		int m_Advance;
	};

	Font() = default;
	Font(const int ImageSize, const GLuint Texture, std::map<char, Font::Letter> Letters);

	const int GetImageSize() const;
	const GLuint& GetTexture() const;
	Letter& GetLetter(char Letter);

private:
	int m_ImageSize;
	GLuint m_Texture;
	std::map<char, Letter> m_Letters;
};

#endif