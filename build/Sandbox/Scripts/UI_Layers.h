#pragma once

enum UILayer
{
	BASE = 0,
	PLAYUI,			//for buttons that are toggable only during gameplay
	PAUSE,			//for buttons that are toggable only during pause
	RESTART,		//for confirmation buttons toggable only during restart confirm
	RETURNMAINMENU,	//for confirmation buttons toggable only during return confirm
	QUIT			//for confirmation buttons toggable only during quit confirm
};