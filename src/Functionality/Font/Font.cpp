#include "../build/Paperback_V2/paperback_pch.h"
#include "Font.h"

Font::Font(const int ImageSize, const GLuint Texture, std::map<char, Font::Letter> Letters) : 
	m_ImageSize{ImageSize},
	m_Texture{Texture},
	m_Letters{Letters}
{

}

const int Font::GetImageSize() const
{
	return m_ImageSize;
}

const GLuint& Font::GetTexture() const
{
	return m_Texture;
}

Font::Letter& Font::GetLetter(char Letter)
{
	return m_Letters[Letter];
}