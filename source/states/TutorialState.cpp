
#include "TutorialState.h"


#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_DirectInput.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../Game.h"
#include "StateManager.h"
#include "MainMenuState.h"

CTutorialState* CTutorialState::GetInstance( void )
{
	static CTutorialState s_Instance;
	return &s_Instance;
}


CTutorialState::CTutorialState(void)
{
	m_aInstruction[0] = -1;
	m_aInstruction[1] = -1;
}


CTutorialState::~CTutorialState(void)
{
	
}

void CTutorialState::Enter( void )
{
	Active(true);

	m_aInstruction[0] = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/combos.png"));
	m_aInstruction[1] = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/controls_kb.png"));
	m_aInstruction[2] = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/controls_360.png"));
	m_aInstruction[3] = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/controls_js.png"));
	m_aInstruction[4] = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/PopUps/SHOP.png"));
	m_aInstruction[5] = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/PopUps/BUY_SKILL.png"));
}

void CTutorialState::Exit( void )
{

	
	for(int i = 5; i >-1; --i)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_aInstruction[i]);
		m_aInstruction[i] = -1;
	}
}

bool CTutorialState::Input( void )
{
	CController* pCtrl = CController::GetInstance();


	if( pCtrl->Cancel() ||
		pCtrl->Confirm()
	  )
	{
		CAudioManager::GetInstance()->Play(SFX::CURSOR);

		if(m_nCurrInstrut ==1 || m_nCurrInstrut == 2)
			++m_nCurrInstrut;
		else
			CGame::GetInstance()->GetState()->RemoveState( this );
		return true;
	}

	return true;
}

void CTutorialState::Update( float fElaspedTime )
{

}

void CTutorialState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	//pD3D->Clear(D3DCOLOR_ARGB(255, 0, 0, 0));

	if(m_nCurrInstrut == 0)
	{
		RECT rBox = {0,0, 529, 333};
		CSGD_TextureManager::GetInstance()->Draw(m_aInstruction[m_nCurrInstrut],
		150,150,1.0f,1.0f, &rBox);
	}
	else if( m_nCurrInstrut == 1)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_aInstruction[m_nCurrInstrut],
			0,0);
	}
	else
	  CSGD_TextureManager::GetInstance()->Draw(m_aInstruction[m_nCurrInstrut],
			 0,0);
}

void CTutorialState::SetInstruction( int nType )
{
	m_nCurrInstrut = nType;
}
