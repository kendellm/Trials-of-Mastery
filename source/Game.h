/*
	File:		Game.h
	Author:		Doug Monroe (dmonroe@fullsail.com)
	Course:		SGD 1305
	Purpose:	CGame class handles the gameplay
				& initializes SGD wrappers
*/


#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "AnimationSystem/AnimInfo.h"

//Xbox 360 controller support
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

#include "Util/FontManager.h"
#include <list>
#include <string>
// Forward Class Declaration
//	- declares that the class name exists
//	- used only for pointers & references

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;
class IMessage;
class CEvent;
class TileManager;
class AnimationManager;
class StateManager;
class CEntityManager;
class CPopUpManager;
class CEntity;
class CController;
class GameInfo;
class IMessage;
class CSGD_MessageSystem;
class CAudioManager;

// SINGLETON:
//	- only ONE object is created
//	- prevents access to constructors
//	- object could be dynamically-allocated or global object
//	- class provides an access method for the object
class CGame
{
public:
	// Singleton accessors:
	static CGame* GetInstance( void );
	static void DeleteInstance( void );

	void Initialize( HWND hWnd, HINSTANCE hInstance,
					 int nWidth, int nHeight,
					 bool bIsWindowed );
	bool Main( void );
	void Terminate( void );

	// Accessors
	FontManager*		GetFont		   ( void ) const   { return m_pFont;}
	StateManager*		GetState       ( void ) const   { return m_pGameState;}
	AnimationManager*	GetAnimator    ( void ) const   { return m_pAM;}
	CEntityManager*		EntityManager  ( void ) const   { return m_pEM;}
	CPopUpManager*		PopUpManager   ( void ) const   { return m_pPUM;}
	int					ScreenWidth    ( void )	const	{ return m_nScreenWidth;}
	int					ScreenHeight   ( void )	const   { return m_nScreenHeight;}
	bool				Fullscreen     ( void ) const	{ return m_bIsWindowed;}

	void				Fullscreen     ( bool _screen);

	GameInfo*			CreateProfile  ( int stg=1,  int exp=1, int hp=1, int sp=1, int life=1, int fire=1, int wind=0,
		                                 int ice=0,	 int earth=0, int fsc=1, int lang=0, int bgm=49, int sfx=49 );

	GameInfo*			LoadProfile    ( int _slot);
	void				SaveProfile    ( int _slot);  //saves to working game profile
	void				SaveProfile    ( GameInfo* pInfo, int _slot );
	void				Create_Save_Load_Directory ();
	
	void				FirstRun	   (bool b)		{m_bFresh = b;}
	bool				FirstRun(void)	{return m_bFresh;}
	void				Tutorial (bool b )     {m_bTutorial = b;}
	bool				Tutorial ( void )	   {return m_bTutorial;}
	void				LockScreen( bool b)  {m_bLockScreen = b;}

	GameInfo*			GetProfile	   (void)          { return m_pProfile;}
	void				Import         (GameInfo* _info);
	bool				IsController    ( void );				

	HINSTANCE			HInstance	   () {return GameInstance;	}
	HWND				GethWnd		   () { return GamehWnd;	}
		static  void MessageProc( IMessage* pMsg );

private:
	// Static pointer to the singleton object
	static CGame* s_pInstance;

	//pointer to current game state
	StateManager* m_pGameState;

	// Private constructor / destructor to prevent multiple objects
	CGame	( void );						// default ctor
	~CGame	( void );						// dtor
	CGame	( const CGame& );				// copy ctor
	CGame& operator= ( const CGame& );	// assignment op


	// Helper Methods
	bool Input		( void );
	void Update		( void );
	void Render		( void );

	// SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	//Managers
	TileManager*			m_pTileM;
	AnimationManager*		m_pAM;
	CEntityManager*			m_pEM;
	FontManager*			m_pFont;
	CPopUpManager*			m_pPUM;
	CController*			m_pCtrl;
	CSGD_MessageSystem*		m_pMS;

	GameInfo*				m_pProfile;
	CAudioManager*			m_pAUM;
	// Window Attributes
	int						m_nScreenWidth;
	int						m_nScreenHeight;
	bool					m_bIsWindowed;
	bool					m_bLockScreen;

	XINPUT_STATE			m_XpadState;
	int						m_nContrNum;

	bool					m_bFresh;
	bool					m_bTutorial;
	std::string				m_szFilepath;
	HINSTANCE				GameInstance;
	HWND					GamehWnd;

	// Current time
	DWORD					m_dwCurrTime;

	// FPS
	int						m_nFPS;
	int						m_nFrames;
	float					m_fFPSTimer;
	bool					m_bShowFPS;

};

