#include "PopUp_RetreatingKick.h"

#include "../Game.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../Camera.h"

#include "../Util/FontManager.h"


CPopUp_RetreatingKick::CPopUp_RetreatingKick( ): CPopUp(DASHING_PUNCH)
{

	Timer(1.25);
	SetPosition(227, 155);
	m_dwRenderColor = D3DCOLOR_ARGB(155, 255, 255, 255);
	AnimationManager::GetInstance()->LoadAnimationFile("config/PopUps/popup_heavy_kick.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/PopUps/popup_medium_kick.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/PopUps/popup_retreating_kick.xml");

	SetIdleAnim("Player_Idle");
	GetAnimInfo()->SetAnimationName("Player_Idle");

	keys[0]  = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/nkey1.png"));

	keys[1]  = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/nkey2.png"));

	keys[2]  = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/nkey3.png"));

}


CPopUp_RetreatingKick::~CPopUp_RetreatingKick(void)
{
}

void CPopUp_RetreatingKick::Update( float fElaspedTime )
{
	CPopUp::Update(fElaspedTime);
	GetAnimInfo()->QueueAnimation("popup_Heavy_Kick");
	GetAnimInfo()->QueueAnimation("popup_Medium_Kick");
	GetAnimInfo()->QueueAnimation("popup_Retreating_Kick");
}

void CPopUp_RetreatingKick::Render()
{
	CCamera* Game_Camera = CCamera::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int x = (int)(GetPosX() - Game_Camera->GetPosX() );
	int y = (int)(GetPosY() - Game_Camera->GetPosY() );

	CPopUp::Render();
	AnimationManager::GetInstance()->Render(  (float)(x + 75), (float)(y + 123), 
		CEntity::IsFlipped(), *GetAnimInfo(), m_dwRenderColor);

	pFont->Draw(
		CHINESE_TAKEAWAY, L"Perform a Retreating\n  Kick with",
		(int)((GetPosX()+153) - Game_Camera->GetPosX()),
		(int)((GetPosY()+37)  - Game_Camera->GetPosY()),
		0.4f, 0.4f, BLACK
		);

	pTM->Draw( keys[2] , x + 250, y + 84 );
	pTM->Draw( keys[1] , x + 210, y + 84 );
	pTM->Draw( keys[0] , x + 170, y + 84 );

}

