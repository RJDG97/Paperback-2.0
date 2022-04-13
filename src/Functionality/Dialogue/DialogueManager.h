/**********************************************************************************
*\file         DialogueManager.h
*\brief        DialogueManager.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <vector>
#include <unordered_map>

struct Line
{
	enum Speaker
	{
		RED,
		BLUE
	};

	Speaker m_Speaker;
	std::string m_AudioFile;
	std::string m_Content;
	std::string m_CameraName;
};

struct Dialogue
{
	std::vector<Line> m_Lines;
};

class DialogueManager
{
	DialogueManager();
	void LoadDialogueJson(std::string file, std::string dialogue_name);

public :

	static DialogueManager& GetInstanced();
	DialogueManager(const DialogueManager&) = delete;
	DialogueManager& operator=(const DialogueManager&) = delete;

	// Map of dialogue
	std::unordered_map<std::string, Dialogue> m_Dialogues;
};

#endif