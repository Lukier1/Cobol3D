

#include "System.h"
#include "Logger.h"
#include "InputClass.h"
#include "SoundClass.h"

namespace Cobol {
	SystemClass::SystemClass() :  c_screenH(600), c_screenW(800), c_fullScreen(false){
		 mGraphic = NULL;
		 logger = new Logger("log.txt");
		 mSound = NULL;
		
	}

	SystemClass::~SystemClass() {
		if(! mGraphic)
		{
			delete  mGraphic;
		}
		if(logger)
			delete logger;
	}

	bool SystemClass::Init() {
		InitWindow();

		mGraphic = new GraphicClass();
		if(!mGraphic) {
			LOG_MSGERR("Nie mozna stworzyc GraphicClass");
			return false;
		}
		mGraphic->Init(mHwnd, c_screenW, c_screenH);

		mInput = new InputClass();
		if(!mInput) {
			LOG_MSGERR("Nie mozna stworzyc InputClass");
			return false;
		}
		if(!mInput->Init(mInstance, mHwnd,c_screenW,c_screenH))  {
			LOG_MSGERR("Nie mozna inicjiowac InputClass");
			return false;
		}
		mSound = new SoundClass();
		if(!mSound) {
			LOG_MSGERR("Nie mozna stworzyc SoundClass");
			return false;
		}
		if(!mSound->Init( mHwnd))  {
			LOG_MSGERR("Nie mozna inicjiowac SoundClass");
			return false;
		}
		return true;
	}

	void SystemClass::Shutdown() {
		if(mSound)
		{
			mSound->Shut();
			delete mSound;
			mSound = nullptr;
		}
		if(mGraphic)
		{
			mGraphic->Clean();
			delete mGraphic;
			mGraphic = nullptr;
		}
		if(mInput) {
			mInput->Shutdown();
			delete mInput;
			mInput = nullptr;
		}
		CloseWindow();
	}

	void SystemClass::Run() {
		MSG msg;
		bool done, result;
		done = false;
		while(!done)
		{
			if(PeekMessage(&msg, NULL,0,0,PM_REMOVE))	{
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if(msg.message == WM_QUIT)
				{
					done = true;
				}
			}
			else
			{
				result = Frame();
				if(!result)
				{
					done = true;
				}
			}
			Sleep(5);
		}
	}
	bool SystemClass::Frame() {
		InputData inp;
		mInput->GetMousePosition(inp.MouseX, inp.MouseY);
		inp.appTime = std::clock();
		inp.litera = mInput->getLetter();
		inp.IX = mInput->lX; inp.IY = mInput->lY;
		mGraphic->Frame(inp); // Cos z czasem zrobic
		if(!mInput->Frame())
			return false;
		if(mInput->IsKeyDown(DIK_ESCAPE))
			return false;


		return true;
	}
	void SystemClass::InitWindow() {
		WNDCLASSEX wndClass = { 0 };
		mInstance = GetModuleHandle(NULL);
		wndClass.cbSize = sizeof( WNDCLASSEX ) ;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = mInstance ;
		wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		wndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "CobolWindowClass";
		if( !RegisterClassEx( &wndClass ) )
			logger->WriteError("Nie mozna znarejstrowac okna");
		DEVMODE dmScreenSettings;
		if(c_fullScreen)
		{
			// If full screen set the screen to maximum size of the users desktop and 32bit.
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth  = (unsigned long)c_screenH;
			dmScreenSettings.dmPelsHeight = (unsigned long)c_screenW;
			dmScreenSettings.dmBitsPerPel = 32;			
			dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the display settings to full screen.
			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		}
		RECT rc = { 0, 0, c_screenW, c_screenH };
		AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
		mHwnd = CreateWindowA( "CobolWindowClass", "Puste Okno",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.
		left,
		rc.bottom - rc.top, NULL, NULL, mInstance, NULL );
		if( !mHwnd )
			logger->WriteError("Nie mozna utworzyc okna");
		ShowWindow( mHwnd, SW_SHOW );
		SetForegroundWindow(mHwnd);
		SetFocus(mHwnd);

	}
	void SystemClass::CloseWindow() {
		// Show the mouse cursor.
		ShowCursor(true);

		// Fix the display settings if leaving full screen mode.
		if(c_fullScreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		// Remove the window.
		DestroyWindow(mHwnd);
		mHwnd = NULL;

		// Remove the application instance.
		UnregisterClass("CobolWindowClass", mInstance);
		mInstance = NULL;

		// Release the pointer to this class.
		

		
	}
	LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
		switch(umsg)
	{
		
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			mInput->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			mInput->KeyUp((unsigned int)wparam);
			return 0;
		}
		
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch(umessage)
		{
			// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

			// Check if the window is being closed.
			case WM_CLOSE:
			{
				PostQuitMessage(0);		
				return 0;
			}

			// All other messages pass to the message handler in the system class.
			default:
			{
				return SystemClass::GetSingletonPtr()->MessageHandler(hwnd, umessage, wparam, lparam);
			}
		}
	}
};