#include "CWindBoss.h"
#include "Player.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../objects/EntityManager.h"
#include "../Messages/Create_Fan_Message.h"
#include "../Messages/Create_Wind_Tornado_Message.h"
#include "../CSGD/CSGD_DirectInput.h"

#include "../Messages/DropEXPMessage.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../Camera.h"


CWindBoss::CWindBoss(void) : CEnemy(100)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(WindBoss);
	m_nExpPts = 500;

	AnimationManager::GetInstance()->LoadAnimationFile("config/WindBoss_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/WindBoss_Special_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/WindBoss_Attack_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/WindBoss_Death_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/WindBoss_Flinch_Animation.xml");


	// Set the player's starting animation to "idle"

	GetAnimInfo()->SetAnimationName("WindBoss_Walk_Animation");
	SetBaseAnimations(ENT_ENEMY);

	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Fan Throw", this);


	m_nCoolDown			= 5.0f;

	m_fPunchCoolDown	= 2.5f;
	m_bPunching			= false;


	m_dwRenderColor		= 0xFFFFFFFF;
	m_bEvadeUp			= false;
	m_fPosXOld			= 0.0f;
	m_fMoveAway			= 0.0f;
	m_fPosYOld			= 0.0f;
	m_fUpdateOldPos		= 0.0f;
	speed = 100;

	//Setup default prev_target_Waypoint
	Prev_Tar_Pos.x = 0;
	Prev_Tar_Pos.y = 0;


	CCamera* cam = CCamera::GetInstance();
	//Setting up way points
	WayPoint temp;

	//Set up first waypoint
	temp.x = (cam->GetPosX() + 50);
	temp.y = 550;

	WayPoints.push_back( temp );

	//Set up second waypoint
	temp.x = (cam->GetPosX() + 350);
	temp.y = 375;

	WayPoints.push_back( temp );

	//Set up third waypoint
	temp.x = (cam->GetPosX() + 750);
	temp.y = 550;

	WayPoints.push_back( temp );

	//Set up way point timer
	WayPoint_Timer = 5.0f;

	inWaypoint = false;

	Waypoint_Counter = 1;

	//Setup special timer 
	Special_Timer = 7.0f;

	//Set up frenzy way points
	temp.x = (CCamera::GetInstance()->GetPosX() + 50);
	temp.y = 375;

	FrenzyPos.push_back( temp );

	temp.x = (CCamera::GetInstance()->GetPosX() + 750);
	temp.y = 425;

	FrenzyPos.push_back( temp );

	temp.x = (CCamera::GetInstance()->GetPosX() + 50);
	temp.y = 475;

	FrenzyPos.push_back( temp );

	temp.x = (CCamera::GetInstance()->GetPosX() + 750);
	temp.y = 550;

	FrenzyPos.push_back( temp );

	temp.x = (CCamera::GetInstance()->GetPosX() + 50);
	temp.y = 585;

	FrenzyPos.push_back( temp );

	FrenzyTimer = 10.0f;

	FrenzyCount = 0;

}
CWindBoss::~CWindBoss(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CWindBoss::Update(float fElapsedTime)
{
	CEnemy::Update(fElapsedTime);

	FrenzyTimer -= fElapsedTime;

	float tar_pos_x = GetTarget()->GetPosX();
	float tar_pos_y = GetTarget()->GetPosY();

	if( FrenzyTimer > 0.0f )
	{
		if (m_pEState->GetState() == EntityState::DEAD)
		{
			return;
		}

		CCamera* cam = CCamera::GetInstance();

		/*if( CSGD_DirectInput::GetInstance()->KeyPressed( DIK_F ) )
		GetAnimInfo()->SetAnimationName("WindBoss_Flinch_Animation");*/

		if( Special_Timer <= 3.0f && Special_Timer >= 2.0f )
		{
			Prev_Tar_Pos.x = GetTarget()->GetPosX();
			Prev_Tar_Pos.y = GetTarget()->GetPosY();
		}

		//Reset waypoint counter, timer, and bool if the enemy has been there long enough
		if( inWaypoint == true && WayPoint_Timer <= 0.0f )
		{
			Waypoint_Counter++;
			WayPoint_Timer = 5.0f;
			inWaypoint = false;
			m_dwRenderColor		= 0xFFFFFFFF;

		}
		else if( inWaypoint == true && WayPoint_Timer > 0.0f )
		{
			WayPoint_Timer -= fElapsedTime; 
			Special_Timer -= fElapsedTime;
			SetVelX(0.0f);
			SetVelY(0.0f);
		}

		if( Waypoint_Counter > 2 )
			Waypoint_Counter = 0;


		
		if(WayPoints[Waypoint_Counter].x > GetPosX() && inWaypoint == false)
		{
			//set Grunt's animation's facing to the right
			SetFlipped(true);
		}
		else
		{
			SetFlipped(false);
		}

		//Have him face the player once at his waypoint
		if(tar_pos_x > GetPosX() && inWaypoint == true)
		{
			//set Grunt's animation's facing to the right
			SetFlipped(true);
		}
		else if( tar_pos_x < GetPosX() && inWaypoint == true )
		{
			SetFlipped(false);
		}

		//Move enemy towards current waypoint
		if( inWaypoint == false )
		{
			if( WayPoints[Waypoint_Counter].x < GetPosX() )
				SetVelX( -speed * fElapsedTime );
			else
				SetVelX( speed * fElapsedTime );

			if( WayPoints[Waypoint_Counter].y < GetPosY() )
				SetVelY( -speed * fElapsedTime );
			else
				SetVelY( speed * fElapsedTime );
		}

		//Check to see if they are in the waypoint (or close enough)
		if( WayPoints[Waypoint_Counter].x - GetPosX() < 64 && WayPoints[Waypoint_Counter].x - GetPosX() > -64 )
			if( WayPoints[Waypoint_Counter].y - GetPosY() < 64 && WayPoints[Waypoint_Counter].y - GetPosY() > -64 )
				if( inWaypoint == false )
				{
					inWaypoint = true;
					WayPoint_Timer = 5.0f;
				}



				//if the player can be shot at.
				if( GetPosX() - tar_pos_x < 350 && GetPosX() - tar_pos_x > -350 )
				{
					if(GetPosY() - tar_pos_y < 150 && GetPosY() - tar_pos_y > -150 && m_bPunching == false)
					{
						if( inWaypoint == true  && Special_Timer <= 0.0f )
						{
							GetAnimInfo()->SetAnimationName("WindBoss_Special_Animation");

							//Color change for testing
							//	m_dwRenderColor		= 0xFF00FF00;




							//if in range rushing starts
							/*CCreate_Projectile_Message* c_pProjectMSG = new CCreate_Projectile_Message(ENT_ENEMY, IsFlipped());
							c_pProjectMSG->SetPosX(this->GetPosX());
							c_pProjectMSG->SetPosY(this->GetPosY());

							CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
							m_nCoolDown += 5;*/

							//Reset Special timer
							Special_Timer = 7.0f;
						}
					}
				}



				//Check to see if player is nearby inbetween waypoints to attack if necessary
				if( GetPosX() - tar_pos_x < 200 && GetPosX() - tar_pos_x > -200 )
					if(GetPosY() - tar_pos_y < 10 && GetPosY() - tar_pos_y > -10 )
					{
						if( m_fPunchCoolDown <= 0.0f )
						{

							//Have him face the player once at his waypoint
							if(tar_pos_x > GetPosX())
							{
								//set Grunt's animation's facing to the right
								SetFlipped(true);
							}
							else if( tar_pos_x < GetPosX())
							{
								SetFlipped(false);
							}


							m_fPunchCoolDown = 5.0f;

							GetAnimInfo()->SetAnimationName("WindBoss_Attack_Animation");
							CCreate_Wind_Tornado_Message* c_pProjectMSG = new CCreate_Wind_Tornado_Message(ENT_ENEMY, IsFlipped(), GetPosX(), GetPosY());


							CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);

						}
					}



					//Decrement punch timer always
					m_fPunchCoolDown -= fElapsedTime;

	}
	else
	{
		SetVelX(0.0f);
		SetVelY(0.0f);

		SetPosX(FrenzyPos[FrenzyCount].x);
		SetPosY(FrenzyPos[FrenzyCount].y);

		//Have him face the player once at his waypoint
		if(tar_pos_x > GetPosX())
		{
			//set Grunt's animation's facing to the right
			SetFlipped(true);
		}
		else if( tar_pos_x < GetPosX())
		{
			SetFlipped(false);
		}

		if( GetAnimInfo()->GetAnimationName() != "WindBoss_Special_Animation" )
			GetAnimInfo()->SetAnimationName("WindBoss_Special_Animation");


		
	}

	//Check Colider
	if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))
	{
		SetPosX(GetPosX() + GetVelX());
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
		{
			SetPosY(GetPosY() + GetVelY());
		}
	}
	else
	{
		if( GetPosY() >= 600 )
			SetPosY( (GetPosY() - (GetPosY() - 599)) );
		else
			SetPosY(GetPosY() + 1);
		SetVelX(0);
		SetVelY(0);
	}


}
void CWindBoss::Render(void)
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), m_dwRenderColor);

	CEnemy::Render();

}

/*virtual*/ RECT CWindBoss::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}
/*virtual*/ Rect CWindBoss::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CWindBoss::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetEventID() == "New_Player")
		SetTarget((CEntity*)(pEvent->GetParam()));
	if (pEvent->GetDestination() == this)
	{
		if (pEvent->GetEventID() == "ModifyHealth")
			ModifyHealth( *(int*)(pEvent->GetParam()) );
		if (pEvent->GetDestination() == this && pEvent->GetEventID() == "Set_State")
			m_pEState->SetState((CEntityState::ENTITY_STATE)(int)(pEvent->GetParam()) );
		if (pEvent->GetDestination() == this && pEvent->GetEventID() == "Dodge_Pitfall")
			HandleCollision((const IEntity*)pEvent->GetSender(), *(const RECT*)pEvent->GetParam());
		if (pEvent->GetDestination() == this && pEvent->GetEventID() == "Kill_Self")
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
		if (pEvent->GetEventID() == "Fan Throw")
		{
			CCreate_Fan_Message* c_pProjectMSG1 = new CCreate_Fan_Message(ENT_ENEMY, IsFlipped(), GetPosX(), GetPosY()-40);
			CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG1);

			if( FrenzyCount < 4 )
				FrenzyCount++;
			else 
			{
				FrenzyCount = 0;
				FrenzyTimer = 10.0f;
			}
		}
	}
}
