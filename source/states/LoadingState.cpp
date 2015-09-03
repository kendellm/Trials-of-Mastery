#include "LoadingState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../Inputs/Controller.h"

#include "../Game.h"
#include "StateManager.h"
#include "MainMenuState.h"
#include "OptionState.h"
#include "HomeMenuState.h"
#include "../../resource.h"


CLoadingState* CLoadingState::GetInstance( void )
{
	static CLoadingState s_Instance;
	return &s_Instance;
}


CLoadingState::CLoadingState(void)
{
	m_nMenuArtid = -1;
}


CLoadingState::~CLoadingState(void)
{
}

void CLoadingState::Enter( void )
{
	Active(true);

	m_fTimer = 1.0f;
	
	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_main.png"));
}

void CLoadingState::Exit( void )
{

}

bool CLoadingState::Input( void )
{
	CController* pCtrl = CController::GetInstance();

	if( m_fTimer <= 0)
	{
		CGame::GetInstance()->GetState()->ChangeState(m_pState);
		return true;
	}
	
	return true;
}

void CLoadingState::Update( float fElaspedTime )
{
	m_fPrevTime = m_fTimer;
	m_fTimer = max( m_fTimer - fElaspedTime, 0 );
	
}

void CLoadingState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pD3D->Clear(WHITE);
	pTM->Draw(m_nMenuArtid,0,0);
	
}