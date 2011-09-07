

#include "stdafx.h"
#include "ULSLogView.h"
#include "tchar.h"
#include "comutil.h"
#include "winerror.h"
#include "SPLogParser.h"
#include <iostream>
#include <sstream>
#include <strstream>
#define MAX_LOADSTRING 100

using namespace std;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//char *listClass[]="SysListView32";
HANDLE fileChange;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI WaitForNewFile(LPVOID);
DWORD ThreadId;
WIN32_FIND_DATA ffd;
BSTR GetLogLocation();
HWND listWindow;
HANDLE OpenFileReading(TCHAR*,TCHAR []);
struct latestFile
{
	FILETIME latestWriteTime;
	TCHAR fileName[MAX_PATH];

};

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);
	// UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInst,IDS_APPTITLE,szTitle,100);
	LoadString(hInst,IDS_APPCLASS,szWindowClass,100);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_APPICON));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MAINMENU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPICON));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable
	//	TCHAR errMsg[100];


	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	BSTR logLocation;
	TCHAR errMsg[100];
	latestFile *logFile;
	TCHAR *Location;

	switch (message)
	{
	case WM_CREATE:
		listWindow=CreateWindowEx(NULL,L"SysListView32",NULL,WS_CHILD+WS_VISIBLE,0,0,0,0,hWnd,0,hInst,0);

		break;
	case WM_SIZE:
		MoveWindow(listWindow,0,0,100,100,true);

		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_LOADULSFROM_SHAREPOINT:
			logLocation=GetLogLocation();
			//char *p= _com_util::ConvertBSTRToString(logLocation);
			//strcpy(testDest,p );
			if(strcmp(_com_util::ConvertBSTRToString(logLocation),"-1")==0)
			{
				//str++;
				LoadString(hInst,IDS_SPNOTFOUND,errMsg,100);
				MessageBox(hWnd, errMsg,szTitle,MB_OK);
			}
			else
			{
				Location=_T("C:\\Program Files\\Common Files\\Microsoft Shared\\Web Server Extensions\\12\\LOGS");
				TCHAR Location1[100];
				fileChange=FindFirstChangeNotification(Location, false, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE);
				if (fileChange == INVALID_HANDLE_VALUE) 
				{
					//printf("\n ERROR: FindFirstChangeNotification function failed.\n");
					ExitProcess(GetLastError()); 
				}

						CreateThread(NULL,NULL, WaitForNewFile,NULL,NULL, &ThreadId);
				//_tcscat(Location,  _T("\\*"));
				_tcscpy(Location1,Location);
				_tcscat(Location1,  _T("\\*.log"));
				HANDLE fileHandle=FindFirstFile(Location1,&ffd);

				if(fileHandle==INVALID_HANDLE_VALUE)
				{
					//do cleanup
				}


				do
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
					}
					else
					{
						FILETIME fileTime=ffd.ftLastWriteTime;
						if(logFile!= NULL)
						{
							int writeTime=CompareFileTime(&fileTime,&logFile->latestWriteTime);
							if(writeTime==1)
							{
								_tcscpy(logFile->fileName, ffd.cFileName);
								logFile->latestWriteTime=ffd.ftLastWriteTime;
							}
						}
						else
						{
							logFile=new latestFile;
							_tcscpy(logFile->fileName, ffd.cFileName);
							logFile->latestWriteTime=ffd.ftLastWriteTime;


						}


					}
				}
				while (FindNextFile(fileHandle, &ffd) != 0);

			//We now have the filename with latest write time in the structure, use memory mapping to open the file
				SPLogParser *parser=new SPLogParser();
				//void* memFile=OpenFileReading(Location, logFile->fileName);
				//DWORD fileSize;
				//DWORD retFileSize=GetFileSize(fileHandle,&fileSize);
				//SPLogParser *parser=new SPLogParser();
				
				void *memFile=parser->OpenFileReading(Location,logFile->fileName);
				if(memFile!=NULL)
				{
				parser->ParseFile(memFile, fileHandle);
				}

			}
			break;
			/*case IDM_EXIT:
			DestroyWindow(hWnd);
			break;*/
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


DWORD WINAPI WaitForNewFile(LPVOID param)
{
	TCHAR errMsg[100];
	while(TRUE)
	{
		if(fileChange!=NULL)
		{
			DWORD waitState=WaitForSingleObject(fileChange, INFINITE);
			if(waitState==WAIT_OBJECT_0)
			{
				LoadString(hInst,IDS_SPNOTFOUND,errMsg,100);
				MessageBox(listWindow, errMsg,szTitle,MB_OK);
				FindNextChangeNotification(fileChange);
				//Reload File
			}
		}

	}
	return 0;
}


