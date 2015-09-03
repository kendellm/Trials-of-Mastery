#include "Special.h"


#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../Util/Rect.h"
#include "../Util/D3DCOLORS.h"


#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"


CSpecial::CSpecial(void)
{
	CEntity::m_eType = ENT_SPECIAL;
	Owner_Type = ENT_PLAYER;
	
	m_fTimer = 0;
	m_nSpeed = 0;

	m_szLv1Anim		    = nullptr;
	m_szLv2Anim	        = nullptr;
	m_szLv3Anim	        = nullptr;

	CSGD_EventSystem::GetInstance()->RegisterClient("SELF_DESTRUCT", this);
}


CSpecial::~CSpecial(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CSpecial::Update( float fElapsedTime )
{
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	m_fTimer = max(m_fTimer - fElapsedTime, 0);
}

void CSpecial::Render( void )
{

#if HIT_BOXES
	CSGD_Direct3D::GetInstance()->DrawHollowRect(
		GetActiveRect().GetRECT(), RED, 1);
#endif

}

RECT CSpecial::GetCollisionRect( void ) const 
{
	return CEntity::GetCollisionRect();

}

Rect CSpecial::GetActiveRect( void ) const 
{
	return CEntity::GetActiveRect();

}

void CSpecial::HandleEvent( CEvent* pEvent )
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

/*virtual*/ void CSpecial::SetBaseAnimations( EntityType eType)
{
	switch(eType)
	{
	case ENT_SUBZERO:
		{
			SetIdleAnim("subzero");
			m_szLv1Anim			= "subzero";
			m_szLv2Anim		    = "subzero2";
			m_szLv3Anim	        = nullptr;
		}
		break;

	case ENT_SHOCKWAVE:
		{
			SetIdleAnim("Shockwave_2");
		}
		break;

	case ENT_SWEEPING_WIND:
		{
			SetIdleAnim("sweeping_wind");
		}
		break;

	default:
		break;
	}
}


