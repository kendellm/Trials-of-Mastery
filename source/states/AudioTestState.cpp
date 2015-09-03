#include "AudioTestState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_DirectInput.h"

#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"


#include "../Game.h"
#include "StateManager.h"
#include "MainMenuState.h"
#include "OptionState.h"
#include "HomeMenuState.h"
#include "../../resource.h"


CAudioTestState* CAudioTestState::GetInstance( void )
{
	static CAudioTestState s_Instance;
	return &s_Instance;
}


CAudioTestState::CAudioTestState(void)
{
}


CAudioTestState::~CAudioTestState(void)
{
}

void CAudioTestState::Enter( void )
{
	Active(true);
	m_pAUM = CAudioManager::GetInstance();
	m_pAUM->Play(MAIN);

}

void CAudioTestState::Exit( void )
{
	//m_pAUM->Stop(BGM::CREDITS);
	m_pAUM->Stop();
}

bool CAudioTestState::Input( void )
{
	CController* pCtrl = CController::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( pCtrl->Cancel() )
	{
		CGame::GetInstance()->GetState()->ChangeState( CMainMenuState::GetInstance());
		return true;
	}

	if( pDI->KeyDown( DIK_LALT) && 
		pDI->KeyPressed( DIK_1) )
	{
		m_pAUM->Play(CURSOR);
	}

	return true;
}

void CAudioTestState::Update( float fElaspedTime )
{

}

void CAudioTestState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pD3D->Clear(BLUE);




}