/*
File:		Iceball.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CIceball creates an iceball for the ice boss
*/

#include "Iceball.h"
#include "../CSGD/CEvent.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/RemoveEntityMessage.h"

#include "../Messages/Create_Projectile_Message.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"


CIceball::CIceball(void)
{
	GetAnimInfo()->SetAnimationName("Ice_Ball_Spawn");
	CSGD_EventSystem::GetInstance()->RegisterClient("Create IceBall Projectile", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
}


CIceball::~CIceball(void)
{
}

/*virtual*/ void CIceball::Update(float fElapsedTime)
{
	//AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());

	CProjectile::Update( fElapsedTime );
}

/*virtual*/ void CIceball::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetDestination() == this)
	{
		if (pEvent->GetEventID() == "Create IceBall Projectile")
		{
			CCreate_Projectile_Message* pMsg = new CCreate_Projectile_Message(GetPosX(), GetPosY() + 27, "Ice_Ball", ENT_ENEMY, GetFlipped());
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}

		if (pEvent->GetEventID() == "Self Destruct")
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
}
