#include "Rock_Fall.h"

#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/RemoveEntityMessage.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"

#include "../TileSystem/TileManager.h"
#include "../objects/Enemy.h"
#include "../Camera.h"


CRock_Fall::CRock_Fall(void)
{
	CEntity::m_eType = ENT_ROCK_FALL;
	SetOwnerType(ENT_BASE);

	SetFlipped(true);
	SetSpeed(250.0f);

	AnimationManager::GetInstance()->LoadAnimationFile("config/Boulder_Trap.xml");
	SetBaseAnimations(ENT_ROCK_FALL);
	GetAnimInfo()->SetAnimationName("Boulder_Trap");
}


CRock_Fall::~CRock_Fall(void)
{
}

void CRock_Fall::Update(float fElapsedTime)
{
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	//CEntity::Update(fElapsedTime);
	SetPosX(GetPosX());
	SetPosY(GetPosY() + GetSpeed() * fElapsedTime);
	CCamera* Game_Camera = CCamera::GetInstance();
	if(this->GetPosY() > Game_Camera->GetPosY() + Game_Camera->GetHeight())
	{
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}

}
void CRock_Fall::Render(void)
{

	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), GetFlipped(), *GetAnimInfo());

#if HIT_BOXES
	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetActiveRect().GetRECT(),  D3DCOLOR_ARGB(255, 255, 0, 0), 1);
#endif
}

/*virtual*/ void CRock_Fall::HandleActiveRectCollision ( const IEntity* pOther )
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
	if(!found)
	{
		int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(this->GetAnimInfo())->GetDamage();
		if(pOther->GetType() == ENT_ENEMY)
		{
			const CEnemy* enemy = dynamic_cast<const CEnemy*>(pOther);

			if (enemy->GetEnemyType() != CEnemy::EarthBoss)
			{
				nDamage = nDamage/3;
				CSGD_EventSystem::GetInstance()->SendEventNow( "ModifyHealth" , &nDamage, (void*)pOther, (void*)this);
				GetHitEnemies().addHead(pOther);
			}
		}
		else
		{
			CSGD_EventSystem::GetInstance()->SendEventNow( "ModifyHealth" , &nDamage, (void*)pOther, (void*)this);
			GetHitEnemies().addHead(pOther);
		}
	}
}