#include "Billboard.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_TextureManager.h"

#include "../CSGD/RemoveEntityMessage.h"
#include "../Util/Rect.h"
#include "../Util/D3DCOLORS.h"

#include "../Camera.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"


CBillboard::CBillboard(BBType nType)
{
	CEntity::m_eType = ENT_BILLBOARD;
	m_nType = nType;
	m_fTimer = 0;



	m_nBackgroundImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/billboard.png"));




	CSGD_EventSystem::GetInstance()->RegisterClient("SELF_DESTRUCT", this);
}


CBillboard::~CBillboard(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CBillboard::Update( float fElapsedTime )
{
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	m_fTimer = max(m_fTimer - fElapsedTime, 0);
}

void CBillboard::Render( void )
{
	CCamera* Game_Camera = CCamera::GetInstance();

	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgid, GetPosX() - Game_Camera->GetPosX(),
								GetPosY() - Game_Camera->GetPosY() );
}



void CBillboard::HandleEvent( CEvent* pEvent )
{
	if (pEvent->GetEventID() == "SELF_DESTRUCT")
	{
		if (pEvent->GetDestination() == this)
		{				
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
}



