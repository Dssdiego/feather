//
// Created by Diego S. Seabra on 06/03/22.
//

#ifndef FEATHER_INPUT_H
#define FEATHER_INPUT_H

#include <vector>
#include <cstdint>
#include <GLFW/glfw3.h>

#include "../profiling/Logger.h"
#include "../rendering/Window.h"
#include "../rendering/Camera.h"

namespace Feather
{
	//
	// Keyboard
	//

	enum Keys
	{
		UNKNOWN = -1,
		SPACE = 32,
		APOSTROPHE = 39,
		COMMA = 44,
		MINUS = 45,
		PERIOD = 46,
		SLASH = 47,
		ALPHA_0 = 48,
		ALPHA_1 = 49,
		ALPHA_2 = 50,
		ALPHA_3 = 51,
		ALPHA_4 = 52,
		ALPHA_5 = 53,
		ALPHA_6 = 54,
		ALPHA_7 = 55,
		ALPHA_8 = 56,
		ALPHA_9 = 57,
		SEMICOLON = 59,
		EQUAL = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91,
		BACKSLASH = 92,
		RIGHT_BRACKET = 93,
		GRAVE_ACCENT = 96,
		WORLD_1 = 161,
		WORLD_2 = 162,
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DEL = 261, // delete
		ARROW_RIGHT = 262,
		ARROW_LEFT = 263,
		ARROW_DOWN = 264,
		ARROW_UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_DECIMAL = 330,
		KP_DIVIDE = 331,
		KP_MULTIPLY = 332,
		KP_SUBTRACT = 333,
		KP_ADD = 334,
		KP_ENTER = 335,
		KP_EQUAL = 336,
		LEFT_SHIFT = 340, // modifier
		LEFT_CONTROL = 341, // modifier
		LEFT_ALT = 342, // modifier
		LEFT_SUPER = 343, // modifier
		RIGHT_SHIFT = 344, // modifier
		RIGHT_CONTROL = 345, // modifier
		RIGHT_ALT = 346, // modifier
		RIGHT_SUPER = 347,
		MENU = 348
	};

	//
	// Mouse
	//

	enum MouseButton
	{
		BUTTON_1 = 0,
		BUTTON_2 = 1,
		BUTTON_3 = 2,
		BUTTON_4 = 3,
		BUTTON_5 = 4,
		BUTTON_6 = 5,
		BUTTON_7 = 6,
		BUTTON_8 = 7,
		LEFT = BUTTON_1,
		RIGHT = BUTTON_2,
		MIDDLE = BUTTON_3
	};

	// TODO: Implement
	enum MouseScroll
	{

	};

	//
	// Joystick/Gamepad
	//

	enum JoystickState
	{
		CONNECTED,
		DISCONNECTED
	};

	enum JoystickButton
	{
		B_A = 0,
		B_B = 1,
		B_X = 2,
		B_Y = 3,
		LEFT_BUMPER = 4,
		RIGHT_BUMPER = 5,
		BACK = 6,
		START = 7,
		GUIDE = 8,
		LEFT_THUMB = 9,
		RIGHT_THUMB = 10,
		DPAD_UP = 11,
		DPAD_RIGHT = 12,
		DPAD_DOWN = 13,
		DPAD_LEFT = 14,
		CROSS = A,
		CIRCLE = B,
		SQUARE = X,
		TRIANGLE = Y
	};

	enum JoystickAxis
	{
		LEFT_X = 0,
		LEFT_Y = 1,
		RIGHT_X = 2,
		RIGHT_Y = 3,
		LEFT_TRIGGER = 4,
		RIGHT_TRIGGER = 5
	};

	// NOTE: Unused for now
	struct Joystick
	{
		uint32_t id;
	};

	//
	// States
	//

	// REVIEW: Buttons and mouse input act "the same way": the can be pressed, released or be hold
	//         Joysticks also have buttons but they have axis and triggers (xbox). Should their information be described here?
	enum InputState
	{
		PRESSED,
		RELEASED,
		HOLD
	};

	class Input
	{
	public:
		static void Init();
		static void Update();
		static void Shutdown();

		static bool GetKeyboardKeyDown(Keys key);
		static bool GetKeyboardKeyUp(Keys key);
		static bool GetKeyboardKeyHold(Keys key);

		static bool IsJoystickButtonPressed(JoystickButton joyButton);

		static bool IsMouseButtonPressed(MouseButton mouseButton);
		static bool IsMouseButtonReleased(MouseButton mouseButton);

		static glm::vec2 GetMouseScreenPosition(); // in screen coordinates
		static glm::vec2 GetMouseWorldPosition(); // in world coordinates

		// TODO: Implement
	//    static glm::vec2 GetMouseScreenPosition();

	//    inline static std::vector<Joystick> joysticks = {};
		// REVIEW: GLFW supports the maximum of 16 joysticks (perhaps we define this in a enum?)
		inline static std::vector<uint32_t> joysticks = {};

		inline static int keyboardKeys[512]; // FIXME: Get this number dynamically
		inline static bool mouseButtons[8]; // FIXME: Get this number dynamically

		inline static glm::vec2 mousePosition = glm::vec2(0, 0);

	private:
		static void SetupGLFWCallbacks();

		inline static bool joystickButtons[15]; // FIXME: Get this number dynamically

		//    std::vector<uint32_t> keys = {};
	};
}

#endif //FEATHER_INPUT_H
