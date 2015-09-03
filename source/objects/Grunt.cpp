#include "Grunt.h"
#include "EntityState.h"
#include "EntityManager.h"


#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/SGD_Util.h"
#include "../CSGD/CSGD_Direct3D.h"

#include "../Messages/DropEXPMessage.h"

#include "../TileSystem/TileManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"

#include "../CSGD/RemoveEntityMessage.h"

#include "../Camera.h"




#include <sstream>


CGrunt::CGrunt(float fHealthScale) : CEnemy(20)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(Grunt);

	AnimationManager::GetInstance()->LoadAnimationFile("config/Grunt_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Grunt_Death_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Grunt_Attack_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Grunt_Flinch_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Grunt_Knocked_Down_Animation.xml");

	GetAnimInfo()->SetAnimationName("Grunt_Walk_Animation");


	SetBaseAnimations(ENT_ENEMY);

	punching = false;
	cooldown = 0.0f;
	SetHealth((int)(GetHealth()*(1.0f + fHealthScale)));

	SetIdleAnim("Grunt_Walk_Animation");

	m_bFlipped = false;
	m_fMoveAway = 0.0f;
	m_bEvadeUp = false;
	m_fPosXOld = 0.0f;
	m_fPosYOld = 0.0f;
	m_fUpdateOldPos = 0.0f;
	m_nExpPts = 50;

	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
}
CGrunt::~CGrunt(void)
{
	SAFE_DELETE(m_pEState);

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CGrunt::Update(float fElapsedTime)
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
	if( cooldown <= 0.0f )
		punching = false;

	if( punching == true )
		cooldown -= fElapsedTime;

	if (GetAnimInfo()->GetAnimationName() == "Grunt_Attack_Animation")
		return;

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
			// Set Grunt's animation to face to the left
			m_bFlipped = false;
		}

		SetFlipped( m_bFlipped );


		if(m_fMoveAway <= 0)
		{
			tar_pos_x += (m_bFlipped) ? -66 : 66;

			//Simple Pathing twards the player
			if(tar_pos_y != GetPosY() )//Above the Player
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

			//stop 'bouncing'
			int threshold = 5;
			if(tar_pos_x - GetPosX() < threshold && tar_pos_x - GetPosX() > -1 * threshold)
				SetVelX(0);
			if(tar_pos_y - GetPosY() < threshold && tar_pos_y - GetPosY() > -1 * threshold)
				SetVelY(0);
		}
		else//update move away
		{
			m_fMoveAway -= fElapsedTime;
			if(m_bEvadeUp)
				SetVelY(-speed * fElapsedTime);
			else
				SetVelY(speed * fElapsedTime);
		}
		//Check Colider
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))
		{
			SetPosX(GetPosX() + GetVelX());
			if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
				SetPosY(GetPosY() + GetVelY());
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
		if(punching == false && GetPosX() - tar_pos_x < 10 && GetPosX() - tar_pos_x > - 10 )
		{
			if(GetPosY() - tar_pos_y < 32 && GetPosY() - tar_pos_y > -32)
			{
				GetAnimInfo()->SetAnimationName("Grunt_Attack_Animation");
				punching = true;
				cooldown = 3.0f;
			}
		}
		//Set/use Move out of the way
		if(m_fUpdateOldPos >= 1.0f)
		{
			if(m_fPosXOld + 5 > GetPosX() && m_fPosXOld - 5 < GetPosX())
			{
				if(m_fPosYOld + 5 > GetPosY() && m_fPosYOld - 5 < GetPosY())
				{
					if(cooldown <= 0)
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

	
}

void CGrunt::Render(void)
{

	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), m_bFlipped, *GetAnimInfo(), GetRenderColor());

	CEnemy::Render();
}

/*virtual*/ RECT CGrunt::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}

/*virtual*/ Rect CGrunt::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}
