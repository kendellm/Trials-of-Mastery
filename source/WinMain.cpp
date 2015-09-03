//////////////////////////////////////////////////////////////////////////////////////////////
//	File			:	"WinMain.cpp"
//
//	Author			:	Doug Monroe, David Brown, & Jensen Rivera
//	Based in part on:
//		-Window code from the book: "Physics for Game Developers" by David M. Bourg.
//
//	Last Modified	:	10/23/2012
//
//	Purpose			:	To provide a basic window framework for student games.
//
//////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>	//	Needed for Windows Applications.
#include <tchar.h>
#include <vld.h>		//	Needed for Visual Leak Detector
#include "Game.h"
#include "../resource.h"
//#include "afxwin.h"

const TCHAR*	g_szWINDOW_CLASS_NAME	= _T("Trials of Mastery");			//	Window Class Name.
const TCHAR*	g_szWINDOW_TITLE		= _T("Trials of Mastery");			//	Window Title.
const int		g_nWINDOW_WIDTH			= 800;							//	Window Width.
const int		g_nWINDOW_HEIGHT		= 600;							//	Window Height.
const BOOL		g_bIS_WINDOWED			= TRUE;							//	Window Status: Windowed / Fullscreen
#define IDT_TIMER  1

//	Function declarations
BOOL IsAlreadyRunning( void );
BOOL RegisterWindowClass( HINSTANCE hInstance );
HWND MakeWindow( HINSTANCE hInstance );
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
//INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//
//	
//
//	switch(uMsg)
//	{
//	case WM_INITDIALOG:
//		SetTimer(hDlg, IDT_TIMER ,2000, (TIMERPROC) NULL);
//	
//
//		//CBitmap bmp;
//		//bmp.LoadBitmap(IDB_PNG1);
//		//.SetBitmap(bmp);
//
//
//		return TRUE;
//	case WM_TIMER:
//		
//		switch (wParam)
//		{
//		case IDT_TIMER:
//			{
//				KillTimer(hDlg, IDT_TIMER);
//				PostQuitMessage(0);
//			}
//			break;
//		default:
//			break;
//		}
//		return TRUE;
//	case WM_CLOSE: 
//		return TRUE; 
//
//	case WM_DESTROY:
//		return TRUE;
//	}
//	return FALSE;
//}

//////////////////////////
//		WinMain			//
//////////////////////////




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;	//	Generic message.
	HWND	hWnd;	//	Main Window Handle.



	//////////////////////////////////////////
	// Don't let more than one instance of the application exist
	//
	// NOTE:	Comment out the following section of code if your game needs to have more
	//			than one instance running on the same computer (i.e. client/server)
	if( hPrevInstance == 0 )
	{
		if( IsAlreadyRunning() == TRUE )
			return -1;
	}
	//////////////////////////////////////////
	/*HWND hDlg;
	MSG hmsg;
	BOOL ret;*/

	//static HBRUSH hBrushStatic;
	//HBITMAP hBmp = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PNG1));

	//hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
	//ShowWindow(hDlg, nCmdShow);


	//while((ret = GetMessage(&hmsg, 0, 0, 0)) != 0) {
	//	if(ret == -1)
	//		return -1;

	//	if(!IsDialogMessage(hDlg, &hmsg)) {
	//		TranslateMessage(&hmsg);
	//		DispatchMessage(&hmsg);
	//	}
	//}

	//	Register the window class
	if( RegisterWindowClass( hInstance ) == FALSE )
	{
		MessageBox( HWND_DESKTOP, _T("ERROR: Failed to register the window class."),
			g_szWINDOW_TITLE, MB_OK | MB_ICONERROR );

		return -2;
	}


	//	Create the window
	hWnd = MakeWindow(hInstance);

	if( hWnd == 0 )
	{
		MessageBox( HWND_DESKTOP, _T("ERROR: Failed to create the Main Window."),
			g_szWINDOW_TITLE, MB_OK | MB_ICONERROR );
		
		//	Unregister the window class
		UnregisterClass( g_szWINDOW_CLASS_NAME, hInstance );

		return -3;
	}


	// Display the window
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );


	//////////////////////////////////////////
	//	Initialize Game here
	//////////////////////////////////////////
	
	// Access the CGame singleton
	CGame* pGame = CGame::GetInstance();

	// Initialize the game
	pGame->Initialize(  hWnd, hInstance, 
						g_nWINDOW_WIDTH, g_nWINDOW_HEIGHT,
						g_bIS_WINDOWED );
					


	//////////////////////////////////////////


	//	Enter main event loop
	while( true )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) == TRUE )
		{ 
			//	Test if this is a quit
			if( msg.message == WM_QUIT )
				break;
		
			//	Translate any accelerator keys
			TranslateMessage( &msg );

			//	Send the message to the window proc
			DispatchMessage( &msg );
		}
		else
		{
			//////////////////////////////////
			//	Run game logic here
			//////////////////////////////////

			if( pGame->Main() == false )
				PostQuitMessage( 0 );

		
			//////////////////////////////////
		}
	}

	/////////////////////////////////////////
	//	Shut down game here
	/////////////////////////////////////////
	
	// Terminate the game
	pGame->Terminate();
	CGame::DeleteInstance();
	pGame = nullptr;


	/////////////////////////////////////////
	
	
	//	Unregister the window class
	UnregisterClass( g_szWINDOW_CLASS_NAME, hInstance );
	
	//	Return message's Quit code to the OS
	return (int)(msg.wParam);
}


/////////////////////////////////////////
//	
//	Function:	IsAlreadyRunning
//	
//	Purpose:	Checks to see if the game was already running in another window.
//
//	NOTE:		Don't call this function if your game needs to have more
//				than one instance running on the same computer (i.e. client/server)
/////////////////////////////////////////
BOOL IsAlreadyRunning(void)
{
	//	Find a window of the same window class name and window title
	HWND hOtherWnd = FindWindow( g_szWINDOW_CLASS_NAME, g_szWINDOW_TITLE );

	//	If one was found
	if( hOtherWnd )
	{
		//	If it was minimized
		if( IsIconic( hOtherWnd ) == TRUE )
			//	restore it
			ShowWindow( hOtherWnd, SW_RESTORE );

		//	Bring it to the front
		SetForegroundWindow( hOtherWnd );

		return TRUE;
	}

	//	No other copies found running
	return FALSE;
}

/////////////////////////////////////////
//
//	Function:	RegisterWindowClass
//	
//	Purpose:	Register the window class for a call to CreateWindow
//
/////////////////////////////////////////
BOOL RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX	winClassEx;	//	This will describe the window class we will create.

	//	First fill in the window class structure
	winClassEx.cbSize			= sizeof(winClassEx);
	winClassEx.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClassEx.lpfnWndProc		= WindowProc;
	winClassEx.cbClsExtra		= 0;
	winClassEx.cbWndExtra		= 0;
	winClassEx.hInstance		= hInstance;
	winClassEx.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winClassEx.hIconSm			= NULL;
	winClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	winClassEx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClassEx.lpszMenuName		= NULL; 
	winClassEx.lpszClassName	= g_szWINDOW_CLASS_NAME;

	//	Register the window class
	return RegisterClassEx( &winClassEx );
}

/////////////////////////////////////////
//
//	Function:	MakeWindow
//	
//	Purpose:	Creates and centers the window.
//
/////////////////////////////////////////
HWND MakeWindow(HINSTANCE hInstance)
{
	// Setup window style flags,                                  // disabled min, max and close buttons
	DWORD dwWindowStyleFlags = WS_VISIBLE | WS_POPUP | WS_CAPTION /*| WS_SYSMENU | WS_MINIMIZEBOX*/;

	// Setup the desired client area size
	RECT rWindow;
	rWindow.left	= 0;
	rWindow.top		= 0;
	rWindow.right	= g_nWINDOW_WIDTH;
	rWindow.bottom	= g_nWINDOW_HEIGHT;

	// Get the dimensions of a window that will have a client rect that
	// will really be the resolution we're looking for.
	AdjustWindowRectEx( &rWindow, 
						dwWindowStyleFlags,
						FALSE, 
						WS_EX_APPWINDOW );
	
	// Calculate the width/height of the window's dimensions
	int nWindowWidth	= rWindow.right - rWindow.left;
	int nWindowHeight	= rWindow.bottom - rWindow.top;

	//	Create the window
	return CreateWindowEx(WS_EX_APPWINDOW,											//	Extended Style flags.
						  g_szWINDOW_CLASS_NAME,									//	Window Class Name.
						  g_szWINDOW_TITLE,											//	Title of the Window.
						  dwWindowStyleFlags,										//	Window Style Flags.
						  (GetSystemMetrics(SM_CXSCREEN) - nWindowWidth)/2,			//	Window Start Point (x, y). 
						  (GetSystemMetrics(SM_CYSCREEN) - nWindowHeight)/2,		//		Center the window over the desktop.
						  nWindowWidth,												//	Width of Window.
						  nWindowHeight,											//	Height of Window.
						  HWND_DESKTOP,												//	Handle to parent window.
						  NULL,														//	Handle to menu.
						  hInstance,												//	Application Instance.
						  NULL);													//	Creation parameters.
}

/////////////////////////////////////////
//
//	Function:	WindowProc
//	
//	Purpose:	Window message handler for the application
//
/////////////////////////////////////////
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	//	What is the message?
	switch( msg )
	{
	//	Window constructor
	case WM_CREATE: 
		{
			//	Window initialization here

		}
		break;
	

	//	Window destructor
	case WM_DESTROY: 
		{
			//	Kill the application			
			PostQuitMessage( 0 );
		}
		break;


	//	Ignore ALT pop up menu (system menu)
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
		{
		}
		break;


	//	Handle ALT+F4
	case WM_CLOSE:
		{
			if(MessageBox(hWnd,
				TEXT("Exit Game?"), TEXT("Close"),
				MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			else
			{
				break;
			}
			// Send a WM_DESTROY
			//DestroyWindow(hWnd);			
		}
		break;


	//	Gain / lose focus
	case WM_ACTIVATE:
		{
			if( LOWORD( wParam ) != WA_INACTIVE )	//	gaining focus
			{
				// Unpause game here
			}
			else									//	losing focus
			{
				// Pause game here
			}
		}
		break;

	
	//	Ignore repaint
	case WM_PAINT:
		{
			PAINTSTRUCT	ps;
			HDC			hdc;

			//	Start painting
			hdc = BeginPaint( hWnd,&ps );

			//	End painting
			EndPaint( hWnd,&ps );
		}
		break;

	
	default:
		//	Process any messages that we didn't take care of 
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	//	We have handled this message.
	return 0;
}
