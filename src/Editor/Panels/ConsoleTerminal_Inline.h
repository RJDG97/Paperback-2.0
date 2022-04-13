/**********************************************************************************
*\file         ConsoleTerminal_Inline.h
*\brief        ConsoleTerminal_Inline.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Editor/Panels/ConsoleTerminal.h"

void ConsoleTerminal::Panel()
{
    if (showing_term)
    {
        showing_term = m_Imgui.m_Log.GetTerminal()->show();
    }
}