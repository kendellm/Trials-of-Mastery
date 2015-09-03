/*************************************************************
File:			GameplayState.cpp
Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
Purpose:		handles all the game play			
*************************************************************/

#include "../Game.h"
#include "../GameInfo.h"
#include "GameplayState.h"
#include "StateManager.h"
#include "HomeMenuState.h"
#include "PauseMenuState.h"
#include "GameOverState.h"

#include "../objects/EntityManager.h"
#include "../objects/Player.h"
#include "../objects/Enemy.h"
#include "../objects/Grunt.h"
#include "../objects/Dummy.h"
#include "../objects/Tank.h"
#include "../objects/Fighter.h"
#include "../objects/EarthBoss.h"
#include "../objects/Trap.h"
#include "../objects/Projectile.h"
#include "../objects/Thrower.h"
#include "../objects/SubZero.h"
#include "../objects/Shockwave.h"
#include "../objects/SweepingWind.h"
#include "../objects/Fireball.h"
#include "../objects/Wrecking_Ball.h"
#include "../objects/CFireBoss.h"
#include "../objects/IceBoss.h"
#include "../objects/CWindBoss.h"
#include "../objects/Rock_Fall.h"
#include "../objects/Boss_Fire_Special.h"
#include "../objects/Iceball.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_DirectInput.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/SGD_Util.h"
#include "../CSGD/Toggle_Camera_Message.h"
#include "../CSGD/Create_ScreenLock_Message.h"
#include "../CSGD/Create_Trap_Message.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/RemoveEntityMessage.h"

#include "../Messages/Create_Fan_Message.h"
#include "../Messages/Create_Rock_Message.h"
#include "../Messages/Create_Wind_Tornado_Message.h"
#include "../Messages/Create_IceSpecial_Message.h"
#include "../Messages/Create_FireSpecial_Message.h"
#include "../Messages/Create_Enemy_Message.h"
#include "../Messages/Create_PopUp_Message.h"
#include "../Messages/CreatePlayerMessage.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../Messages/DropEXPMessage.h"
#include "../Messages/Perform_SubZero_Special_MSG.h"
#include "../Messages/Perform_Shockwave_Special_MSG.h"
#include "../Messages/Perform_Sweeping_Wind_MSG.h"
#include "../Messages/Perform_Fireball_Special_MSG.h"
#include "../Messages/Create_Wrecking_Ball_Message.h"
#include "../Messages/Create_FireBoss_FireBall_Message.h"
#include "../Messages/Create_Rock_Fall_Message.h"
#include "../Messages/Create_IceBall_Message.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"

#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../Util/PopUpManager.h"
#include "../Util/FontManager.h"
#include "../HUD.h"
#include "../Camera.h"


#include <cassert>

CGameplayState* CGameplayState::GetInstance( void )
{
	static CGameplayState s_Instance;
	return &s_Instance;
}

CGameplayState::CGameplayState(void)
{	
	m_pD3D		= nullptr;
	m_pDI		= nullptr;
	m_pTM		= nullptr;
	m_pXA		= nullptr;
	m_pFont		= nullptr;
	m_pMS		= nullptr;
	m_pES		= nullptr;
	m_pEM		= nullptr;
	m_pTileM	= nullptr;	
	m_pCamera	= nullptr;
	m_pPUM		= nullptr;
	m_pPlayer1	= nullptr;
	m_pPlayer2	= nullptr;

	m_nBackgroundImgID = -1;
	m_nLevel	= -1;
	m_bGameOver = false;
	m_bWin	  = false;
	m_bFirstTime = false;
	LockY = 0.0f;
}

CGameplayState::~CGameplayState(void)
{

}

void CGameplayState::Enter( void )
{
	Active(true);

	m_pD3D		= CSGD_Direct3D::GetInstance();
	m_pDI		= CSGD_DirectInput::GetInstance();
	m_pTM		= CSGD_TextureManager::GetInstance();
	m_pXA		= CSGD_XAudio2::GetInstance();
	m_pFont		= CGame::GetInstance()->GetFont();
	m_pEM		= CGame::GetInstance()->EntityManager();
	m_pES		= CSGD_EventSystem::GetInstance();
	m_pMS		= CSGD_MessageSystem::GetInstance();
	m_pCamera	= CCamera::GetInstance();
	m_pPUM		= CGame::GetInstance()->PopUpManager();
	m_pCamera->SetPosX(0.0f);
	m_pCamera->SetPosY(0.0f);
	m_pCamera->SetHeight(600);
	m_pCamera->SetWidth(800);

	m_pMS->InitMessageSystem(MessageProc);
	m_pES->RegisterClient( "GAME_OVER", this );
	m_pES->RegisterClient( "PLAYER_DEAD", this );

	m_pTileM	= TileManager::GetInstance();

	CreateLevel(m_nLevel);

	m_pPlayer1 = CreatePlayer( CGame::GetInstance()->GetProfile(), 100,400);
	m_pEM->AddEntity(m_pPlayer1, m_pEM->PLAYER_LAYER);

	m_pHUD = new CHUD;

	wave_Count_Down = 0;
	current_Wave = 0;
	m_bGameOver = false;
	m_bWin	  = false;
	CToggle_Camera_Message * p_ToggleC_Message = new CToggle_Camera_Message(false);
	CSGD_MessageSystem::GetInstance()->SendMsg(p_ToggleC_Message);
	CPopUpManager::GetInstance()->Register_Completed();
}

void CGameplayState::Exit( void )
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer1);
	if(m_bWin)
		CGame::GetInstance()->GetProfile()->Stage(max(m_nLevel + 1, pPlayer->Level() ) );


	Reset_Screen_Lock();
	CSGD_EventSystem::GetInstance()->SendEventNow("Pop_Up_Completed", (void*)0, (void*)0, (void*)0);

	CGame::GetInstance()->GetProfile()->EXP( pPlayer->EXP()) ;



	if( m_pPlayer1 != nullptr )
	{
		m_pEM->RemoveEntity( m_pPlayer1 );
		m_pPlayer1->Release();
		m_pPlayer1 = nullptr;
	}

	SAFE_DELETE(m_pHUD);

	m_pTM->UnloadTexture(m_nBackgroundImgID);

	if( m_pEM != nullptr )
	{
		m_pEM->RemoveAll();
		m_pEM = nullptr;
	}
	CSGD_EventSystem::GetInstance()->UnregisterClientAll( this );

	if(m_pES != nullptr )
	{
		m_pES->ShutdownEventSystem();
		m_pES = nullptr;
	}

	if( m_pMS != nullptr )
	{
		m_pMS->ShutdownMessageSystem();
		m_pMS = nullptr;
	}

	m_pPUM			= nullptr;
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;
	m_pFont			= nullptr;
	m_nLevel		= -1;


}

bool CGameplayState::Input( void )
{
	if( CController::GetInstance()->Pause() )
	{
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # 6 / 56
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		CSGD_EventSystem::GetInstance()->SendEventNow("Game Paused", nullptr, m_pPlayer1, nullptr);

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # 
		//////////////////////////////////////////////////////////////////////////

		CGame::GetInstance()->GetState()->AddState(CPauseMenuState::GetInstance());
		return true;
	}

	//if (m_pDI->JoystickButtonPressed(6)	== true && m_pPlayer2 == nullptr)
	//{
	//	// Create player 2 in the middle of the screen
	//	m_pPlayer2 = CreatePlayer(CGame::GetInstance()->GetProfile(), 50, 600, false);
	//	m_pEM->AddEntity(m_pPlayer2, m_pEM->PLAYER_LAYER);
	//}

	return true;
}

void CGameplayState::Update( float fElaspedTime )
{
	m_pEM->UpdateAll(fElaspedTime);

	if(m_bGameOver)
	{
		CGameOverState::GetInstance()->SetWin(m_bWin);
		CGameOverState::GetInstance()->SetLevelFinished(m_nLevel);
		CGame::GetInstance()->GetState()->ChangeState(CGameOverState::GetInstance());
		return;
	}

	HandleScreenLock();

	m_pCamera->Update(m_pPlayer1);


	m_pES->ProcessEvents();
	m_pMS->ProcessMessages();

	m_pHUD->Update(fElaspedTime);
	m_pPUM->Update(fElaspedTime);
}

void CGameplayState::Render( void )
{
	m_pD3D->Clear(D3DCOLOR_ARGB(255, 150, 150, 255));

	RenderBG();
	m_pTileM->Render();

	m_pEM->RenderAllZBuffered();
	m_pHUD->Render();

	m_pPUM->Render();

}

void CGameplayState::RenderBG()
{

	int h = (int)m_pCamera->GetHeight(); //Get camera height
	int w = (int)m_pCamera->GetWidth();
	RECT r;
	if(m_nLevel == 0)
	{
		r.left = 0;

		r.right =  w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.top = 0;
		r.bottom = 559;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.right = w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.top = r.bottom + 1;
		r.bottom = 1146;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.top = r.bottom + 4;
		r.bottom = 1482;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/2) % w)/w;;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()) % w)/w;
		r.top = r.bottom + 1;
		r.bottom = 1839;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);


		r.top = r.bottom + 1;
		r.bottom = 1895;
		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()) % w)/w;
		r.top = r.bottom + 1;
		r.bottom = 1984;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



	}
	else if( m_nLevel == 1)
	{
		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.top = 0;
		r.bottom = 579;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.right = w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.top = 589;
		r.bottom = 922;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.top = 946;
		r.bottom = 1218;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()) % w)/w;
		r.top = 1242;
		r.bottom = 1484;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);
	}
	else if(m_nLevel == 2)
	{
		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.top = 0;
		r.bottom = 597;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.right = w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.top = 565;
		r.bottom = 965;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 150, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 150, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.top = 988;
		r.bottom = 1356;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 150, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 150, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.top = 1376;
		r.bottom = 1635;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 125, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 125, 1.0f, 1.0f, &r);
	}
	else if(m_nLevel == 3)
	{
		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.top = 1049;
		r.bottom = 1648;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.top = 0;
		r.bottom = 143;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 350, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 350, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.top = 153;
		r.bottom = 382;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 315, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 315, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/1) % w)/w;
		r.top = 393;
		r.bottom = 602;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 165, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/1) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 165, 1.0f, 1.0f, &r);


	}
	else if(m_nLevel == 4)
	{
		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.top = 0;
		r.bottom = 512;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 100, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/4) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 100, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.top = 531;
		r.bottom = 992;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 50, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/3) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 50, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.top = 1013;
		r.bottom = 1294;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/2) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 200, 1.0f, 1.0f, &r);



		r.left = 0;
		r.right =  w * ((int)(m_pCamera->GetPosX()/1) % w)/w;
		r.top = 1313;
		r.bottom = 1552;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 125, 1.0f, 1.0f, &r);

		r.left = w * ((int)(m_pCamera->GetPosX()/1) % w)/w;
		r.right =  w;
		m_pTM->Draw(m_nBackgroundImgID, w - r.right, h + r.top - r.bottom - 125, 1.0f, 1.0f, &r);
	}
}

void CGameplayState::HandleScreenLock()
{
	if(ScreenLockWaves.size() > 0 && (unsigned int)current_Wave < ScreenLockWaves.size() )
	{
		if(wave_Count_Down <= 0)
		{
			wave_Count_Down = ScreenLockWaves[current_Wave].size();
			CPlayer* pPlayer =
				dynamic_cast< CPlayer* >( m_pPlayer1 );
			for(int i = 0; i < wave_Count_Down; i++)
			{
				Create_Enemy_Message* CE_Message = new Create_Enemy_Message(ScreenLockWaves[current_Wave][i], pPlayer);
				CSGD_MessageSystem::GetInstance()->SendMsg(CE_Message);
				CE_Message = nullptr;
			}
			current_Wave++;
		}
	}
	if(wave_Count_Down == 0)
	{
		Reset_Screen_Lock();
	}
}

void CGameplayState::Reset_Screen_Lock()
{
	std::vector<std::vector<int>> Temp;
	Temp.resize(0);
	ScreenLockWaves  = Temp;

	CToggle_Camera_Message * p_ToggleC_Message = new CToggle_Camera_Message(false);
	CSGD_MessageSystem::GetInstance()->SendMsg(p_ToggleC_Message);
	current_Wave = 0;
	wave_Count_Down = -1;
}

CEntity* CGameplayState::CreatePlayer(  GameInfo* pInfo, float x, float y, bool bIsPlayerOne, bool bPvp )
{
	int HP = pInfo->Health()  * 100;

	int SP = pInfo->Special() * 50;



	CPlayer* pPlayer = new CPlayer(HP, SP, pInfo->Lives() + 1, pInfo->EXP(),
		pInfo->Stage(), pInfo->Fire(), pInfo->Wind(), pInfo->Ice(), pInfo->Earth());


	pPlayer->SetCheckPoint(x,y);
	pPlayer->SetPosition(x,y);
	CSGD_EventSystem::GetInstance()->SendEventNow("New_Player", pPlayer);

	return (CEntity*)pPlayer;
}

/*static*/ void	CGameplayState::MessageProc( IMessage* pMsg )
{
	// Create a pointer to the singleton to replace the invoking object
	CGameplayState* self = CGameplayState::GetInstance();
	CEntityManager* m_pEM = self->m_pEM;

	if(pMsg == nullptr)
		return;
	// What type of message?
	switch( pMsg->GetMessageID() )
	{
	case MSG_REMOVE_ENTITY:
		{
			// Downcast to the actual message type
			CRemoveEntityMessage* pDestroyMsg =
				dynamic_cast< CRemoveEntityMessage* >( pMsg );

			assert( pDestroyMsg != nullptr 
				&& "CGame::MessageProc - MSG_DESTROY_ENTITY is not actually a CDestroyEntityMessage" );


			// Get the entity that is attached to the message
			CEntity* pEntity = pDestroyMsg->GetEntity();

			CGameplayState::GetInstance()->m_pEM->RemoveEntity( pEntity );
			if(pEntity->GetType() == CEntity::ENT_ENEMY )
			{
				// Remove the entity from the Entity Manager
				// USING THE CGAME SINGLETON!!!
				self->wave_Count_Down = self->wave_Count_Down - 1;
			}
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			Create_Enemy_Message* pCE_Message = 
				dynamic_cast< Create_Enemy_Message* >( pMsg );

			switch(pCE_Message->Get_Enemy_Type())
			{
			case CEnemy::Grunt:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CGrunt* Grunt;
						switch(self->m_nLevel)
						{
						case 2:
							Grunt = new CGrunt(0.25f);
							break;

						case 3:
							Grunt = new CGrunt(0.5f);
							break;

						case 4:
							Grunt = new CGrunt(0.75f);
							break;
						default:
							Grunt = new CGrunt(0.0f);
							break;
						}

						if(self->m_pCamera->GetPosX() <= 128)
							Grunt->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64 ));
						else
						{
							if(rand() % 2 == 0)
								Grunt->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								Grunt->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							Grunt->SetPosY(self->LockY);
						else
							Grunt->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						Grunt->SetTarget(pCE_Message->Get_Target());
						Grunt->SetVelX(0.0f);
						Grunt->SetVelY(0.0f);
						Grunt->m_fStartPosX = Grunt->GetPosX();
						Grunt->m_fStartPosY = Grunt->GetPosY();
						m_pEM->AddEntity(Grunt, m_pEM->ENEMY_LAYER);
						Grunt->Release();
					}
				}
				break;

			case CEnemy::Dummy:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CDummy* Dummy = new CDummy();
						Dummy->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						if(self->wave_Count_Down > 0)
							Dummy->SetPosY(self->LockY);
						else
							Dummy->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						Dummy->SetTarget(pCE_Message->Get_Target());
						Dummy->SetVelX(0.0f);
						Dummy->SetVelY(0.0f);
						Dummy->m_fStartPosX = Dummy->GetPosX();
						Dummy->m_fStartPosY = Dummy->GetPosY();
						m_pEM->AddEntity(Dummy, m_pEM->ENEMY_LAYER);
						Dummy->Release();
					}
				}
				break;

			case CEnemy::Fighter:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CFighter* Fighter;
						switch(self->m_nLevel)
						{
						case 2:
							Fighter = new CFighter(0.25f);
							break;

						case 3:
							Fighter = new CFighter(0.5f);
							break;

						case 4:
							Fighter = new CFighter(0.75f);
							break;
						default:
							Fighter = new CFighter(0.0f);
							break;
						}

						if(self->wave_Count_Down > 0)
							Fighter->SetPosY(self->LockY);
						else
							Fighter->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						if(self->m_pCamera->GetPosX() <= 128)
							Fighter->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								Fighter->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								Fighter->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						Fighter->SetTarget(pCE_Message->Get_Target());
						Fighter->SetVelX(0.0f);
						Fighter->SetVelY(0.0f);
						Fighter->m_fStartPosX = Fighter->GetPosX();
						Fighter->m_fStartPosY = Fighter->GetPosY();
						m_pEM->AddEntity(Fighter, m_pEM->ENEMY_LAYER);
						Fighter->Release();
					}
				}
				break;

			case CEnemy::Tank:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CTank* Tank;
						switch(self->m_nLevel)
						{
						case 2:
							Tank = new CTank(0.25f);
							break;

						case 3:
							Tank = new CTank(0.5f);
							break;

						case 4:
							Tank = new CTank(0.75f);
							break;
						default:
							Tank = new CTank(0.0f);
							break;
						}

						if(self->m_pCamera->GetPosX() <= 128)
							Tank->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								Tank->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								Tank->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							Tank->SetPosY(self->LockY);
						else
							Tank->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						Tank->SetTarget(pCE_Message->Get_Target());
						Tank->SetVelX(0.0f);
						Tank->SetVelY(0.0f);
						Tank->m_fStartPosX = Tank->GetPosX();
						Tank->m_fStartPosY = Tank->GetPosY();
						m_pEM->AddEntity(Tank, m_pEM->ENEMY_LAYER);
						Tank->Release();
					}
				}
				break;

			case CEnemy::EarthBoss:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CEarthBoss* Boss1 = new CEarthBoss();
						if(self->m_pCamera->GetPosX() <= 128)
							Boss1->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								Boss1->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								Boss1->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							Boss1->SetPosY(self->LockY);
						else
							Boss1->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						Boss1->SetTarget(pCE_Message->Get_Target());
						Boss1->SetVelX(0.0f);
						Boss1->SetVelY(0.0f);
						Boss1->m_fStartPosX = Boss1->GetPosX();
						Boss1->m_fStartPosY = Boss1->GetPosY();
						m_pEM->AddEntity(Boss1, m_pEM->ENEMY_LAYER);
						Boss1->Release();

						CAudioManager::GetInstance()->Play(BGM::BOSS);
					}
				}
				break;

			case CEnemy::FireBoss:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CFireBoss* FireBoss = new CFireBoss();
						if(self->m_pCamera->GetPosX() <= 128)
							FireBoss->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								FireBoss->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								FireBoss->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							FireBoss->SetPosY(self->LockY);
						else
							FireBoss->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						FireBoss->SetTarget(pCE_Message->Get_Target());
						FireBoss->SetVelX(0.0f);
						FireBoss->SetVelY(0.0f);
						FireBoss->m_fStartPosX = FireBoss->GetPosX();
						FireBoss->m_fStartPosY = FireBoss->GetPosY();
						m_pEM->AddEntity(FireBoss, m_pEM->ENEMY_LAYER);
						FireBoss->Release();
						CAudioManager::GetInstance()->Play(BGM::BOSS);

					}
				}
				break;

			case CEnemy::WindBoss:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CWindBoss* WindBoss = new CWindBoss();
						if(self->m_pCamera->GetPosX() <= 128)
							WindBoss->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								WindBoss->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								WindBoss->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							WindBoss->SetPosY(self->LockY);
						else
							WindBoss->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						WindBoss->SetTarget(pCE_Message->Get_Target());
						WindBoss->SetVelX(0.0f);
						WindBoss->SetVelY(0.0f);
						WindBoss->m_fStartPosX = WindBoss->GetPosX();
						WindBoss->m_fStartPosY = WindBoss->GetPosY();
						m_pEM->AddEntity(WindBoss, m_pEM->ENEMY_LAYER);
						WindBoss->Release();
						CAudioManager::GetInstance()->Play(BGM::BOSS);

					}
				}
				break;

			case CEnemy::IceBoss:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CIceBoss* IceBoss = new CIceBoss();
						if(self->m_pCamera->GetPosX() <= 128)
							IceBoss->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								IceBoss->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								IceBoss->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							IceBoss->SetPosY(self->LockY);
						else
							IceBoss->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						IceBoss->SetTarget(pCE_Message->Get_Target());
						IceBoss->SetVelX(0.0f);
						IceBoss->SetVelY(0.0f);
						IceBoss->m_fStartPosX = IceBoss->GetPosX();
						IceBoss->m_fStartPosY = IceBoss->GetPosY();
						m_pEM->AddEntity(IceBoss, m_pEM->ENEMY_LAYER);
						IceBoss->Release();
						CAudioManager::GetInstance()->Play(BGM::BOSS);

					}
				}
				break;

			case CEnemy::Thrower:
				{
					if(pCE_Message->Get_Target() != nullptr)
					{
						CThrower* Thrower;
						switch(self->m_nLevel)
						{
						case 2:
							Thrower = new CThrower(0.25f);
							break;

						case 3:
							Thrower = new CThrower(0.5f);
							break;

						case 4:
							Thrower = new CThrower(0.75f);
							break;
						default:
							Thrower = new CThrower(0.0f);
							break;
						}

						if(self->m_pCamera->GetPosX() <= 128)
							Thrower->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
						else
						{
							if(rand() % 2 == 0)
								Thrower->SetPosX( (float) (self->m_pCamera->GetPosX() + self->m_pCamera->GetWidth() + 64));
							else
								Thrower->SetPosX( (float) (self->m_pCamera->GetPosX() - 64)  );
						}
						if(self->wave_Count_Down > 0)
							Thrower->SetPosY(self->LockY);
						else
							Thrower->SetPosY((float)(pCE_Message->Get_Target()->GetPosY()));
						Thrower->SetTarget(pCE_Message->Get_Target());
						Thrower->SetVelX(0.0f);
						Thrower->SetVelY(0.0f);
						Thrower->m_fStartPosX = Thrower->GetPosX();
						Thrower->m_fStartPosY = Thrower->GetPosY();
						m_pEM->AddEntity(Thrower, m_pEM->ENEMY_LAYER);
						Thrower->Release();
					}
				}
				break;
			default:
				//do nothing
				break;
			}

			break;
		}
	case MSG_CREATE_POPUP:
		{
			Create_PopUp_Message* pCPopUP_Message = 
				dynamic_cast< Create_PopUp_Message* >( pMsg );

			CGameplayState::GetInstance()->m_pPUM->Load(pCPopUP_Message->Type());
			if(pCPopUP_Message->Type() == 0  || pCPopUP_Message->Type() == 1 || pCPopUP_Message->Type() == 2 || pCPopUP_Message->Type() == 3 || pCPopUP_Message->Type() == 4)
			{
				CToggle_Camera_Message * temp = new CToggle_Camera_Message(true);
				self->m_pMS->SendMsg(temp);
			}


		}
		break;
	case MSG_TOGGLE_CAMERA_LOCK:
		{
			CToggle_Camera_Message * p_ToggleC_Message = 
				dynamic_cast< CToggle_Camera_Message* >( pMsg );
			self->m_pCamera->SetLocked(p_ToggleC_Message->GetSet_To());
			if(p_ToggleC_Message->GetSet_To() == false)
			{

			}
			//sets the camera to locked or unlocked based off the message's Set_To
			break;
		}
	case MSG_CREATE_SCREEN_LOCK:
		{
			CCreate_ScreenLock_Message * p_SL_Message = 
				dynamic_cast< CCreate_ScreenLock_Message* >( pMsg );
			self->ScreenLockWaves = p_SL_Message->GetWaves();

			CToggle_Camera_Message * p_ToggleC_Message = new CToggle_Camera_Message(true);
			CSGD_MessageSystem::GetInstance()->SendMsg(p_ToggleC_Message);
			self->LockY = self->m_pPlayer1->GetPosY();
			break;
		}
	case MSG_CREATE_TRAP:
		{
			Create_Trap_Message* p_Trap_Message = dynamic_cast< Create_Trap_Message* >( pMsg );
			CTrap* Trap = new CTrap();
			Trap->SetPosX(p_Trap_Message->GetPosX());
			Trap->SetTrapType(p_Trap_Message->GetTrapType());
			Trap->SetPosY(p_Trap_Message->GetPosY());
			self->m_pEM->AddEntity(Trap, self->m_pEM->TRAP_LAYER);
			Trap->Release();
			break;
		}
	case MSG_CREATE_PLAYER:
		{
			CreatePlayerMessage* pPlayerMsg = dynamic_cast< CreatePlayerMessage*>(pMsg);
			CPlayer* pPlayer =   pPlayerMsg->GetPlayer();

			CGame::GetInstance()->GetProfile()->EXP(pPlayer->EXP());
			CEntity* temp;

			temp = self->CreatePlayer( CGame::GetInstance()->GetProfile(), 100,400);
			m_pEM->AddEntity(temp, m_pEM->PLAYER_LAYER);
			self->m_bWin = false;

			dynamic_cast<CPlayer*>(temp)->Lives( pPlayer->Lives()-1 );

			//Set the new player to the checkpoint, and sets their check point to the same spot
			float x = dynamic_cast<CPlayer*>(self->m_pPlayer1)->GetCheckPointX();
			float y = dynamic_cast<CPlayer*>(self->m_pPlayer1)->GetCheckPointY();
			dynamic_cast<CPlayer*>(temp)->SetPosX(x);
			dynamic_cast<CPlayer*>(temp)->SetPosY(y);
			dynamic_cast<CPlayer*>(temp)->SetCheckPoint(x, y);

			m_pEM->RemoveEntity( self->m_pPlayer1 );
			self->m_pPlayer1->Release();
			self->m_pPlayer1 = temp;		

		}
		break;
	case MSG_CREATE_PROJECTILE:
		{
			CCreate_Projectile_Message* pProjectMsg = dynamic_cast< CCreate_Projectile_Message*>(pMsg);
			if (pProjectMsg->GetAnimationName() == "")
			{
				CProjectile* temp = new CProjectile;
				temp->SetFlipped(pProjectMsg->GetFlipped());
				temp->SetOwnerType(pProjectMsg->GetOwner());
				temp->SetPosX(pProjectMsg->GetPosX());
				temp->SetPosY(pProjectMsg->GetPosY());
				m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
				temp->Release();
				CAudioManager::GetInstance()->Play(PLAYER_FIREBALL_SPAWN);
			}
			else
			{
				CProjectile* temp = new CProjectile(pProjectMsg->GetAnimationName(), 200, pProjectMsg->GetFlipped());
				temp->SetFlipped(pProjectMsg->GetFlipped());
				temp->SetOwnerType(pProjectMsg->GetOwner());
				temp->SetPosX(pProjectMsg->GetPosX());
				temp->SetPosY(pProjectMsg->GetPosY());
				m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
				temp->Release();
				CAudioManager::GetInstance()->Play(PLAYER_FIREBALL_SPAWN);

			}
			break;
		}
	case MSG_FIREBOSS_FIREBALL:
		{
			CCreate_FireBoss_Fireball_Message* pProjectMsg = dynamic_cast< CCreate_FireBoss_Fireball_Message*>(pMsg);
			CProjectile* temp = new CProjectile("config/FireBoss_FireBall_Animation.xml", "FireBoss_FireBall_Animation", 300.0f, false);
			temp->SetFlipped(pProjectMsg->GetFlipped());
			temp->SetOwnerType(pProjectMsg->GetOwner());
			temp->SetPosX(pProjectMsg->GetPosX());
			temp->SetPosY(pProjectMsg->GetPosY());
			m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
			temp->Release();
			//CAudioManager::GetInstance()->Play(PLAYER_FIREBALL_SPAWN);
			break;
		}
	case MSG_FIRE_SPECIAL:
		{
			CCreate_FireSpecial_Message* pProjectMsg = dynamic_cast< CCreate_FireSpecial_Message*>(pMsg);
			CProjectile* temp = new CProjectile("config/FireBoss_Explosion_Animation.xml", "FireBoss_Explosion_Animation", 0.0f, true);
			temp->SetFlipped(pProjectMsg->GetFlipped());
			temp->SetOwnerType(pProjectMsg->GetOwner());
			temp->SetPosX(pProjectMsg->GetPosX());
			temp->SetPosY(pProjectMsg->GetPosY());
			m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
			temp->Release();

			break;
		}
	case MSG_WIND_FAN:
		{
			CCreate_Fan_Message* pProjectMsg = dynamic_cast< CCreate_Fan_Message*>(pMsg);
			CProjectile* temp = new CProjectile("config/WindBoss_Fan_Animation.xml", "WindBoss_Fan_Animation", 325.0f, false);
			temp->SetFlipped(pProjectMsg->GetFlipped());
			temp->SetOwnerType(pProjectMsg->GetOwner());
			temp->SetPosX(pProjectMsg->GetPosX());
			temp->SetPosY(pProjectMsg->GetPosY());
			m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
			temp->Release();
			break;
		}
	case MSG_WIND_TORNADO:
		{
			CCreate_Wind_Tornado_Message* pProjectMsg = dynamic_cast< CCreate_Wind_Tornado_Message*>(pMsg);
			CProjectile* temp = new CProjectile("config/WindBoss_Tornado_Animation.xml", "WindBoss_Tornado_Animation", 175.0f, true);
			temp->SetFlipped(pProjectMsg->GetFlipped());
			temp->SetOwnerType(pProjectMsg->GetOwner());
			temp->SetPosX(pProjectMsg->GetPosX());
			temp->SetPosY(pProjectMsg->GetPosY());
			m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
			temp->Release();
			break;
		}
	case MSG_ROCK_SPECIAL:
		{
			break;
		}
		break;
	case MSG_ICE_SPECIAL:
		{

			CCreate_IceSpecial_Message* pProjectMsg = dynamic_cast< CCreate_IceSpecial_Message*>(pMsg);
			CProjectile* temp = new CProjectile("config/IceBoss_Spike_Special.xml", "IceBoss_Spike_Special", 0.0f, true);
			temp->SetFlipped(pProjectMsg->GetFlipped());
			temp->SetOwnerType(pProjectMsg->GetOwner());
			temp->SetPosX(pProjectMsg->GetPosX());
			temp->SetPosY(pProjectMsg->GetPosY());
			m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
			temp->Release();

			break;
		}
	case MSG_DROP_EXP:
		{
			CDropEXPMessage* pExpMsg = dynamic_cast< CDropEXPMessage*>(pMsg);			
			self->m_pHUD->ShowEXP(pExpMsg->GetExp(), 3);
			CPlayer* pP1 = 	dynamic_cast<CPlayer*>(self->m_pPlayer1);
			pP1->EXP( pP1->EXP() + pExpMsg->GetExp() );
		}
		break;
	case MSG_PERFORM_SUBZERO:
		{
			CPerform_SubZero_Special_MSG* ice = dynamic_cast<CPerform_SubZero_Special_MSG*>(pMsg);
			CSubZero* freeze = new CSubZero(ice->GetLevel());

			freeze->SetPosX (     ice->GetPosX()    );
			freeze->SetPosY (     ice->GetPosY()    );
			freeze->SetOwnerType( ice->GetOwner()   );

			m_pEM->AddEntity( freeze, m_pEM->PROJECTILE_LAYER );
			freeze->Release();
		}
		break;
	case MSG_PERFORM_SHOCKWAVE:
		{
			CPerform_Shockwave_Special_MSG* earth = dynamic_cast<CPerform_Shockwave_Special_MSG*>(pMsg);
			CShockwave* shockwave = new CShockwave(earth->GetLevel());
			shockwave->SetFlipped(earth->GetFlipped());
			shockwave->SetLevel(     earth->GetLevel()   );
			shockwave->SetPosX (     earth->GetPosX()    );
			shockwave->SetPosY (     earth->GetPosY()    );
			shockwave->SetOwnerType( earth->GetOwner()   );

			m_pEM->AddEntity( shockwave, m_pEM->PROJECTILE_LAYER );
			shockwave->Release();
		}
		break;
	case MSG_PERFORM_SWEEPING_WIND:
		{
			CPerform_Sweeping_Wind_MSG* wind = dynamic_cast<CPerform_Sweeping_Wind_MSG*>(pMsg);
			CSweepingWind* push = new CSweepingWind;
			push->SetFlipped(   wind->GetFlipped()    );
			push->SetLevel(     wind->GetLevel()      );
			push->SetPosX (     wind->GetPosX()       );
			push->SetPosY (     wind->GetPosY()       );
			push->SetOwnerType( wind->GetOwner()      );

			m_pEM->AddEntity( push, m_pEM->PROJECTILE_LAYER );
			push->Release();
		}
		break;
	case MSG_PERFORM_FIREBALL:
		{
			CPerform_Fireball_Special_MSG* fire = dynamic_cast< CPerform_Fireball_Special_MSG*>(pMsg);

			CFireball* fireSpl = new CFireball(fire->GetLevel());
			fireSpl->SetFlipped(   fire->GetFlipped()    );
			fireSpl->SetPosX (     fire->GetPosX()       );
			fireSpl->SetPosY (     fire->GetPosY()       );
			fireSpl->SetOwnerType( fire->GetOwner()      );

			m_pEM->AddEntity( fireSpl, m_pEM->PROJECTILE_LAYER );
			fireSpl->Release();
			CAudioManager::GetInstance()->Play(PLAYER_FIREBALL_SPAWN);
		}
		break;
	case MSG_CREATE_WRECKING_BALL:
		{
			CCreate_Wrecking_Ball_Message * pProjectMsg = dynamic_cast<CCreate_Wrecking_Ball_Message*>(pMsg);
			CWrecking_Ball* temp = new CWrecking_Ball();
			temp->SetFlipped(pProjectMsg->GetFlipped());
			temp->SetOwnerType(pProjectMsg->GetOwner());
			temp->SetPosX(pProjectMsg->GetPosX());
			temp->SetPosY(0);
			m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
			temp->Release();
		}
		break;
	case MSG_CREATE_ROCK_FALL:
		{
			CCreate_Rock_Fall_Message * pProjectMsg = dynamic_cast<CCreate_Rock_Fall_Message*>(pMsg);
			CCamera* Game_Camera = CCamera::GetInstance();
			int rocks = rand() % 5 + 3;
			std::vector<int> slots;
			slots.resize(rocks);
			for(int i = 0; i < rocks; i++)
				slots[i] = -1;

			int spot;
			for(int i = 0; i < rocks; i++)
			{
				spot = rand() % 9;
				for(int j = 0; j < rocks; j++)
				{
					if(slots[j] == spot)
					{
						spot = rand() % 9;
						j = -1;
					}
				}
				slots[i] = spot;

				CRock_Fall* temp = new CRock_Fall();
				temp->SetFlipped(false);
				temp->SetOwnerType(pProjectMsg->GetOwner());
				temp->SetPosX(Game_Camera->GetPosX() + spot * 85 + 85);
				temp->SetPosY(0);

				m_pEM->AddEntity(temp, m_pEM->PROJECTILE_LAYER);
				temp->Release();
			}


		}
		break;
	case MSG_ICEBALL:
		{
			CCreate_IceBall_Message* Msg = dynamic_cast< CCreate_IceBall_Message*>(pMsg);

			CIceball* IceBall = new CIceball();

			IceBall->SetPosX(Msg->GetPosX());
			IceBall->SetPosY(Msg->GetPosY());
			IceBall->SetFlipped(Msg->GetFlipped());

			m_pEM->AddEntity(IceBall, m_pEM->PROJECTILE_LAYER);
			IceBall->Release();

			break;
		}
	default:
		break;
	}

}

void CGameplayState::CreateLevel( int nlevel )
{

	switch (nlevel)
	{
	case 0:
		{
			m_pTileM->LoadLevels("config/Level_0.xml");

			m_nBackgroundImgID = m_pTM->LoadTexture(
				_T("resource/Dojo_BackGround.png"));

			CAudioManager::GetInstance()->Play(BGM::DOJO);

			if(CGame::GetInstance()->FirstRun() == true)
				m_bFirstTime = true;
		}
		break;
	case 1:
		{
			m_pTileM->LoadLevels("config/Level_1.xml");

			m_nBackgroundImgID = m_pTM->LoadTexture(
				_T("resource/Mountain_BackGround.png"));

			CAudioManager::GetInstance()->Play(BGM::MOUNTAIN);
		}
		break;
	case 2:
		{
			m_pTileM->LoadLevels("config/Level_2.xml");

			m_nBackgroundImgID = m_pTM->LoadTexture(
				_T("resource/Plains_BackGround.png"));

			CAudioManager::GetInstance()->Play(BGM::DOJO);
		}
		break;
	case 3:
		{
			m_pTileM->LoadLevels("config/Level_3.xml");

			m_nBackgroundImgID = m_pTM->LoadTexture(
				_T("resource/Forest_BackGround.png"));

			CAudioManager::GetInstance()->Play(BGM::DOJO);
		}
		break;
	case 4:
		{
			m_pTileM->LoadLevels("config/Level_4.xml");

			m_nBackgroundImgID = m_pTM->LoadTexture(
				_T("resource/Desert_BackGround.png"));

			CAudioManager::GetInstance()->Play(BGM::DOJO);
		}
		break;
	case 5:
		{
			//m_pTileM->LoadLevels("config/Level_5.xml");

			//m_nBackgroundImgID = m_pTM->LoadTexture( 
			//	_T("");

			// CAudioManager::GetInstance()->Play(BGM:: );
		}

	default:
		break;
	}
}

void CGameplayState::HandleEvent( CEvent* pEvent )
{
	if( pEvent->GetEventID() == "GAME_OVER" )
	{
		bool* pWin = reinterpret_cast< bool* >( pEvent->GetParam() );
		m_bWin = (pWin != 0);
		m_bGameOver = true;
	}

	if( pEvent->GetEventID() == "PLAYER_DEAD" )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer1);
		if(pPlayer->Lives() <=0) 
		{
			m_bWin = false;
			m_bGameOver = true;
		}
		else
		{
			CreatePlayerMessage* pMsg = new CreatePlayerMessage(pPlayer);
			CSGD_MessageSystem::GetInstance()->SendMsg( pMsg );
			pMsg = nullptr;
		}

	}
}
