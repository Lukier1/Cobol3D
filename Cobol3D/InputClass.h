#ifndef __INPUTCLASS_H__
#define __INPUTCLASS_H__

#define DIRECTINPUT_VERSION 0x0800 

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class SystemClass;
#include "Singleton.h"
#define KEYSBUFFORSIZE 256
namespace Cobol {
	class InputClass : public Singleton<InputClass> {
	public:
		bool Init(HINSTANCE hinstance, HWND hwnd, int scrW, int scrH);
		void Shutdown();
		bool IsKeyDown(unsigned int key);

		void GetMousePosition(int& X, int& Y);
		void GetMove(int& X, int& Y);
		bool Frame();
		char getLetter();
	private:
		InputClass();
		~InputClass();
		int a;
		bool ReadKeyboard();
		bool ReadMouse();
		void ProcessInput();
		
		
		void KeyDown(unsigned int key);
		void KeyUp(unsigned int key);

		bool m_keys[KEYSBUFFORSIZE];
		friend SystemClass;

		IDirectInput8 * mDirectInput;
		IDirectInputDevice8 * mKeyboard;
		IDirectInputDevice8 * mMouse;

		unsigned char mKeyboardState[KEYSBUFFORSIZE];
		DIMOUSESTATE mMouseState;

		int mScreenW, mScreenH;
		int mMouseX, mMouseY;
		
		long mLastHit;
		short mLastKey;
	};
}
#endif