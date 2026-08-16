#include "ClientDefines.h"
#include "CClientApp.h"
#include "ACoreInstance.h"
#include "CClientAdiminstor.h"

CClientApp::CClientApp() 
{
}

_bool CClientApp::Start()
{
	GetCoreInstance()->ReadyCoreInstance(CreateServiceObject<CClientAdiminstor>());

	std::cout << "Start Client!\n";
	return GetCoreInstance()->Start();
}
