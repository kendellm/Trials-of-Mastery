#include "Projectile.h"

#include "../TileSystem/TileManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/RemoveEntityMessage.h"

#include "../Inputs/AudioManager.h"

#include "../Camera.h"




CProjectile::CProjectile(void)
{
	CEntity::m_eType = ENT_PROJECTILE;
	Owner_Type = ENT_PLAYER;

	m_bFlipped = true;
	m_nSpeed = 250.0f;

	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBall.xml");


	GetAnimInfo()->SetAnimationName("FireBall");

	CSGD_EventSystem::GetInstance()->RegisterClient("SELF_DESTRUCT", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);

	isSpecial = false;


}

CProjectile::CProjectile(string szName, float fSpeed, bool bFlipped)
{
	CEntity::m_eType = ENT_PROJECTILE;
	Owner_Type = ENT_ENEMY;

	m_bFlipped = bFlipped;
	m_nSpeed = fSpeed;

	GetAnimInfo()->SetAnimationName(szName);

	CSGD_EventSystem::GetInstance()->RegisterClient("SELF_DESTRUCT", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);

	isSpecial = false;
}



CProjectile::CProjectile(string anim, string name, float speed, bool s)
{
	CEntity::m_eType = ENT_PROJECTILE;
	Owner_Type = ENT_ENEMY;

	m_nSpeed = speed;

	AnimationManager::GetInstance()->LoadAnimationFile(anim.c_str());


	GetAnimInfo()->SetAnimationName(name);

	CSGD_EventSystem::GetInstance()->RegisterClient("SELF_DESTRUCT", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
	m_bFlipped = true;

	isSpecial = s;
}


CProjectile::~CProjectile(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}
void CProjectile::Update(float fElapsedTime)
{
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	if(m_bFlipped)
		SetVelX(m_nSpeed);
	else
		SetVelX(-m_nSpeed);
	//CEntity::Update(fElapsedTime);
	if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))//can move x?
	{
		SetPosX(GetPosX() + GetVelX() * fElapsedTime);
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY())) // can move y?
		{
			SetPosY(GetPosY() + GetVelY() * fElapsedTime);
		}
		else//Remove self if hits a colider
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
	else//Remove self if hits a colider
	{
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}


	CCamera* Game_Camera = CCamera::GetInstance();
	if(this->GetPosX() < Game_Camera->GetPosX() || this->GetPosX() > Game_Camera->GetPosX() + Game_Camera->GetWidth())
	{		
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}

}
void CProjectile::Render(void)
{

	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), m_bFlipped, *GetAnimInfo());

#if HIT_BOXES
	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetActiveRect().GetRECT(),  D3DCOLOR_ARGB(255, 255, 0, 0), 1);
#endif
}

RECT CProjectile::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}
Rect CProjectile::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

void CProjectile::HandleEvent(CEvent* pEvent)
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

	if (pEvent->GetEventID() == "Self Destruct")
	{
		if (pEvent->GetDestination() == this)
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
}

/*virtual*/ void CProjectile::HandleActiveRectCollision ( const IEntity* pOther )
{
	if(this->GetOwnerType() != pOther->GetType())
	{
		
		bool found = false;
		SLLIter<const IEntity*> iter(GetHitEnemies());

		if (GetHitEnemies().size() != 0)
		{
			for (iter.begin(); !iter.end(); ++iter)
			{
				if (iter.current() == pOther)
				{
					found = true;
					break;
				}
			}
		}
		if (found == false)
		{
			int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(GetAnimInfo())->GetDamage();
			CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)pOther, this);
			// Add the player to the entity's enemies hit list
			GetHitEnemies().addHead(pOther);
			CAudioManager::GetInstance()->Play(FIREBALL_IMPACT);
			if( isSpecial == false )
				CSGD_EventSystem::GetInstance()->SendEventNow( "SELF_DESTRUCT" , (void*)0, (void*)this);	
		}
	//	int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(this->GetAnimInfo())->GetDamage();
	//	CSGD_EventSystem::GetInstance()->SendEventNow( "ModifyHealth" , &nDamage, (void*)pOther, (void*)this);
	//	CSGD_EventSystem::GetInstance()->SendEventNow( "SELF_DESTRUCT" , (void*)0, (void*)this);
	//	CAudioManager::GetInstance()->Play(FIREBALL_IMPACT);
	}
}