#include "Tank.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../objects/EntityManager.h"

#include "../CSGD/RemoveEntityMessage.h"
#include "../Messages/DropEXPMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../Inputs/AudioManager.h"



CTank::CTank(float fHealthScale) : CEnemy(40)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(Tank);


	m_dwRenderColor	= 0xFFFFFFFF;
	m_bFlipped = false;
	SetHealth((int)(GetHealth()*(1.0f + fHealthScale)));

	//Set rush variables to default values
	Rush_Timer = 3.0f;

	m_nRushDirection = 0;
	m_fMoveAway = 0.0f;
	m_bEvadeUp = false;
	m_fPosXOld = 0.0f;
	m_fPosYOld = 0.0f;
	m_fUpdateOldPos = 0.0f;
	m_nExpPts = 50;
	AnimationManager::GetInstance()->LoadAnimationFile("config/Jugg_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Jugg_Rush_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Jugg_Death_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Jugg_Flinch_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Jugg_Knocked_Down_Animation.xml");

	// Set the player's starting animation to "idle"
	GetAnimInfo()->SetAnimationName("Jugg_Walk_Animation");

	SetBaseAnimations(ENT_ENEMY);

	RushWait = 3.0f;
	m_bRushing = false;

	speed = 50;

	m_bIsDead = false;

	
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Set_State", this);
}
CTank::~CTank(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CTank::Update(float fElapsedTime)
{
	// Update common properties across all enemies
	CEnemy::Update(fElapsedTime);

	// if this enemy is cc'd don't 
	if (m_pEState->GetTimer() == true)
	{
		m_pEState->CEntityState::Update(fElapsedTime);
		return;
	}
	
	//decrement rush timer while rushing
	m_fUpdateOldPos += fElapsedTime;
	if( m_bRushing == true )
		Rush_Timer -= fElapsedTime;

	//if rush timer gets below 0 end rushing attack
	if( Rush_Timer <= 0.0f )
	{
		m_bRushing = false;
		Rush_Timer = 3.0f;
		RushWait = 5.0f;
		GetAnimInfo()->SetAnimationName("Jugg_Walk_Animation");
	}

	//if not rushing do basic path finding to the player
	if(GetTarget() != nullptr && m_bRushing == false)
	{
		RushWait -= fElapsedTime;
		float tar_pos_x = GetTarget()->GetPosX() - 32;
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
	
		SetFlipped( m_bFlipped );


		if(m_fMoveAway <= 0)
		{
			//Simple Pathing twards the player
			if(tar_pos_y != GetPosY())//Above the Player
			{

				float min_Distance = 120;
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
			
			//if the player is in front of him or behind and in range of rush attack
			if( GetPosX() - tar_pos_x < 300 && GetPosX() - tar_pos_x > -300 )
			{
				if(GetPosY() - tar_pos_y < 32 && GetPosY() - tar_pos_y > -32 && RushWait <= 0)
				{
					GetHitEnemies().clear();
					GetAnimInfo()->SetAnimationName("Jugg_Rush_Animation");
					//if in range rushing starts
					m_bRushing = true;

					if( GetPosX() - tar_pos_x < 0 )					// if player is in front of him
						m_nRushDirection = 1;						// set enemy to move forward
					else											// if not
						m_nRushDirection = -1;						// move backwards towards player
				}
			}
			if(m_bRushing == false)
			{
				int threshold = 5;
				if(tar_pos_x - GetPosX() < threshold && tar_pos_x - GetPosX() > -1 * threshold)
					SetVelX(0);
				if(tar_pos_y - GetPosY() < threshold && tar_pos_y - GetPosY() > -1 * threshold)
					SetVelY(0);
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
	}

	if( m_bRushing == true )
	{
		SetVelX( (float)(m_nRushDirection * 3) * speed * fElapsedTime );
		SetVelY(0);
		CCamera* Game_Camera = CCamera::GetInstance();
		if(Game_Camera->GetPosX() + 10 > GetPosX() || Game_Camera->GetPosX() + Game_Camera->GetWidth() - 10 < GetPosX())
		{
			Rush_Timer = 0;
			CSGD_EventSystem::GetInstance()->SendEventNow("Set_State", (void*) 2, this, this);
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
	if(m_fUpdateOldPos >= 2.0f)
	{
		if(m_fPosXOld + 5 > GetPosX() && m_fPosXOld - 5 < GetPosX())
		{
			if(m_fPosYOld + 5 > GetPosY() && m_fPosYOld - 5 < GetPosY())
			{
				if(!m_bRushing)
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

void CTank::Render(void)
{
	CCamera* Game_Camera = CCamera::GetInstance();

	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), m_bFlipped, *GetAnimInfo(), GetRenderColor());

	CEnemy::Render();
	
}
/*virtual*/ void CTank::HandleCollision(const IEntity* pOther, const RECT rOverlap)
{
	if(m_bRushing == true)
		return;
	int width = rOverlap.right - rOverlap.left;
	int height = rOverlap.bottom - rOverlap.top;

	// Check if the collision was a horizontal or vertical collision
	if ( width < height )
	{
		// Check if this object is left or right of the object it collided with
		if ( GetPosX() < ((CEntity*)(pOther))->GetPosX() )
		{ 
			SetPosX( GetPosX() - width );
		}
		else
		{
			SetPosX( GetPosX() + width );
		}
	}
	else
	{
		// Check if this object if above or below the object it collided with
		if ( GetPosY() < ((CEntity*)(pOther))->GetPosY() )
		{
			SetPosY( GetPosY() - height  );
		}
		else
		{
			SetPosY( GetPosY() + height );
		}
	}
}

/*virtual*/ RECT CTank::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}

/*virtual*/ Rect CTank::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CTank::HandleActiveRectCollision ( const IEntity* pOther )
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

				int temp = EntityState::KNOCKED_DOWN;
				// Create an event to deal damage to the player
				CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)pOther, this);
				CSGD_EventSystem::GetInstance()->SendEventNow("Set_State", &temp, (void*)pOther, this);
				// Add the player to the entity's enemies hit list
				GetHitEnemies().addHead(pOther);

				// Play an awesome sound effect
				CAudioManager::GetInstance()->Play( (SFX)(rand() % 3 + 4) );

				if(Rush_Timer > 0)
					Rush_Timer = (float)(min(Rush_Timer, 1.0f));
			}
		}
		break;
	default:
		break;
	}
}