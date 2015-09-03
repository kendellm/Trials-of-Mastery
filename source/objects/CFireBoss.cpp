#include "CFireBoss.h"
#include "Player.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../objects/EntityManager.h"
#include "../CSGD/CSGD_DirectInput.h"

#include "../Messages/Create_FireSpecial_Message.h"
#include "../Messages/Create_FireBoss_FireBall_Message.h"
#include "../Messages/DropEXPMessage.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../Camera.h"


CFireBoss::CFireBoss(void) : CEnemy(150)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(FireBoss);
	m_nExpPts = 500;

	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBoss_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBoss_Special_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBoss_Attack_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBoss_Death_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/FireBoss_Flinch_Animation.xml");

	// Set the player's starting animation to "idle"

	GetAnimInfo()->SetAnimationName("FireBoss_Walk_Animation");
	SetBaseAnimations(ENT_ENEMY);

	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Fire Special", this);



	m_nCoolDown			= 5.0f;

	m_fPunchCoolDown	= 2.5f;
	m_bPunching			= false;


	m_dwRenderColor		= 0xFFFFFFFF;
	m_bEvadeUp			= false;
	m_fPosXOld			= 0.0f;
	m_fMoveAway			= 0.0f;
	m_fPosYOld			= 0.0f;
	m_fUpdateOldPos		= 0.0f;
	speed = 75;

	//Setup default prev_target_Waypoint
	Prev_Tar_Pos.x = 0;
	Prev_Tar_Pos.y = 0;


	CCamera* cam = CCamera::GetInstance();
	//Setting up way points
	WayPoint temp;

	//Set up first waypoint
	temp.x = (cam->GetPosX() + 50);
	temp.y = 450;

	WayPoints.push_back( temp );

	//Set up second waypoint
	temp.x = (cam->GetPosX() + 450);
	temp.y = 375;

	WayPoints.push_back( temp );

	//Set up third waypoint
	temp.x = (cam->GetPosX() + 750);
	temp.y = 450;

	WayPoints.push_back( temp );

	//Set up third waypoint
	temp.x = (cam->GetPosX() + 450);
	temp.y = 550;

	WayPoints.push_back( temp );

	//Set up way point timer
	WayPoint_Timer = 5.0f;

	inWaypoint = false;

	Waypoint_Counter = 1;

	//Setup special timer 
	Special_Timer = 7.0f;


	//Waypoint to check against targets previous position
	Cur_Tar_Pos.x = 0;
	Cur_Tar_Pos.y = 0;

	//Timer to see how long target is in one spot
	Tar_Pos_Timer = 2.5f;

	StartHealth = GetHealth();

}
CFireBoss::~CFireBoss(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CFireBoss::Update(float fElapsedTime)
{
	CEnemy::Update(fElapsedTime);

	if (m_pEState->GetState() == EntityState::DEAD)
	{
		return;
	}

//	if( CSGD_DirectInput::GetInstance()->KeyPressed( DIK_F ) )
	//	GetAnimInfo()->SetAnimationName("FireBoss_Flinch_Animation");



	CCamera* cam = CCamera::GetInstance();


	if( Special_Timer <= 0.5f && Special_Timer >= 0.0f )
	{
		Prev_Tar_Pos.x = GetTarget()->GetPosX();
		Prev_Tar_Pos.y = GetTarget()->GetPosY();
	}

	if( Special_Timer <= 0.5f )
		m_dwRenderColor = 0xFFAA0000;

	if( Tar_Pos_Timer <= 0.0f )
		if( Cur_Tar_Pos.x < (Prev_Tar_Pos.x + 5) && Cur_Tar_Pos.x > (Prev_Tar_Pos.x - 5) )
		{
			if(  Cur_Tar_Pos.y < (Prev_Tar_Pos.y + 5) && Cur_Tar_Pos.y > (Prev_Tar_Pos.y - 5) )
			{
				Special_Timer -= 1.0f;
				Tar_Pos_Timer = 2.5f;
			}
		}
		else
			Tar_Pos_Timer = 2.5f;


	


	Tar_Pos_Timer -= fElapsedTime;

	if( Tar_Pos_Timer <= 2.0f )
	{
		Prev_Tar_Pos.x = GetTarget()->GetPosX();
		Prev_Tar_Pos.y = GetTarget()->GetPosY();
	}
	else
	{
		Cur_Tar_Pos.x = GetTarget()->GetPosX();
		Cur_Tar_Pos.y = GetTarget()->GetPosY();
	}


	//Increase speed if the enemies health is low
	if( GetHealth() < (0.4f * StartHealth) )
		speed = 95;

	//Increment waypoint counter, timer, and bool if the enemy has been there long enough
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


	//Reset way point vector  
	if( Waypoint_Counter > 3 )
		Waypoint_Counter = 0;


	float tar_pos_x = GetTarget()->GetPosX();
	float tar_pos_y = GetTarget()->GetPosY();

	if( GetHealth() > StartHealth * 0.4f )
	{
		if(WayPoints[Waypoint_Counter].x > GetPosX() && inWaypoint == false)
		{
			SetFlipped(true);
		}
		else
		{
			SetFlipped(false);
		}
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


	if( GetHealth() < (StartHealth * 0.4) )
		inWaypoint = false;

	//Move enemy towards current waypoint
	if ( inWaypoint == false && GetHealth() < (StartHealth * 0.4f) )
	{
		if( tar_pos_x < GetPosX() )
			SetVelX( -speed * fElapsedTime );
		else
			SetVelX( speed * fElapsedTime );

		if( tar_pos_y < GetPosY() )
			SetVelY( -speed * fElapsedTime );
		else
			SetVelY( speed * fElapsedTime );
	}
	else if( inWaypoint == false  )
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
					if( Special_Timer <= 0.0f )
					{
						GetAnimInfo()->SetAnimationName("FireBoss_Special_Animation");

						//Color change for testing


						CCreate_FireSpecial_Message* c_pProjectMSG = new CCreate_FireSpecial_Message(ENT_ENEMY, IsFlipped(), Prev_Tar_Pos.x, Prev_Tar_Pos.y);
						m_dwRenderColor = 0xFFFFFFFF;
						

						CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);


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


						m_fPunchCoolDown = 3.5f;

						GetAnimInfo()->SetAnimationName("FireBoss_Attack_Animation");
						

					}
				}



				//Decrement punch timer always
				m_fPunchCoolDown -= fElapsedTime;


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
void CFireBoss::Render(void)
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), m_dwRenderColor);

	CEnemy::Render();

}

/*virtual*/ RECT CFireBoss::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}
/*virtual*/ Rect CFireBoss::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CFireBoss::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetEventID() == "New_Player")
		SetTarget((CEntity*)(pEvent->GetParam()));
	if (pEvent->GetDestination() == this)
	{
		if (pEvent->GetEventID() == "Fire Special")
		{
			CCreate_FireBoss_Fireball_Message* c_pProjectMSG = new CCreate_FireBoss_Fireball_Message(ENT_ENEMY, IsFlipped());
			c_pProjectMSG->SetPosX(this->GetPosX());
			c_pProjectMSG->SetPosY(this->GetPosY());

			CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
		}

		if (pEvent->GetEventID() == "ModifyHealth")
		{
			if( GetHealth() < (int( StartHealth * 0.4f)) )
				ModifyHealth( (*(int*)(pEvent->GetParam())  - 1 ) );
			else
				ModifyHealth( *(int*)(pEvent->GetParam()) );
		}

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
		
	}
}
