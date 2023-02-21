#include <crtdbg.h>
#include "Client.h"

int main(int argc, char** argv)
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(185080);

	srand((unsigned int)time(NULL));

	Client* pClient = new Client();
	RESULT result = pClient->Ready();
	if (result != PK_NOERROR) return result;

	pClient->Loop();
	pClient->Destroy();

	return 0;
}