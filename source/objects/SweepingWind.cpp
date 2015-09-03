#include "SweepingWind.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"

#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/RemoveEntityMessage.h"

#include "../objects/Human.h"
#include "../objects/Enemy.h"
#include "../Camera.h"


CSweepingWind::CSweepingWind(void)
{
	CEntity::m_eType = ENT_SWEEPING_WIND;
	GetAnimInfo()->SetAnimationName("Wind_Special");

	SetSpeed(400.0f);
	Timer(1.7f);
}


CSweepingWind::~CSweepingWind(void)
{
	//UnitsHit.clear();
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);

}

void CSweepingWind::Update( float fElapsedTime )
{
	CSpecial::Update(fElapsedTime);

	if(IsFlipped())
		SetVelX(GetSpeed());
	else
		SetVelX(-GetSpeed());

	SetPosX(GetPosX() + GetVelX() * fElapsedTime);


	CCamera* Game_Camera = CCamera::GetInstance();
	if(this->GetPosX() < Game_Camera->GetPosX() || this->GetPosX() > Game_Camera->GetPosX() + Game_Camera->GetWidth())
	{		
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}

	if(CSpecial::Timer() == 0)
	{
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}
}

void CSweepingWind::Render( void )
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(),
		GetPosY() - Game_Camera->GetPosY(), CEntity::IsFlipped(), *GetAnimInfo());
}

/*virtual*/ void CSweepingWind::HandleActiveRectCollision ( const IEntity* pOther )
{
	if(pOther->GetType() != ENT_PLAYER)
	{
		if (pOther->GetType() == ENT_ENEMY)
		{
			const CEnemy* enemy = dynamic_cast<const CEnemy*>(pOther);
			if (enemy->GetEnemyType() != CEnemy::EarthBoss	&&
				enemy->GetEnemyType() != CEnemy::FireBoss	&& 
				enemy->GetEnemyType() != CEnemy::IceBoss	&& 
				enemy->GetEnemyType() != CEnemy::WindBoss 
				)
			CSGD_EventSystem::GetInstance()->SendEvent( "Set_State" , (void*)CEntityState::PUSHED, (void*)pOther);
		}
	}
}
