#include "stdafx.h"
#include "Engine.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	//_crtBreakAlloc = 39814;
	/*_crtBreakAlloc = -1;*/ 	//Use this to find memory leaks if present, set it equal to the leak number.
	Engine engine(hInstance);
	engine.run();
	
	
	
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return 0;
}


/*OutputDebugStringA((std::to_string() + "\n").c_str());*/