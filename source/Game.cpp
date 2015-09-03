/*
	File:		Game.cpp
	Author:		Doug Monroe (dmonroe@fullsail.com)
	Course:		SGD 1305
	Purpose:	CGame class handles the gameplay
				& initializes SGD wrappers
*/


#include "Game.h"
#include "GameInfo.h"


#include "CSGD/CSGD_Direct3D.h"
#include "CSGD/CSGD_DirectInput.h"
#include "CSGD/CSGD_TextureManager.h"
#include "CSGD/CSGD_XAudio2.h"
#include "CSGD/CSGD_EventSystem.h"
#include "CSGD/CSGD_MessageSystem.h"
#include "CSGD/SGD_String.h"

#include "TinyXML/tinyxml.h"

#include "AnimationSystem\AnimInfo.h"
#include "AnimationSystem\AnimationManager.h"
#include "TileSystem\TileManager.h"
#include "Util\PopUpManager.h"

#include "Messages/CreateMenuPopUp.h"

#include "states/MainMenuState.h"
#include "states/GameplayState.h"
#include "states/OptionState.h"
#include "states/StateManager.h"
//#include "states/PlayerSelectState.h"


#include "objects/EntityManager.h"
#include "objects/Player.h"
#include "objects/Entity.h"
#include <ctime>
#include "Inputs/Controller.h"
#include "Inputs/AudioManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <Shlobj.h>


// Instantiate the static member
/*static*/ CGame* CGame::s_pInstance = nullptr;

// GetInstance
//	- returns the ONE singleton object
/*static*/ CGame* CGame::GetInstance( void )
{
	// Allocate the static pointer if necessary
	if( s_pInstance == nullptr )
		s_pInstance = new CGame;

	// Return the singleton
	return s_pInstance;
}

// DeleteInstance
//	- deallocate the singleton object
/*static*/ void CGame::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


// Constructor
//	- set all the data members to 'clean' values
CGame::CGame(void)
{
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;
	m_pPUM			= nullptr;
	m_pAM			= nullptr;
	
	m_nScreenWidth	= 1;
	m_nScreenHeight	= 1;
	m_bIsWindowed	= true;
	m_pProfile		= nullptr;
	m_bFresh		= true;
	m_bShowFPS		= false;
	m_bLockScreen	= false;

	m_dwCurrTime= 0;
	m_nFPS		   = 0;
	m_nFrames	   = 0;
	m_fFPSTimer	   = 0.0f;


	srand( (unsigned int)time( nullptr ) );
}

// Destructor
CGame::~CGame(void)
{
}


// Initialize
//	- setup the SGD wrappers
//	- load resources
void CGame::Initialize( HWND hWnd, HINSTANCE hInstance,
					int nWidth, int nHeight,
					bool bIsWindowed )
{
	GameInstance	= hInstance;
	GamehWnd		= hWnd;
	// Store the SGD Wrapper singletons
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pXA			= CSGD_XAudio2::GetInstance();
	m_pAM			= AnimationManager::GetInstance();	
	
	// Store the parameters
	m_nScreenWidth	= nWidth;
	m_nScreenHeight	= nHeight;
	m_bIsWindowed	= bIsWindowed;


	// Initialize the wrappers
	m_pD3D->Initialize( hWnd, 
						m_nScreenWidth, m_nScreenHeight, 
						m_bIsWindowed );

	m_pDI->Initialize( hWnd, hInstance, 
						DI_KEYBOARD | DI_JOYSTICKS /*| DI_MOUSE | DI_JOYSTICKS*/ );

	m_pTM->Initialize( m_pD3D->GetDirect3DDevice(),
						m_pD3D->GetSprite() );

	m_pXA->Initialize();

	m_pMS = CSGD_MessageSystem::GetInstance();
	m_pMS->InitMessageSystem(MessageProc);

	m_pAUM = CAudioManager::GetInstance();
	m_pAUM->Initialize("config/audio.xml");

	m_pFont = new FontManager;
	m_pFont->Initialize("config/fonts.xml");

	m_pEM = new CEntityManager;

	// Starts in the Main Menu state
	m_pGameState = new StateManager;
	m_pGameState->Initialize();

	m_pCtrl = CController::GetInstance();

	Create_Save_Load_Directory();
	m_pProfile = CreateProfile(1,1,1);

	COptionState::GetInstance()->Import(m_pProfile);


	m_pPUM   = CPopUpManager::GetInstance(); 
	m_pPUM->Initialize("config/popups.xml");

	FirstRun(true);
	Tutorial(false);
}


// Main
//	- run one frame of the gameplay
bool CGame::Main( void )
{
	// Check input
	if( Input() == false )
		return false;

	// Check input
	

	// Update & render
	Update();
	Render();

	//if( IsController() == true )
	//	m_pGameState->AddState( CPlayerSelectState::GetInstance() );


	return true;
}

// Terminate
//	- deallocate the game entities
//	- unload the assets
//	- shutdown the SGD wrappers (in REVERSE ORDER)
void CGame::Terminate( void )
{

	if( m_pPUM != nullptr )
	{
		m_pPUM->Shutdown();
		m_pPUM = nullptr;		
	}

	
	SAFE_DELETE(m_pProfile);	


	if( m_pEM != nullptr )
	{
		m_pEM->RemoveAll();
		SAFE_DELETE(m_pEM);
	}

	if( m_pGameState != nullptr )
	{
		m_pGameState->Terminate();
		SAFE_DELETE(m_pGameState);
	}


	if( m_pFont != nullptr )
	{
		m_pFont->Shutdown();
		SAFE_DELETE(m_pFont);
	}

	if( m_pAUM != nullptr )
	{
		m_pAUM->Terminate();
		m_pAUM = nullptr;
	}

	m_pTileM->Terminate();
	m_pXA->Terminate();
	m_pTM->Terminate();
	m_pDI->Terminate();
	m_pD3D->Terminate();

	// Reset the data members to 'clean' values
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;
	
	m_nScreenWidth	= 1;
	m_nScreenHeight	= 1;
	m_bIsWindowed	= true;

}

// Input
//	- handle user input
bool CGame::Input( void )
{
	// Read the devices
	m_pDI->ReadDevices();

	//toggle full screen with Alt + Enter
	if( (m_pDI->KeyDown( DIK_LALT) == true) &&
		m_pDI->KeyPressed( DIK_RETURN ) == true )
	{
		m_pProfile->Fullscreen(!m_pProfile->Fullscreen());
		Fullscreen( m_pProfile->Fullscreen() != 0 );	
		return true;
	}

	//toggle language
	if( (m_pDI->KeyDown( DIK_LALT) == true) &&
		m_pDI->KeyPressed( DIK_L ) == true )
	{
		if(m_pProfile->Language())
			m_pProfile->Language(LANG_NEUTRAL);
		else 
			m_pProfile->Language(LANG_SPANISH);
	}

	// toggle fps with f1
	if (m_pDI->KeyPressed(DIK_F1) == true)
		m_bShowFPS = !m_bShowFPS;

	return m_pGameState->Input();	
}

// Update
//	- move the game entities
void CGame::Update( void )
{
	// Update the audio
	m_pXA->Update();

	DWORD dwNow = GetTickCount();
	float fElapsedTime = (dwNow - m_dwCurrTime) / 1000.0f;
	m_dwCurrTime = dwNow;


	// Cap the elapsed time to 1/8th
	if( fElapsedTime > 0.125f )
		fElapsedTime = 0.125f;


	// Increase the FPS counters
	++m_nFrames;
	m_fFPSTimer += fElapsedTime;
	if (m_fFPSTimer >= 1.0f)
	{
		m_nFPS = m_nFrames;
		m_nFrames = 0;
		m_fFPSTimer = 0.0f;
	}

	// Current state updates
	m_pGameState->Update( fElapsedTime );
	m_pCtrl->Update( fElapsedTime );
	m_pMS->ProcessMessages();


}

// Render
//	- render the game entities
void CGame::Render( void )
{
	// Clear the backbuffer
	m_pD3D->Clear( D3DCOLOR_XRGB( 255, 0, 0 ) );

	// Begin rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	m_pGameState->RenderAll();

	if (m_bShowFPS == true)
	{
		// Use an output string stream to format numbers
		TOSTRINGSTREAM output;
		output << _T("FPS: ") << m_nFPS;

		m_pD3D->DrawText(output.str().c_str(), 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0) );
	}

	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	// Present the back buffer to the screen
	m_pD3D->Present();
}

void CGame::Fullscreen( bool _screen )
{
	if(m_bLockScreen == false)
	{
		m_bIsWindowed = _screen;
		m_pD3D->Resize(m_nScreenWidth, m_nScreenHeight, m_bIsWindowed);
	}

}

void CGame::SaveProfile( int _slot )
{
	SaveProfile(m_pProfile, _slot);
}

void CGame::SaveProfile( GameInfo* pInfo, int _slot )
{
	TiXmlDocument x_saveSlot;

	//assert( x_saveSlot.LoadFile("config/save_slots.xml") != false && "XML save slot file could not be loaded");
//	if(m_szFilepath.c_str() == NULL)
	//	x_saveSlot.LoadFile("config/save_slots.xml");
	//else

	ifstream ifs( m_szFilepath.c_str() );
	if(ifs)
		x_saveSlot.LoadFile(m_szFilepath.c_str());
	else
		x_saveSlot.LoadFile("config/save_slots.xml");

	ifs.close();

	TiXmlElement* x_pRoot = x_saveSlot.RootElement();
	//assert( x_pRoot != nullptr && "XML root is NULL");

	int nSlot = 0;
	bool bClear = false;
	TiXmlElement* x_pSave = x_pRoot->FirstChildElement("Save");
	x_pSave->Attribute("slot", &nSlot);

	while(nSlot != _slot)
	{
		x_pSave = x_pSave->NextSiblingElement("Save");
		x_pSave->Attribute("slot", &nSlot);
	}

	if( pInfo == nullptr)
	{
		pInfo = CreateProfile();
		x_pSave->SetAttribute("empty", 0);
		bClear = true;
	}
	else
		x_pSave->SetAttribute("empty", 1);

	TiXmlElement* x_pPlayer = x_pSave->FirstChildElement("Player");
	x_pPlayer->SetAttribute("stage", pInfo->Stage());
	x_pPlayer->SetAttribute("Exp", pInfo->EXP());

	TiXmlElement* x_pUpgrades = x_pPlayer->NextSiblingElement("Upgrades");

	TiXmlElement* x_pItem = x_pUpgrades->FirstChildElement("Health");
	x_pItem->SetAttribute("level", pInfo->Health());

	x_pItem = x_pItem->NextSiblingElement("Special");
	x_pItem->SetAttribute("level", pInfo->Special());

	x_pItem = x_pItem->NextSiblingElement("Lives");
	x_pItem->SetAttribute("level", pInfo->Lives());

	x_pItem = x_pItem->NextSiblingElement("Fire");
	x_pItem->SetAttribute("level", pInfo->Fire());

	x_pItem = x_pItem->NextSiblingElement("Wind");
	x_pItem->SetAttribute("level", pInfo->Wind());

	x_pItem = x_pItem->NextSiblingElement("Ice");
	x_pItem->SetAttribute("level", pInfo->Ice());

	x_pItem = x_pItem->NextSiblingElement("Earth");
	x_pItem->SetAttribute("level", pInfo->Earth());


	TiXmlElement* x_pOptions = x_pUpgrades->NextSiblingElement("Options");

	TiXmlElement* x_pSetting = x_pOptions->FirstChildElement("fullscreen");
	x_pSetting->SetAttribute("value", pInfo->Fullscreen());

	x_pSetting = x_pSetting->NextSiblingElement("language");
	x_pSetting->SetAttribute("value", pInfo->Language());

	x_pSetting = x_pSetting->NextSiblingElement("BGM");
	x_pSetting->SetAttribute("value", pInfo->BGM());

	x_pSetting = x_pSetting->NextSiblingElement("SFX");
	x_pSetting->SetAttribute("value", pInfo->SFX() );

	
	x_saveSlot.SaveFile(m_szFilepath.c_str());
	//x_saveSlot.SaveFile("config/save_slots.xml");


	if(bClear)
		delete pInfo;
}

GameInfo*   CGame::LoadProfile( int _slot )
{

	TiXmlDocument x_loadslot;


	//assert( x_loadslot.LoadFile("config/save_slots.xml") != false && "XML save slot file could not be loaded");
	//x_loadslot.LoadFile("config/save_slots.xml");

	ifstream ifs( m_szFilepath.c_str() );
	if(ifs)
		x_loadslot.LoadFile(m_szFilepath.c_str());
	else
		x_loadslot.LoadFile("config/save_slots.xml");
	ifs.close();


	TiXmlElement* x_pRoot = x_loadslot.RootElement();


	//assert( x_pRoot != nullptr && "XML root is NULL");

	int nSlot = 0;
	TiXmlElement* x_save = x_pRoot->FirstChildElement("Save");
	x_save->Attribute("slot", &nSlot);

	while(nSlot != _slot)
	{
		x_save = x_save->NextSiblingElement("Save");
		x_save->Attribute("slot", &nSlot);
	}

	int isFilled = 0;
	x_save->Attribute("empty", &isFilled);

	if(isFilled == false && _slot > 0)
		return nullptr;	
	else
	{
		
		int stg, exp, hp, sp, life, fire, wind, ice, earth, fsc, lang, bgm, sfx;

		TiXmlElement* x_player = x_save->FirstChildElement("Player");
		x_player->Attribute("stage", &stg);
		x_player->Attribute("Exp", &exp);

		TiXmlElement* x_upgrades = x_player->NextSiblingElement("Upgrades");

		TiXmlElement* x_HP = x_upgrades->FirstChildElement("Health");
		x_HP->Attribute("level", &hp);

		TiXmlElement* x_Spl = x_HP->NextSiblingElement("Special");
		x_Spl->Attribute("level", &sp);

		TiXmlElement* x_lives = x_Spl->NextSiblingElement("Lives");
		x_lives->Attribute("level", &life);

		TiXmlElement* x_fire = x_lives->NextSiblingElement("Fire");
		x_fire->Attribute("level", &fire);

		TiXmlElement* x_wind = x_fire->NextSiblingElement("Wind");
		x_wind->Attribute("level", &wind);

		TiXmlElement* x_ice = x_wind->NextSiblingElement("Ice");
		x_ice->Attribute("level", &ice);

		TiXmlElement* x_earth = x_ice->NextSiblingElement("Earth");
		x_earth->Attribute("level", &earth);


		TiXmlElement* x_Options = x_upgrades->NextSiblingElement("Options");

		TiXmlElement* x_setting = x_Options->FirstChildElement("fullscreen");


		//Set the pointer to it's own child
		x_setting->Attribute("value", &fsc);
		
		x_setting = x_setting->NextSiblingElement("language");
		x_setting->Attribute("value", &lang);

		x_setting = x_setting->NextSiblingElement("BGM");
		x_setting->Attribute("value", &bgm);

		x_setting = x_setting->NextSiblingElement("SFX");
		x_setting->Attribute("value", &sfx);

		return CreateProfile(stg, exp, hp, sp, life, fire, wind, ice,
										earth, fsc, lang, bgm, sfx);
	}

}

void CGame::Import( GameInfo* _info )
{
	SAFE_DELETE(m_pProfile);
	m_pProfile = _info;
}

void CGame::MessageProc( IMessage* pMsg )
{
	CGame* self = CGame::GetInstance();

	if(pMsg == nullptr)
		return;

	switch( pMsg->GetMessageID() )
	{
	case MSG_CREATE_MENU_POPUP:
		{
			CCreateMenuPopUp* pCPopUP_Message = 
				dynamic_cast< CCreateMenuPopUp* >( pMsg );

			self->m_pPUM->Create(pCPopUP_Message->Type());
	
		}
	}
}

GameInfo* CGame::CreateProfile( int stg, int exp, int hp, int sp, int life, int fire, int wind, int ice,
							   int earth, int fsc, int lang, int bgm, int sfx )
{
	GameInfo* pInfo = new GameInfo(
		/*Stage		 Lv */ stg   ,  
		/*EXP 		 Lv */ exp   ,  
		/*Health	 Lv */ hp    ,  	
		/*Special	 Lv */ sp    ,  	
		/*Lives		 Lv */ life  ,  
		/*Fire		 Lv */ fire  ,  
		/*Wind		 Lv */ wind  ,  
		/*Ice		 Lv */ ice   ,     
		/*Earth		 Lv */ earth ,  
		/*Fullscreen Lv */ fsc   ,  	
		/*Language	 Lv */ lang  ,  
		/*BGM		 Lv */ bgm   ,      
		/*SFX		 Lv */ sfx      
						         );

	return pInfo;
}

void CGame::Create_Save_Load_Directory()
{
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t pathlen;

	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath); 
	wcstombs_s(&pathlen , path, wszPath, MAX_PATH);

	m_szFilepath = path;

	TOSTRINGSTREAM wss;
	 
	wss.str(L"");
	m_szFilepath += "\\Trials Of Mastery";
	wss << m_szFilepath.c_str();

	CreateDirectory(wss.str().c_str(),0);

	m_szFilepath += "\\save_slots.xml";
}

bool CGame::IsController( void )
{
	memset(&m_XpadState, 0, sizeof(XINPUT_STATE)); 
	DWORD result = XInputGetState(m_nContrNum, &m_XpadState); 
	 if(result == ERROR_DEVICE_NOT_CONNECTED)
		 return false;
	 else 
		 return true;
}







