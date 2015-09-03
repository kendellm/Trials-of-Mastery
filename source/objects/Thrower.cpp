#include "Thrower.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../objects/EntityManager.h"

#include "../CSGD/RemoveEntityMessage.h"
#include "../Messages/DropEXPMessage.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"
#include "../CSGD/CSGD_Direct3D.h"

#include <sstream>

CThrower::CThrower(float fHealthScale) : CEnemy(10)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(Thrower);

	AnimationManager::GetInstance()->LoadAnimationFile("config/thrower_animations.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/thrower_flinch_animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Thrower_Knocked_Down_Animation.xml");

	GetAnimInfo()->SetAnimationName("Thrower_Idle");
	SetBaseAnimations(ENT_ENEMY);

	punching = false;
	cooldown = 0.0f;
	m_dwRenderColor	= 0xFFFFFFFF;
	SetHealth((int)(GetHealth()*(1.0f + fHealthScale)));
	

	m_bFlipped = false;
	speed = 75;
	m_nExpPts = 50;
	m_nCoolDown = 0.0f;
	m_fMoveAway = 0.0f;
	m_bEvadeUp = false;
	m_fPosXOld = 0.0f;
	m_fPosYOld = 0.0f;
	m_fUpdateOldPos = 0.0f;

	CSGD_EventSystem::GetInstance()->RegisterClient("Projectile", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
}
CThrower::~CThrower(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CThrower::Update(float fElapsedTime)
{
	// Update common properties across all enemies
	CEnemy::Update(fElapsedTime);

	// if this enemy is cc'd don't 
	if (m_pEState->GetTimer() == true)
	{
		m_pEState->CEntityState::Update(fElapsedTime);
		return;
	}

	m_fUpdateOldPos += fElapsedTime;
	if(m_nCoolDown > 0)
		m_nCoolDown -= fElapsedTime;


	//if not rushing do basic path finding to the player
	if(GetTarget() != nullptr)
	{
		float tar_pos_x = GetTarget()->GetPosX();
		float tar_pos_y = GetTarget()->GetPosY();
		if(tar_pos_x > GetPosX())
		{
			//set Grunt's animation's facing to the right
			m_bFlipped = true;
		}
		else
		{
			m_bFlipped = false;
		}
		if(m_fMoveAway <= 0)
		{
			if(m_bOnScreen == true)
				tar_pos_x += (m_bFlipped) ? -236 : 236;
			else
				tar_pos_x += (m_bFlipped) ? 236 : -236;

			//Simple Pathing twards the player
			if(tar_pos_y != GetPosY())//Above the Player
			{
				float min_Distance = (float)(GetTarget()->GetWidth()/2 + GetWidth()/2);
				if(GetPosX() + min_Distance > tar_pos_x && GetPosX() - min_Distance < tar_pos_x)
				{
					if( tar_pos_x < GetPosX())

						SetVelX(speed * fElapsedTime);
					else

						SetVelX(-speed * fElapsedTime);
				}
				else
				{
					if( tar_pos_y < GetPosY())

						SetVelY(-speed * fElapsedTime);
					else

						SetVelY(speed * fElapsedTime);
					if( tar_pos_x < GetPosX())

						SetVelX(-speed * fElapsedTime);
					else

						SetVelX(speed * fElapsedTime);
				}
			}
			else
			{
				SetVelY(0);
				if( tar_pos_x < GetPosX())

					SetVelX(-speed * fElapsedTime);
				else

					SetVelX(speed * fElapsedTime);
			}

			//if the player can be shot at.
			if(m_nCoolDown <= 0 && GetPosX() - tar_pos_x < 5 && GetPosX() - tar_pos_x > -5 )
			{
				if(GetPosY() - tar_pos_y < 5 && GetPosY() - tar_pos_y > -5)
				{
					GetAnimInfo()->SetAnimationName("Thrower_Attack");
					//if in range rushing starts
					m_nCoolDown += 5;
				}
			}
			//stop 'bouncing'
			int threshold = 5;
			if(tar_pos_x - GetPosX() < threshold && tar_pos_x - GetPosX() > -1 * threshold)
				SetVelX(0);
			if(tar_pos_y - GetPosY() < threshold && tar_pos_y - GetPosY() > -1 * threshold)
				SetVelY(0);
		}
		else //update move away
		{
			m_fMoveAway -= fElapsedTime;
			if(m_bEvadeUp)
				SetVelY(-speed * fElapsedTime);
			else
				SetVelY(speed * fElapsedTime);
		}
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
	if(GetPosX() < 0)
		SetPosX(0);

	//Set/use Move out of the way
	if(m_fUpdateOldPos >= 1.0f)
	{
		if(m_fPosXOld + 5 > GetPosX() && m_fPosXOld - 5 < GetPosX())
		{
			if(m_fPosYOld + 5 > GetPosY() && m_fPosYOld - 5 < GetPosY())
			{
				if(m_nCoolDown <= 0)
				{
					m_bEvadeUp = !m_bEvadeUp;
					if(m_fMoveAway <= 0)
						m_fMoveAway = 4.0f;
				}
			}
		}
		m_fPosXOld = GetPosX();
		m_fPosYOld = GetPosY();
		m_fUpdateOldPos = 0.0f;
	}
}
void CThrower::Render(void)
{

	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), m_bFlipped, *GetAnimInfo(), GetRenderColor());


	CEnemy::Render();

}

/*virtual*/ RECT CThrower::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}
/*virtual*/ Rect CThrower::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CThrower::HandleEvent(CEvent* pEvent)
{

	if (pEvent->GetDestination() == this)
	{
		if(pEvent->GetEventID() == "Projectile")
		{
			CCreate_Projectile_Message* c_pProjectMSG = new CCreate_Projectile_Message(ENT_ENEMY, m_bFlipped);
			c_pProjectMSG->SetPosX(this->GetPosX());
			c_pProjectMSG->SetPosY(this->GetPosY());
			CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
			return;
		}
	}
	CEnemy::HandleEvent(pEvent);

}
