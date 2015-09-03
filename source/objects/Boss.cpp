/*
File:		Boss.cpp
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1308
Purpose:	CBoss class is a base class for al
*/

#include "Boss.h"
#include "Player.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../objects/EntityManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"

#include "../Messages/Create_FireSpecial_Message.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../Messages/Create_IceSpecial_Message.h"
#include "../Messages/Create_IceBall_Message.h"
#include "../Messages/DropEXPMessage.h"


#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_Direct3D.h"

#include "../Camera.h"

CBoss::CBoss(void)
{
	// Waypoint Maintanence
	m_nWayPointCounter	= 0;
	m_fWayPointTimer	= 0.0f;
	m_fSpecialTimer		= 0.0f;
	m_bAtWayPoint		= false;

	//Setup default prev_target_Waypoint
	Prev_Tar_Pos.x = 0;
	Prev_Tar_Pos.y = 0;

	// Pathfinding 
	m_fPosXOld			= 0.0f;
	m_fMoveAway			= 0.0f;
	m_fPosYOld			= 0.0f;
	m_fUpdateOldPos		= 0.0f;
		m_bEvadeUp			= false;

	// Attacking
	m_nCoolDown			= 5.0f;
	m_fPunchCoolDown	= 2.5f;
	m_bPunching			= false;
	m_bInAttackRange	= false;

	m_szSpecialAnim		= "";
}

CBoss::CBoss(float fWayPointTimer, float fSpecialTimer, bool bAtWayPoint, int nWayPointCounter, int nHealth) : CEnemy(nHealth)
{
	// Waypoint Maintanence
	m_nWayPointCounter	= nWayPointCounter;
	m_fWayPointTimer	= fWayPointTimer;
	m_fSpecialTimer		= fSpecialTimer;
	m_bAtWayPoint		= bAtWayPoint;

	//Setup default prev_target_Waypoint
	Prev_Tar_Pos.x = 0;
	Prev_Tar_Pos.y = 0;

	// Pathfinding 
	m_fPosXOld			= 0.0f;
	m_fMoveAway			= 0.0f;
	m_fPosYOld			= 0.0f;
	m_fUpdateOldPos		= 0.0f;
	m_bEvadeUp			= false;

	// Attacking
	m_nCoolDown			= 5.0f;
	m_fPunchCoolDown	= 2.5f;
	m_bPunching			= false;
	m_bInAttackRange	= false;

	m_szSpecialAnim		= "";
}




CBoss::~CBoss(void)
{
}

/*virtual*/ void CBoss::Update(float fElapsedTime)	
{
	//Take a previous position to compare to the current to see if player is standing still
	if( GetSpecialTimer() >= 2.5 )
	{
		m_fPosXOld =  GetTarget()->GetPosX();
		m_fPosYOld =  GetTarget()->GetPosY();
	}

	CCamera* cam = CCamera::GetInstance();
	if( GetSpecialTimer() <= 1.0f && GetSpecialTimer() >= 0.5f )
	{
		Prev_Tar_Pos.x = GetTarget()->GetPosX();
		Prev_Tar_Pos.y = GetTarget()->GetPosY();
	}


	//Check positions to see if player has been standing still
	if( m_fPosXOld == Prev_Tar_Pos.x + 5 || m_fPosXOld == Prev_Tar_Pos.x - 5 )
		if( m_fPosYOld == Prev_Tar_Pos.y + 5 || m_fPosYOld == Prev_Tar_Pos.y - 5 )
			SetSpecialTimer(GetSpecialTimer() - 1.0f);




	//Reset waypoint counter, timer, and bool if the enemy has been there long enough
	if( AtWayPoint() == true && GetWayPointTimer() <= 0.0f )
	{
		SetWayPointCounter(GetWayPointCounter() + 1);
		SetWayPointTimer(5.0f);
		SetAtWayPoint(false);
		SetRenderColor(255, 255, 255, 255);
	}
	else if( AtWayPoint() == true && GetWayPointTimer() > 0.0f )
	{
		SetWayPointTimer(GetWayPointTimer() - fElapsedTime);
		SetSpecialTimer(GetSpecialTimer() - fElapsedTime);
		SetVelX(0.0f);
		SetVelY(0.0f);
	}

	if( GetWayPointCounter() > 2 )
		SetWayPointCounter(0);


	float tar_pos_x = GetTarget()->GetPosX();
	float tar_pos_y = GetTarget()->GetPosY();
	if(GetWayPoint(GetWayPointCounter()).x > GetPosX() && AtWayPoint() == false)
	{
		//set Grunt's animation's facing to the right
		SetFlipped(true);
	}
	else
	{
		SetFlipped(false);
	}

	//Have him face the player once at his waypoint
	if(tar_pos_x > GetPosX() && AtWayPoint() == true)
	{
		//set animation facing to the right
		SetFlipped(true);
	}
	else if( tar_pos_x < GetPosX() && AtWayPoint() == true )
	{
		SetFlipped(false);
	}

	//Move enemy towards current waypoint
	if( AtWayPoint() == false )
	{
		if( GetWayPoint(GetWayPointCounter()).x < GetPosX() )
			SetVelX( -speed * fElapsedTime );
		else
			SetVelX( speed * fElapsedTime );

		if( GetWayPoint(GetWayPointCounter()).y < GetPosY() )
			SetVelY( -speed * fElapsedTime );
		else
			SetVelY( speed * fElapsedTime );
	}

	//Check to see if they are in the waypoint (or close enough)
	if( GetWayPoint(GetWayPointCounter()).x - GetPosX() < 64 && GetWayPoint(GetWayPointCounter()).x - GetPosX() > -64 )
		if( GetWayPoint(GetWayPointCounter()).y - GetPosY() < 64 && GetWayPoint(GetWayPointCounter()).y - GetPosY() > -64 )
			if( AtWayPoint() == false )
			{
				SetAtWayPoint(true);
				SetWayPointTimer(5.0f);
			}

			//if the player can be shot at.
			if( GetPosX() - tar_pos_x < 350 && GetPosX() - tar_pos_x > -350 )
			{
				if(GetPosY() - tar_pos_y < 150 && GetPosY() - tar_pos_y > -150 && m_bPunching == false)
				{
					if( AtWayPoint() == true  && GetSpecialTimer() <= 0.0f )
					{
						GetAnimInfo()->SetAnimationName("IceBoss_Special_Animation");

						SetRenderColor(255, 0, 0, 255);


						CCreate_IceSpecial_Message* c_pProjectMSG = new CCreate_IceSpecial_Message(ENT_ENEMY, IsFlipped(), Prev_Tar_Pos.x, Prev_Tar_Pos.y);


						CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);

						//Reset Special timer
						SetSpecialTimer(7.0f);
					}
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

/*virtual*/ void CBoss::Render(void)							
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), GetRenderColor());

	CEnemy::Render();
}

/*virtual*/ void CBoss::HandleEvent(CEvent* pEvent)	
{
	CEnemy::HandleEvent(pEvent);
}
