#include "ComingSoonState.h"


#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../Inputs/Controller.h"

#include "../Game.h"
#include "StateManager.h"
#include "MainMenuState.h"
#include "OptionState.h"
#include "HomeMenuState.h"
#include "../../resource.h"


CComingSoonState* CComingSoonState::GetInstance( void )
{
	static CComingSoonState s_Instance;
	return &s_Instance;
}


CComingSoonState::CComingSoonState(void)
{
}


CComingSoonState::~CComingSoonState(void)
{
}

void CComingSoonState::Enter( void )
{
	Active(true);

}

void CComingSoonState::Exit( void )
{

}

bool CComingSoonState::Input( void )
{
	CController* pCtrl = CController::GetInstance();

	if( pCtrl->Cancel() || pCtrl->Confirm() )
	{
			CGame::GetInstance()->GetState()->RemoveState(this);
			return true;
	}

	return true;
}

void CComingSoonState::Update( float fElaspedTime )
{

}

void CComingSoonState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();


	RECT box = {330, 240, 530, 300};
	pD3D->DrawRect(box,  BLACK );

	pFont->Draw( ARIAL , GAME_COMINGSOON,
		350, 250,1.0f,1.0f, WHITE);

}