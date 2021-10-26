#pragma once
#include "Editor/Panels/ConsoleTerminal.h"

void ConsoleTerminal::Panel()
{
    if (showing_term)
    {
        showing_term = m_Imgui.m_Log.GetTerminal()->show();
    }
}