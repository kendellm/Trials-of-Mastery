#include "PopUp.h"

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


CPopUp::CPopUp(PopUp_Type nType)
{
	CEntity::m_eType = ENT_POPUP;
	m_nType = nType;
	m_fTimer = 0;
	SetFlipped(true);


	m_nBackgroundImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/scroll_XL.png"));


	CSGD_EventSystem::GetInstance()->RegisterClient("SELF_DESTRUCT", this);
}


CPopUp::~CPopUp(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CPopUp::Update( float fElapsedTime )
{
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	m_fTimer = max(m_fTimer - fElapsedTime, 0);
}

void CPopUp::Render( void )
{
	CCamera* Game_Camera = CCamera::GetInstance();

	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundImgid, (int)(GetPosX() - Game_Camera->GetPosX()),
		(int)(GetPosY() - Game_Camera->GetPosY()) );
}



void CPopUp::HandleEvent( CEvent* pEvent )
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
