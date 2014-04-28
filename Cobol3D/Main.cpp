#include "Logger.h"
//#include "Root.h"
#include "System.h"


int WINAPI wWinMain(HINSTANCE hInstance,  HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) 
{
	
	Cobol::SystemClass system;
	if(system.Init())
	{
		system.Run();
	}
	else
		MessageBox(NULL,"Error", "!!!!", MB_OK);
	system.Shutdown();
	
	
//	Cobol::Root lRoot;
//	if(!lRoot.NewWindow(hInstance, cmdShow))
//		MessageBox(NULL,"Error", "!!!!", MB_OK);
//	lRoot.Run();
	return 0;
}