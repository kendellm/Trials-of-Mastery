#include "Shockwave.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../Camera.h"

CShockwave::CShockwave(int lvl)
{
	SetLevel(lvl);
	CEntity::m_eType = ENT_SHOCKWAVE;
	AnimationManager::GetInstance()->LoadAnimationFile("config/shockwave_2.xml");
	//AnimationManager::GetInstance()->LoadAnimationFile("config/shockwave.xml");
	//AnimationManager::GetInstance()->LoadAnimationFile("config/shockwave2.xml");
	//AnimationManager::GetInstance()->LoadAnimationFile("config/shockwave3.xml");

	GetAnimInfo()->SetAnimationName("Shockwave_2");
	SetBaseAnimations(ENT_SHOCKWAVE);

	m_nCurrFrame = 0;
	SetSpeed(250.0f);
}

CShockwave::~CShockwave(void)
{
}

void CShockwave::Update( float fElapsedTime )
{
	CSpecial::Update(fElapsedTime);

	if(GetAnimInfo()->GetCurrFrame() != m_nCurrFrame)
	{
		GetHitEnemies().clear(); // I DEBUGGED FOR 2 HOURS LOOKING FOR THIS 
		m_nCurrFrame = GetAnimInfo()->GetCurrFrame();
	}

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
}

void CShockwave::Render( void )
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(),
		GetPosY() - Game_Camera->GetPosY(), CEntity::IsFlipped(), *GetAnimInfo());

}

/*virtual*/ void CShockwave::HandleActiveRectCollision ( const IEntity* pOther )
{		

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # 60 / 75
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	if(pOther->GetType() == ENT_PLAYER)
		return;

	bool found = false;
	SLLIter<const IEntity*> iter(m_lEnemiesHit);

	unsigned int size = m_lEnemiesHit.size();
	if (size != 0)
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

		// add a damage increase for higher level shockwave

		CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)pOther, this);
		// Add the player to the entity's enemies hit list
		m_lEnemiesHit.addHead(pOther);
		size = m_lEnemiesHit.size();
	}

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # 60 / 75
	//////////////////////////////////////////////////////////////////////////
}