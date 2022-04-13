/**********************************************************************************
*\file         Scripts_Debug.h
*\brief        Scripts_Debug.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_DEBUG
{
	MONO_EXPORT void Log(MonoString* string)
	{
		ERROR_PRINT(mono_string_to_utf8(string));
	}

	//MONO_EXPORT void DrawLine()
	//{
	//}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Debug::Log(string)", &MONO_DEBUG::Log);
	}
}