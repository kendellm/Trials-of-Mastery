#include "Fighter.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"

#include "../Inputs/AudioManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../objects/EntityManager.h"

#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"



CFighter::CFighter(float fHealthScale) : CEnemy(30)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(Fighter);

	m_dwRenderColor	= 0xFFFFFFFF;
	SetHealth((int)(GetHealth()*(1.0f + fHealthScale)));

	//Animation set up
	AnimationManager::GetInstance()->LoadAnimationFile("config/Fighter_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Fighter_Death_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Fighter_Attack_Animations.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Fighter_Flinch_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Fighter_Knocked_Down_Animation.xml");

	GetAnimInfo()->SetAnimationName("Fighter_Walk_Animation");
	SetBaseAnimations(ENT_ENEMY);


	m_nExpPts = 75;

	m_bEvadeUp		= true;
	m_bAttacking	 = false;
	m_bSecondAttackQueued = false;

	m_fPosXOld = 0.0f;
	m_fPosYOld = 0.0f;
	m_fMoveAway = 0.0f;
	m_fUpdateOldPos = 0.0f;
	cooldown = 0.0f;

	m_vAttacks.push_back("Fighter_Knee");
	m_vAttacks.push_back("Fighter_Low_Kick");
	m_vAttacks.push_back("Fighter_Punch_L");
	m_vAttacks.push_back("Fighter_Punch_M");

	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);

	CSGD_EventSystem::GetInstance()->RegisterClient("Set_State", this);
}
CFighter::~CFighter(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CFighter::Update(float fElapsedTime)
{
	// Update common properties across all enemies
	CEnemy::Update(fElapsedTime);

	// if this enemy is cc'd don't 
	if (m_pEState->GetTimer() == true)
	{
		m_pEState->CEntityState::Update(fElapsedTime);
		return;
	}

	if (GetAnimInfo()->GetAnimationName() == GetAnimations("Idle") && m_bAttacking == true)
		m_bSecondAttackQueued = m_bAttacking = false;


	if (m_bAttacking == true)
		return;

	m_fUpdateOldPos += fElapsedTime;

	if(GetTarget() != nullptr)
	{
		float tar_pos_x = GetTarget()->GetPosX() - 32;
		float tar_pos_y = GetTarget()->GetPosY();

		if(tar_pos_x > GetPosX())
		{
			//set Grunt's animation's facing to the right
			SetFlipped( true );
		}
		else
		{
			SetFlipped( false );
		}


		if(m_fMoveAway <= 0)
		{

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
				int threshold = 5;
				if(tar_pos_x - GetPosX() < threshold && tar_pos_x - GetPosX() > -1 * threshold)
					SetVelX(0);
				if(tar_pos_y - GetPosY() < threshold && tar_pos_y - GetPosY() > -1 * threshold)
					SetVelY(0);
			}
			else
			{
				SetVelY(0);
				if( tar_pos_x < GetPosX())
					SetVelX(-speed * fElapsedTime);
				else
					SetVelX(speed * fElapsedTime);
			}

		}
		else
		{
			m_fMoveAway -= fElapsedTime;
			if(m_bEvadeUp)
				SetVelY(-speed * fElapsedTime);
			else
				SetVelY(speed * fElapsedTime);
		}

		//Check Colider
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))
			SetPosX(GetPosX() + GetVelX());
		else if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), -GetVelX(), 0))
			SetPosX(GetPosX() - GetVelX());
		else
		{
			if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
				SetPosY(GetPosY() + GetVelY());
			else if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, -GetVelY()))
				SetPosY(GetPosY() - GetVelY());
			else
			{
				SetPosX(m_fStartPosX);
				SetPosY(m_fStartPosY);
			}
		}
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
			SetPosY(GetPosY() + GetVelY());
		else if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, -GetVelY()))
			SetPosY(GetPosY() - GetVelY());
		if(GetPosX() < 0)
			SetPosX(0);

		if(cooldown <= 0)
		{
			if((GetPosX() - tar_pos_x) < 120 && (GetPosX() - tar_pos_x) > -64 )
			{
				if(GetPosY() - tar_pos_y < 16 && GetPosY() - tar_pos_y > -16)
				{
					GetAnimInfo()->SetAnimationName(m_vAttacks[rand() % 4]);
					m_bAttacking = true;
					cooldown = 3.0f;
				}
			}
		}
		else
			cooldown -= fElapsedTime;

		//Update/use move away
		if(m_fUpdateOldPos >= 0.5f)
		{
			if(m_fPosXOld + 5 > GetPosX() && m_fPosXOld - 5 < GetPosX())
			{
				if(m_fPosYOld + 5 > GetPosY() && m_fPosYOld - 5 < GetPosY())
				{
					//if(!m_bRushing)
					{
						m_bEvadeUp = !m_bEvadeUp;
						if(m_fMoveAway <= 0)
							m_fMoveAway = 3.0f;
					}
				}
			}
			m_fPosXOld = GetPosX();
			m_fPosYOld = GetPosY();
			m_fUpdateOldPos = 0.0f;
		}
	}
}

void CFighter::Render(void)
{
	CCamera* Game_Camera = CCamera::GetInstance();
	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # 33 / 56
	// BUG FIX START
	/////////////////////////////////////////////////////////////////
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), GetRenderColor());
	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # 33 / 56
	//////////////////////////////////////////////////////////////////////////
	CEnemy::Render();

}

/*virtual*/ RECT CFighter::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}

/*virtual*/ Rect CFighter::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CFighter::HandleActiveRectCollision ( const IEntity* pOther )
{
	switch (pOther->GetType())
	{
	case ENT_PLAYER:
		{
			// a boolean to check if an object has already been hit
			bool found = false;

			// Have I already hit the player this animation?

			// Make an Iterator to the list of enemies
			SLLIter<const IEntity*> iter(GetHitEnemies());

			// Check if the List is empty
			if (GetHitEnemies().size() != 0)
			{
				// Iterator through the list looking for the object that was hit
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
				// Save the damage of the current frame in a temporary variable
				int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(GetAnimInfo())->GetDamage();

				// Create an event to deal damage to the player
				CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)pOther, this);

				// Add the player to the entity's enemies hit list
				GetHitEnemies().addHead(pOther);

				// Play an awesome sound effect
				CAudioManager::GetInstance()->Play( (SFX)(rand() % 3 + 4) );

				//Queue up another attack because come on this is the fighter
				if (m_bSecondAttackQueued == false)
				{
					GetAnimInfo()->QueueAnimation(m_vAttacks[rand() % 4]);
					m_bSecondAttackQueued = true;
				}
			}
		}
		break;
	default:
		break;
	}
}


