#include "Fireball.h"

#include "../TileSystem/TileManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"

#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/RemoveEntityMessage.h"

#include "../Inputs/AudioManager.h"

#include "../Camera.h"

CFireball::CFireball(int lvl)
{
	SetLevel(lvl);
	CEntity::m_eType = ENT_FIREBALL;
	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBall.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/fireball2.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/fireball3.xml");

	GetAnimInfo()->SetAnimationName("FireBall");
	SetBaseAnimations(ENT_FIREBALL);
	Timer(0.85f);

	switch(GetLevel())
	{
	case 1: GetAnimInfo()->SetAnimationName("FireBall" );
		Timer(0.85f);
		break;
	case 2: GetAnimInfo()->SetAnimationName("FireBall2");
		Timer();
		break;
	case 3: GetAnimInfo()->SetAnimationName("FireBall3");
		Timer(1.25f);
		break;
	}

	SetSpeed(250.f);
	SetFlipped(true);
}

CFireball::~CFireball(void)
{
}

void CFireball::Update( float fElapsedTime )
{
	CSpecial::Update(fElapsedTime);


	if(IsFlipped())
		SetVelX(GetSpeed());
	else
		SetVelX(-GetSpeed());
	//CEntity::Update(fElapsedTime);
	if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))//can move x?
	{
		SetPosX(GetPosX() + GetVelX() * fElapsedTime);
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY())) // can move y?
		{
			SetPosY(GetPosY() + GetVelY() * fElapsedTime);
		}
		else//Remove self if hits a collider
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
	else//Remove self if hits a collider
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

void CFireball::Render( void )
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(),
		GetPosY() - Game_Camera->GetPosY(), CEntity::IsFlipped(), *GetAnimInfo());

}
/*virtual*/ void CFireball::HandleActiveRectCollision ( const IEntity* pOther )
{
	if(this->GetOwnerType() != pOther->GetType())
	{
		int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(this->GetAnimInfo())->GetDamage();
		CSGD_EventSystem::GetInstance()->SendEventNow( "ModifyHealth" , &nDamage, (void*)pOther, (void*)this);
		CSGD_EventSystem::GetInstance()->SendEventNow( "SELF_DESTRUCT" , (void*)0, (void*)this);
		CAudioManager::GetInstance()->Play(FIREBALL_IMPACT);
	}
}