#include "InputClass.h"
#include <Windows.h>
namespace Cobol {
	short mLetterTab[] =
	{
		DIK_A,
		DIK_B,
		DIK_C,
		DIK_D,
		DIK_E,
		DIK_F,
		DIK_G,
		DIK_H,
		DIK_I,
		DIK_J,
		DIK_K,
		DIK_L,
		DIK_M,
		DIK_N,
		DIK_O,
		DIK_P,
		DIK_Q,
		DIK_R,
		DIK_S,
		DIK_T,
		DIK_U,
		DIK_V,
		DIK_W,
		DIK_X,
		DIK_Y,
		DIK_Z
	};

	InputClass::InputClass()
	{
		mDirectInput=0;
		mKeyboard=0;
		mMouse=0;
		mLastHit = 0;
		mLastKey = 0;
		mMouseX = 0;
		mMouseY = 0;
	}

	InputClass::~InputClass()
	{
	}

	bool InputClass::Init(HINSTANCE hinstace, HWND hwnd, int scrW, int scrH)
	{
		HRESULT result;

		mScreenW = scrW;
		mScreenH = scrH;

		int i;
	
		// Initialize all the keys to being released and not pressed.
		for(i=0; i<KEYSBUFFORSIZE; i++)
		{
			m_keys[i] = false;
		}
		

		result = DirectInput8Create(hinstace,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&mDirectInput, NULL);
		if(FAILED(result))
			return false;

		result = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard,NULL);
		if(FAILED(result))
			return false;

		result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(result))
			return false;

		result = mKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
		if(FAILED(result))
			return false;

		result = mKeyboard->Acquire();
		if(FAILED(result))
			return false;

		result = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse,NULL);
		if(FAILED(result))
			return false;

		result = mMouse->SetDataFormat(&c_dfDIMouse);
		if(FAILED(result))
			return false;

		result = mMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if(FAILED(result))
			return false;
	
		result = mMouse->Acquire();
		if(FAILED(result))
			return false;
	
		return true;
	}

	void InputClass::Shutdown() {
		if(mMouse)
		{
			mMouse->Unacquire();
			mMouse->Release();
			mMouse = NULL;
		}
		if(mKeyboard)
		{
			mKeyboard->Unacquire();
			mKeyboard->Release();
			mKeyboard = NULL;
		}
		if(mDirectInput) {
			mDirectInput->Release();
			mDirectInput = NULL;
		}
	}
	char InputClass::getLetter() {
		if(mLastHit+200 < GetTickCount() || !(0x80 & mKeyboardState[mLastKey]))
		{
			if(mLastKey != 0 && (0x80 & mKeyboardState[mLastKey]))
			{
				mLastHit = GetTickCount();
				if(mLastKey == DIK_SPACE)
					return ' ';
				if(mLastKey == DIK_BACKSPACE)
					return 0x7F;
			for(int i = 0; i < sizeof(mLetterTab)/sizeof(short); ++i) 
					if(mLetterTab[i] == mLastKey)
						{
							int duzLit = 
								(0x80 & mKeyboardState[DIK_LSHIFT] || 0x80 & mKeyboardState[DIK_RSHIFT]) ? 0 : 32;
							mLastHit = GetTickCount();
							return (i+65) + duzLit;
						}

			
			}
			else
			{
				mLastKey = 0;
				for(int i = 0; i < sizeof(mLetterTab)/sizeof(short); ++i) 
					if(0x80 & mKeyboardState[mLetterTab[i]])
						{
							int duzLit = 
								(0x80 & mKeyboardState[DIK_LSHIFT] || 0x80 & mKeyboardState[DIK_RSHIFT]) ? 0 : 32;
							mLastHit = GetTickCount();
							mLastKey = mLetterTab[i];
						
							return (i+65) + duzLit;
						}
				if(0x80 & mKeyboardState[DIK_SPACE])
				{
					mLastKey = DIK_SPACE;
					mLastHit = GetTickCount();
					return ' ';
				}
				if(0x80 & mKeyboardState[DIK_BACKSPACE])
				{
					mLastKey = DIK_BACKSPACE;
					mLastHit = GetTickCount();
					return 0x7F;
				}
			}
		}
		return NULL;
	}

	bool InputClass::Frame() {
		bool result;
		result = ReadKeyboard();
		if(!result)
		{
			return false;
		}
		result = ReadMouse();
		if(!result)
			return false;
		ProcessInput();
		return true;
		
	}

	bool InputClass::ReadKeyboard() {
		HRESULT result;

		result = mKeyboard->GetDeviceState(sizeof(mKeyboardState),(LPVOID)&mKeyboardState);
		if(FAILED(result))
		{
			if((result==DIERR_INPUTLOST) || (result==DIERR_NOTACQUIRED))
			{
				mKeyboard->Acquire();
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	bool InputClass::ReadMouse() {
		HRESULT result = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);
		if(FAILED(result))
		{
			if((result==DIERR_INPUTLOST) || (result==DIERR_NOTACQUIRED))
			{
				mMouse->Acquire();
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	void InputClass::ProcessInput() {
		mMouseX += mMouseState.lX; 
		mMouseY += mMouseState.lY;
		
		//if(mMouseX<0)
			//mMouseX = 0;
		////if(mMouseY<0)
		//	mMouseY = 0;
		//if(mMouseX>mScreenW) 
		//	mMouseX = mScreenW;
		//if(mMouseY>mScreenH) 
			//mMouseY = mScreenH;
	}
	void InputClass::KeyDown(unsigned int input)
	{
		// If a key is pressed then save that state in the key array.
		if(input < KEYSBUFFORSIZE)
			m_keys[input] = true;
		return;
	}


	void InputClass::KeyUp(unsigned int input)
	{
		// If a key is released then clear that state in the key array.
		if(input < KEYSBUFFORSIZE)
			m_keys[input] = false;
		return;
	}

	void InputClass::GetMousePosition(int& X, int& Y) { 
		
		X= mMouseX;
		Y= mMouseY; 
	}
	bool InputClass::IsKeyDown(unsigned int key)
	{
		if(mKeyboardState[key] & 0x80)
		{
			return true;
		}
		return false;
		/*// Return what state the key is in (pressed/not pressed).
		if(key < KEYSBUFFORSIZE)
			return m_keys[key];
		else
			return false;*/
	}

}