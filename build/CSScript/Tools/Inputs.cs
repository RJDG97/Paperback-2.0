using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public class Input
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsKeyPress(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsKeyPressDown(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsKeyPressUp(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsGamepadButtonPress(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsGamepadButtonPressDown(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsGamepadButtonPressUp(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsMousePress(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsMouseDown(int Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsMouseUp(int Key);

        /* Printable keys */
        public const int PB_SPACE = 32;
        public const int PB_APOSTROPHE = 39;
        public const int PB_COMMA = 44;
        public const int PB_MINUS = 45;
        public const int PB_PERIOD = 46;
        public const int PB_SLASH = 47;
        public const int PB_0 = 48;
        public const int PB_1 = 49;
        public const int PB_2 = 50;
        public const int PB_3 = 51;
        public const int PB_4 = 52;
        public const int PB_5 = 53;
        public const int PB_6 = 54;
        public const int PB_7 = 55;
        public const int PB_8 = 56;
        public const int PB_9 = 57;
        public const int PB_SEMICOLON = 59;
        public const int PB_EQUAL = 61;
        public const int PB_A = 65;
        public const int PB_B = 66;
        public const int PB_C = 67;
        public const int PB_D = 68;
        public const int PB_E = 69;
        public const int PB_F = 70;
        public const int PB_G = 71;
        public const int PB_H = 72;
        public const int PB_I = 73;
        public const int PB_J = 74;
        public const int PB_K = 75;
        public const int PB_L = 76;
        public const int PB_M = 77;
        public const int PB_N = 78;
        public const int PB_O = 79;
        public const int PB_P = 80;
        public const int PB_Q = 81;
        public const int PB_R = 82;
        public const int PB_S = 83;
        public const int PB_T = 84;
        public const int PB_U = 85;
        public const int PB_V = 86;
        public const int PB_W = 87;
        public const int PB_X = 88;
        public const int PB_Y = 89;
        public const int PB_Z = 90;
        public const int PB_LEFT_BRACKET = 91;
        public const int PB_BACKSLASH = 92;
        public const int PB_RIGHT_BRACKET = 93;
        public const int PB_GRAVE_ACCENT = 96;
        public const int PB_WORLD_1 = 161;
        public const int PB_WORLD_2 = 162;

        /* Function keys */
        public const int PB_ESCAPE = 256;
        public const int PB_ENTER = 257;
        public const int PB_TAB = 258;
        public const int PB_BACKSPACE = 259;
        public const int PB_INSERT = 260;
        public const int PB_DELETE = 261;
        public const int PB_RIGHT = 262;
        public const int PB_LEFT = 263;
        public const int PB_DOWN = 264;
        public const int PB_UP = 265;
        public const int PB_PAGE_UP = 266;
        public const int PB_PAGE_DOWN = 267;
        public const int PB_HOME = 268;
        public const int PB_END = 269;
        public const int PB_CAPS_LOCK = 280;
        public const int PB_SCROLL_LOCK = 281;
        public const int PB_NUM_LOCK = 282;
        public const int PB_PRINT_SCREEN = 283;
        public const int PB_PAUSE = 284;
        public const int PB_F1 = 290;
        public const int PB_F2 = 291;
        public const int PB_F3 = 292;
        public const int PB_F4 = 293;
        public const int PB_F5 = 294;
        public const int PB_F6 = 295;
        public const int PB_F7 = 296;
        public const int PB_F8 = 297;
        public const int PB_F9 = 298;
        public const int PB_F10 = 299;
        public const int PB_F11 = 300;
        public const int PB_F12 = 301;
        public const int PB_F13 = 302;
        public const int PB_F14 = 303;
        public const int PB_F15 = 304;
        public const int PB_F16 = 305;
        public const int PB_F17 = 306;
        public const int PB_F18 = 307;
        public const int PB_F19 = 308;
        public const int PB_F20 = 309;
        public const int PB_F21 = 310;
        public const int PB_F22 = 311;
        public const int PB_F23 = 312;
        public const int PB_F24 = 313;
        public const int PB_F25 = 314;
        public const int PB_KP_0 = 320;
        public const int PB_KP_1 = 321;
        public const int PB_KP_2 = 322;
        public const int PB_KP_3 = 323;
        public const int PB_KP_4 = 324;
        public const int PB_KP_5 = 325;
        public const int PB_KP_6 = 326;
        public const int PB_KP_7 = 327;
        public const int PB_KP_8 = 328;
        public const int PB_KP_9 = 329;
        public const int PB_KP_DECIMAL = 330;
        public const int PB_KP_DIVIDE = 331;
        public const int PB_KP_MULTIPLY = 332;
        public const int PB_KP_SUBTRACT = 333;
        public const int PB_KP_ADD = 334;
        public const int PB_KP_ENTER = 335;
        public const int PB_KP_EQUAL = 336;
        public const int PB_LEFT_SHIFT = 340;
        public const int PB_LEFT_CONTROL = 341;
        public const int PB_LEFT_ALT = 342;
        public const int PB_LEFT_SUPER = 343;
        public const int PB_RIGHT_SHIFT = 344;
        public const int PB_RIGHT_CONTROL = 345;
        public const int PB_RIGHT_ALT = 346;
        public const int PB_RIGHT_SUPER = 347;
        public const int PB_MENU = 348;

        /* Mouse keys */
        public const int PB_MOUSE_BUTTON_1 = 0;
        public const int PB_MOUSE_BUTTON_2 = 1;
        public const int PB_MOUSE_BUTTON_3 = 2;
        public const int PB_MOUSE_BUTTON_4 = 3;
        public const int PB_MOUSE_BUTTON_5 = 4;
        public const int PB_MOUSE_BUTTON_6 = 5;
        public const int PB_MOUSE_BUTTON_7 = 6;
        public const int PB_MOUSE_BUTTON_8 = 7;

        /* Gamepad Keys */
        public const int PB_GAMEPAD_BUTTON_A            =  0;
        public const int PB_GAMEPAD_BUTTON_B            =  1;
        public const int PB_GAMEPAD_BUTTON_X            =  2;
        public const int PB_GAMEPAD_BUTTON_Y            =  3;
        public const int PB_GAMEPAD_BUTTON_LEFT_BUMPER  =  4;
        public const int PB_GAMEPAD_BUTTON_RIGHT_BUMPER =  5;
        public const int PB_GAMEPAD_BUTTON_BACK         =  6;
        public const int PB_GAMEPAD_BUTTON_START        =  7;
        public const int PB_GAMEPAD_BUTTON_GUIDE        =  8;
        public const int PB_GAMEPAD_BUTTON_LEFT_THUMB   =  9;
        public const int PB_GAMEPAD_BUTTON_RIGHT_THUMB  = 10;
        public const int PB_GAMEPAD_BUTTON_DPAD_UP      = 11;
        public const int PB_GAMEPAD_BUTTON_DPAD_RIGHT   = 12;
        public const int PB_GAMEPAD_BUTTON_DPAD_DOWN    = 13;
        public const int PB_GAMEPAD_BUTTON_DPAD_LEFT    = 14;
    }
}
