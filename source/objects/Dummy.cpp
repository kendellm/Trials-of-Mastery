#include "Dummy.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../objects/EntityManager.h"
#include "../objects/Fireball.h"

#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/Toggle_Camera_Message.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"


CDummy::CDummy(void) : CEnemy()
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(Dummy);
	GetAnimInfo()->SetAnimationName("MVC_TEST");

	m_nHealth = 20;

	m_bFlipped = false;

	CSGD_EventSystem::GetInstance()->ClearEvents();
	CSGD_EventSystem::GetInstance()->RegisterClient("Pop_Up_Completed", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
}
CDummy::~CDummy(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CDummy::Update(float fElapsedTime)
{
	if (m_nHealth <= 0)
	{
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}

	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	
	if(GetTarget() != nullptr)
	{
		float tar_pos_x = GetTarget()->GetPosX() - 32;
		float tar_pos_y = GetTarget()->GetPosY();
		if(tar_pos_x > GetPosX())
		{
			//set Grunt's animation's facing to the right
			m_bFlipped = false;
		}
		else
		{
			m_bFlipped = true;
		}
	
		
		////Check Colider
		//if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))
		//{
		//	SetPosX(GetPosX() + GetVelX());
		//	if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
		//	{
		//		SetPosY(GetPosY() + GetVelY());
		//	}
		//}
		//else
		//{
		//	SetVelX(0);
		//	SetVelY(0);
		//}
		//if(GetPosX() < 0)
		//	SetPosX(0);
		//if(GetPosX() - tar_pos_x < 32 && GetPosX() - tar_pos_x > -32 )
		//{
		//	if(GetPosY() - tar_pos_y < 32 && GetPosY() - tar_pos_y > -32)
		//	{
		//		GetAnimInfo()->SetAnimationName("LightPunch");
		//	}
		//}
	
	}
}

void CDummy::Render(void)
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), m_bFlipped, *GetAnimInfo());
}

/*virtual*/ void CDummy::ModifyHealth(int damage)
{
	m_nHealth -= damage;
}
/*virtual*/ RECT CDummy::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}

/*virtual*/ Rect CDummy::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}
/*virtual*/ void CDummy::HandleActiveRectCollision ( const IEntity* pOther )
{
}

/*virtual*/ void CDummy::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetEventID() == "Pop_Up_Completed")
	{
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;	
		CToggle_Camera_Message * temp = new CToggle_Camera_Message(false);
		CSGD_MessageSystem::GetInstance()->SendMsg(temp);
	}
	if(pEvent->GetDestination() == this && pEvent->GetEventID() == "ModifyHealth" )
	{
		if( ((IEntity*)(pEvent->GetSender()))->GetType() == ENT_FIREBALL)
		{
			CSGD_EventSystem::GetInstance()->SendEventNow("Hit_By_Fireball");
		}
	}
}