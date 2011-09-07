#import "..\SPULS\bin\Debug\SPULS.tlb" raw_interfaces_only

using namespace SPULS;


#include "stdafx.h"
#include "tchar.h"

BSTR GetLogLocation()
{
    // Initialize COM.
    HRESULT hr = CoInitialize(NULL);

    // Create the interface pointer.
    IUlsInfoPtr   pICalc(__uuidof(SPUlsInfo));

  
	BSTR logLocation=SysAllocStringLen(NULL, MAX_PATH);
    // Call the Add method.
    hr=pICalc->GetLogLocation(&logLocation);

   // wprintf(L"The result is %s\n", logLocation);


    // Uninitialize COM.
    CoUninitialize();
    return logLocation;
}

/*
char* GetLogLocation()
{
	CreateProcessEx(


}*/