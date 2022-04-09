#pragma once

enum UI_LAYER
{
	BASE = 0,
	PLAYUI,				//for buttons that are toggable only during gameplay
	PAUSE,				//for buttons that are toggable only during pause
	RESTART,			//for confirmation buttons toggable only during restart confirm
	RETURNMAINMENU,		//for confirmation buttons toggable only during return confirm
	QUIT,				//for confirmation buttons toggable only during quit confirm
	HOWTOPLAY,			//for close button that disables all how to play screens
	HOWTOPLAY1,			//for page 1 of how to play
	HOWTOPLAY2,			//for page 2 of how to play
	HOWTOPLAY3,			//for page 3 of how to play
	HOWTOPLAY4,			//for page 4 of how to play
	HOWTOPLAY5,			//for page 5 of how to play
	HOWTOPLAY6,			//for page 6 of how to play
	HOWTOPLAY7,			//for page 7 of how to play
	RESTART_CHECKPOINT, //for buttons toggable only during restart from last checkpoint confirm
	SETTINGS,
	GAMMA,
	FULLSCREEN,
	VOLUME1,
	VOLUME2,
	VOLUME3,
	VOLUME4,
	VOLUME5,
	MOUSESENS1,
	MOUSESENS2,
	MOUSESENS3,
	MOUSESENS4,
	MOUSESENS5,

};