#include "PopUp_SPGain.h"


#include "../Game.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../Camera.h"

#include "../Util/FontManager.h"


CPopUp_SPGain::CPopUp_SPGain( ): CPopUp(SP_GAIN)
{

	Timer(1.25);
	SetPosition(960, 155);
	m_dwRenderColor = D3DCOLOR_ARGB(155, 255, 255, 255);
	AnimationManager::GetInstance()->LoadAnimationFile("config/PopUps/popup_SP_bar.xml");

	SetIdleAnim("Player_Idle");
	GetAnimInfo()->SetAnimationName("Player_Idle");

}


CPopUp_SPGain::~CPopUp_SPGain(void)
{
}

void CPopUp_SPGain::Update( float fElaspedTime )
{
	CPopUp::Update(fElaspedTime);
	GetAnimInfo()->QueueAnimation("popup_SP_bar");

}

void CPopUp_SPGain::Render()
{
	CCamera* Game_Camera = CCamera::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int x = int(GetPosX() - Game_Camera->GetPosX());
	int y = int(GetPosY() - Game_Camera->GetPosY());

	CPopUp::Render();
	AnimationManager::GetInstance()->Render(  (float)(x + 75), (float)(y + 101), 
		CEntity::IsFlipped(), *GetAnimInfo(), m_dwRenderColor);

	pFont->Draw(
				  CHINESE_TAKEAWAY, L"Attack the enemy \n to build your special meter",
				  x+48, y+44, 0.4f, 0.4f, BLACK
			   );

}


