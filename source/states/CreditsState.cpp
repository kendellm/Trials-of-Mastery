/*************************************************************
	File:			CreditState.cpp
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Renders the credits animation
*************************************************************/
#include "CreditsState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_DirectInput.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../Inputs/Controller.h"

#include "../Game.h"
#include "StateManager.h"
#include "MainMenuState.h"
#include "../Inputs/AudioManager.h"

CCreditsState* CCreditsState::GetInstance( void )
{
	static CCreditsState s_Instance;
	return &s_Instance;
}


CCreditsState::CCreditsState(void)
{
	m_nBackgroundImgID = -1;
}


CCreditsState::~CCreditsState(void)
{
  
}

void CCreditsState::Enter( void )
{
	Active(true);

	m_nBackgroundImgID = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/credits.png"));

	CAudioManager::GetInstance()->Play(BGM::CREDITS);
}

void CCreditsState::Exit( void )
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundImgID);
	m_nBackgroundImgID = -1;
}

bool CCreditsState::Input( void )
{
	CController* pCtrl = CController::GetInstance();


	if( pCtrl->Cancel() ||
		pCtrl->Confirm()
		)
	{
		 CGame::GetInstance()->GetState()->ChangeState( CMainMenuState::GetInstance() );
		return true;
	}

	return true;
}

void CCreditsState::Update( float fElaspedTime )
{

}

void CCreditsState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	pD3D->Clear(D3DCOLOR_ARGB(255, 0, 0, 0));

	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgID, 0,0);
}
