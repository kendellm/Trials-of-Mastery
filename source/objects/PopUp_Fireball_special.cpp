#include "PopUp_Fireball_special.h"


#include "../Game.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../Camera.h"

#include "../Util/FontManager.h"


CPopUp_Fireball_special::CPopUp_Fireball_special( ): CPopUp(FIREBALL_SPECIAL)
{

	Timer(1.25);
	SetPosition(1400, 155);
	m_dwRenderColor = D3DCOLOR_ARGB(155, 255, 255, 255);

	m_nBackgroundImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/popup_special.png"));


	AnimationManager::GetInstance()->LoadAnimationFile("config/PopUps/popup_FireBall.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/PopUps/popup_player_fireball.xml");

	SetIdleAnim("Player_Idle");
	GetAnimInfo()->SetAnimationName("Player_Idle");

}


CPopUp_Fireball_special::~CPopUp_Fireball_special(void)
{
	 CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundImgid);
}

void CPopUp_Fireball_special::Update( float fElaspedTime )
{
	CPopUp::Update(fElaspedTime);
	GetAnimInfo()->QueueAnimation("popup_Player_Fireball");
	GetAnimInfo()->QueueAnimation("popup_FireBall");


}

void CPopUp_Fireball_special::Render()
{
	CCamera* Game_Camera = CCamera::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int x = int(GetPosX() - Game_Camera->GetPosX());
	int y = int(GetPosY() - Game_Camera->GetPosY());

	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgid, x, y);

	//CPopUp::Render();
	AnimationManager::GetInstance()->Render(  (float)(x + 75), (float)(y + 101), 
		CEntity::IsFlipped(), *GetAnimInfo(), m_dwRenderColor);

}