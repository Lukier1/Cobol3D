#ifndef __SYSTEM_H_
#define __SYSTEM_H__

#include <Windows.h>
#include "GraphicClass.h"
#include "Singleton.h"

namespace Cobol {
	class Logger;
	class InputClass;
	class SoundClass;
	class SystemClass : public Singleton<SystemClass>{
	private:
		int c_screenH, c_screenW;
		bool c_fullScreen;
		bool Frame();
		void InitWindow();
		void CloseWindow();

		LPCWSTR mApplicationName;
		HINSTANCE mInstance;
		HWND mHwnd;
		GraphicClass * mGraphic; 
		SystemClass(const SystemClass&) { }

		Logger * logger;
		InputClass * mInput;
		SoundClass * mSound;
	public:
		SystemClass();
		~SystemClass();

		bool Init();
		void Shutdown();
		void Run();

		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	};

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
}
#endif 