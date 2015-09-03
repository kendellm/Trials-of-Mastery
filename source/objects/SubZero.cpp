#include "SubZero.h"


#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "EntityState.h"
#include "../Camera.h"


CSubZero::CSubZero(int lvl)
{
	SetLevel(lvl);
	CEntity::m_eType = ENT_SUBZERO;
	//AnimationManager::GetInstance()->LoadAnimationFile("config/subzero.xml");
	//AnimationManager::GetInstance()->LoadAnimationFile("config/subzero2.xml");
	//AnimationManager::GetInstance()->LoadAnimationFile("config/subzero3.xml");

	switch(GetLevel())
	{
	case 1: GetAnimInfo()->SetAnimationName("Player_Subzero_lvl1");
		Timer(2.0f);
		break;
	case 2: GetAnimInfo()->SetAnimationName("Player_Subzero_lvl2");
		Timer(2.0f);
		break;
	case 3: GetAnimInfo()->SetAnimationName("Player_Subzero_lvl3");
		Timer(1.2f);
		break;
	}

	SetBaseAnimations(ENT_SUBZERO);
}

CSubZero::~CSubZero(void)
{

}

void CSubZero::Update( float fElapsedTime )
{
	CSpecial::Update(fElapsedTime);

	if(CSpecial::Timer() == 0)
	{
		CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}
}

void CSubZero::Render( void )
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(),
		GetPosY() - Game_Camera->GetPosY(), CEntity::IsFlipped(), *GetAnimInfo());
}
/*virtual*/ void CSubZero::HandleActiveRectCollision ( const IEntity* pOther )
{
	if(this->GetOwnerType() != pOther->GetType())
	{
		CSGD_EventSystem::GetInstance()->SendEvent( "SELF_DESTRUCT" , (void*)0, (void*)pOther);
		CSGD_EventSystem::GetInstance()->SendEvent( "Set_State" , (void*)CEntityState::FROZEN, (void*)pOther);
	}
}