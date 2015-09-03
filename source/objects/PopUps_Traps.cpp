#include "PopUps_Traps.h"


#include "../Game.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../Camera.h"

#include "../Util/FontManager.h"


CPopUps_Traps::CPopUps_Traps( ): CPopUp(SP_GAIN)
{

	Timer(1.25);
	SetPosition(960, 155);
	m_dwRenderColor = D3DCOLOR_ARGB(155, 255, 255, 255);


	m_nBGImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/scroll_traps.png"));
}


CPopUps_Traps::~CPopUps_Traps(void)
{
}

void CPopUps_Traps::Update( float fElaspedTime )
{
	CPopUp::Update(fElaspedTime);


}

void CPopUps_Traps::Render()
{
	CCamera* Game_Camera = CCamera::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	int x = int(GetPosX() - Game_Camera->GetPosX());
	int y = int(GetPosY() - Game_Camera->GetPosY());

	CSGD_TextureManager::GetInstance()->Draw(m_nBGImgid,x, y );

	pFont->Draw(
		CHINESE_TAKEAWAY, L"Hazard! \n Traps ahead",
		x+48, y+44, 0.4f, 0.4f, BLACK
		);

}