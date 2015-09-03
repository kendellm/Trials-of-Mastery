#include "IceBoss.h"
#include "Player.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../objects/EntityManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../CSGD/CSGD_DirectInput.h"

#include "../Messages/Create_FireSpecial_Message.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../Messages/Create_IceSpecial_Message.h"
#include "../Messages/Create_IceBall_Message.h"
#include "../Messages/DropEXPMessage.h"


#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_Direct3D.h"

#include "../Camera.h"




#define EXP 500


CIceBoss::CIceBoss(void) : CBoss( 5.0f, 7.0f, false, 1, 80)  
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(IceBoss);

	CCamera* cam = CCamera::GetInstance();

	AnimationManager::GetInstance()->LoadAnimationFile("config/IceBoss_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/IceBoss_Special_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/IceBoss_Attack_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/IceBoss_Death_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/IceBoss_Flinch_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Ice_Ball_Animations.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/IceBoss_IceBall.xml");

	GetAnimInfo()->SetAnimationName("IceBoss_Walk_Animation");
	SetBaseAnimations(ENT_ENEMY);

	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Spawn IceBall", this);

	SetSpecialAnim("IceBoss_Special_Animation");


	//// IceBall Info ///////////////
	m_fIceBallOffsetX	= 2;
	m_fIceBallOffsetY	= 24;
	m_fIceBallSpawnPosX	= 0.0f;
	m_fIceBallSpawnPosY = 0.0f;
	/////////////////////////////


	/////// Frenzy Info //////////////
	ScreenLeft		= cam->GetPosX() + 64;
	ScreenRight		= cam->GetPosX() + 775;
	FrenzyYPos[0]	= 440;
	FrenzyYPos[1]	= 504;
	FrenzyYPos[2]	= 568;
	frenzy_timer	= 5.0f;
	fpc				= 0;
	stage			= 1;
	/////////////////////////////

	m_nExpPts = EXP;

	m_eCurrState = Normal;

	speed = 60;
	iceball_count = 0;

	SetUpWaypoints(cam);

}

CIceBoss::~CIceBoss(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CIceBoss::Update(float fElapsedTime)
{
	CEnemy::Update(fElapsedTime);

	if (m_pEState->GetState() == EntityState::DEAD)
	{
		return;
	}

	if( CSGD_DirectInput::GetInstance()->KeyPressed( DIK_F ) )
		GetAnimInfo()->SetAnimationName(GetAnimations("Flinch"));

	CalculateIceBallSpawn();

	switch (m_eCurrState)
	{
	case Normal:
		{
			frenzy_timer -= fElapsedTime;

			if (frenzy_timer < 0)
			{
				m_eCurrState = Frenzy;
				return;
			}

			CBoss::Update(fElapsedTime);

		}
		break;

	case Frenzy:
		{
			switch (stage)
			{
			case 1:
				{
					// Randomly teleport left or right
					if (rand () % 2 == 0)
					{
						SetPosX(ScreenRight);
						SetFlipped(false);
					}
					else
					{
						SetPosX(ScreenLeft);
						SetFlipped(true);
					}

					SetPosY((float)FrenzyYPos[fpc]);

					GetAnimInfo()->SetAnimationName("IceBoss_IceBall");

					fpc = rand() % 2 + 1;

					// Set Next stage to 2 
					stage = 2;
				}
				break;

			case 2:
				{
					if (GetAnimInfo()->GetAnimationName() != "IceBoss_Walk_Animation")
						return;

					//Move towards current waypoint
					if( FrenzyYPos[fpc] > GetPosY())
					{
						SetPosY(GetPosY() + 200 * fElapsedTime );
					}
					else if ( FrenzyYPos[fpc] < GetPosY() && abs(FrenzyYPos[fpc] - GetPosY()) > 10)
					{
						SetPosY(GetPosY() - 200 * fElapsedTime );						
					}
					else
					{				
						GetAnimInfo()->SetAnimationName("IceBoss_IceBall");

						if (iceball_count != 4)
							++iceball_count;

						if (iceball_count == 4)
							stage = 3;
						else
						{
							int old_fpc = fpc;
							while (fpc == old_fpc)
							{
								fpc = rand() % 3;
							}
						}

					}
				}
				break;

			case 3:
				{
					if (GetAnimInfo()->GetAnimationName() != "IceBoss_Walk_Animation")
						return;

					fpc = 0;
					stage = 1;
					m_eCurrState = Normal;
					frenzy_timer = 20.0f;
					iceball_count = 0;

				}
				break;

			}
		}
		break;

	}

}

/*virtual*/ RECT CIceBoss::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}
/*virtual*/ Rect CIceBoss::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CIceBoss::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetEventID() == "New_Player")
		SetTarget((CEntity*)(pEvent->GetParam()));

	if (pEvent->GetDestination() == this)
	{
		if (pEvent->GetEventID() == "ModifyHealth")
			ModifyHealth( *(int*)(pEvent->GetParam()) );

		if (pEvent->GetEventID() == "Set_State")
			m_pEState->SetState((CEntityState::ENTITY_STATE)(int)(pEvent->GetParam()) );

		if (pEvent->GetEventID() == "Dodge_Pitfall")
			HandleCollision((const IEntity*)pEvent->GetSender(), *(const RECT*)pEvent->GetParam());

		if (pEvent->GetEventID() == "Kill_Self")
		{
			CRemoveEntityMessage* pDestroyMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pDestroyMsg);
			pDestroyMsg = nullptr;
		}

		if (pEvent->GetEventID() == "Self Destruct")
		{
			CRemoveEntityMessage* pMsg = new CRemoveEntityMessage(this);
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;

			CDropEXPMessage* free_money = new CDropEXPMessage(m_nExpPts);
			CSGD_MessageSystem::GetInstance()->SendMsg(free_money);
			free_money = nullptr;
		}

		if (pEvent->GetEventID() == "Spawn IceBall")
		{
			CCreate_IceBall_Message* pMsg = new CCreate_IceBall_Message(
				m_fIceBallSpawnPosX, m_fIceBallSpawnPosY, this, IsFlipped());
			CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
}

void CIceBoss::CalculateIceBallSpawn(void)
{
	float camera_pos = CCamera::GetInstance()->GetPosX();
	float ice_boss_pos = GetPosX();

	if (IsFlipped() == true)
		m_fIceBallSpawnPosX = m_fIceBallOffsetX  + GetPosX() + 10;	
	else
		m_fIceBallSpawnPosX = m_fIceBallOffsetX  + GetPosX() - 75;

	m_fIceBallSpawnPosY = m_fIceBallOffsetY + CCamera::GetInstance()->GetPosY() + GetPosY() - 110;
}

void CIceBoss::SetUpWaypoints(CCamera* cam)
{
	//Setting up way points
	WayPoint temp;

	//Set up first waypoint
	temp.x = (cam->GetPosX() + 150);
	temp.y = 450;

	GetWayPoints().push_back( temp );

	//Set up second waypoint
	temp.x = (cam->GetPosX() + 350);
	temp.y = 375;

	GetWayPoints().push_back( temp );

	//Set up third waypoint
	temp.x = (cam->GetPosX() + 650);
	temp.y = 450;

	GetWayPoints().push_back( temp );

	// Frenzy GetWayPoints
	temp.x = 660;
	temp.y = 500;

	RFrenzyPos.push_back(temp);

	temp.y = 530;

	RFrenzyPos.push_back(temp);

	temp.y = 570;

	RFrenzyPos.push_back(temp);

	LFrenzyPos = RFrenzyPos;

	for (unsigned int i = 0; i < LFrenzyPos.size(); ++i)
	{
		LFrenzyPos[i].x = 64;
	}


}


