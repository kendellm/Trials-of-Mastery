#include "Wrecking_Ball.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"

#include "../Camera.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../TileSystem/TileManager.h"

CWrecking_Ball::CWrecking_Ball(void)
{
	CEntity::m_eType = ENT_WRECKING_BALL;
	SetOwnerType(ENT_BASE);

	SetFlipped(true);
	SetSpeed(250.0f);

	AnimationManager::GetInstance()->LoadAnimationFile("config/Wrecking_Ball.xml");
	SetBaseAnimations(ENT_WRECKING_BALL);
	GetAnimInfo()->SetAnimationName("Wrecking_Ball");

}
CWrecking_Ball::~CWrecking_Ball(void)
{
}

void CWrecking_Ball::Update(float fElapsedTime)
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
void CWrecking_Ball::Render(void)
{
	
	CCamera* Game_Camera = CCamera::GetInstance();
	int temp0 = (int)(Game_Camera->GetPosX());
	int temp2 = (int)(GetPosX() - Game_Camera->GetPosX());
	int temp3 = (int)GetPosY();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), GetFlipped(), *GetAnimInfo());
	
#if HIT_BOXES
	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetActiveRect().GetRECT(),  D3DCOLOR_ARGB(255, 255, 0, 0), 1);
#endif
}

/*virtual*/ void CWrecking_Ball::HandleActiveRectCollision ( const IEntity* pOther )
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
		CSGD_EventSystem::GetInstance()->SendEventNow("Set_State", (void*)2, (void*)pOther, this);
		GetHitEnemies().addHead(pOther);
	}
}