#include "EarthBoss.h"
#include "Player.h"
#include "EntityState.h"

#include "../TileSystem/TileManager.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"

#include "../objects/EntityManager.h"

#include "../Inputs/AudioManager.h"

#include "../Messages/DropEXPMessage.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../Messages/Create_Rock_Fall_Message.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Camera.h"
#include "../CSGD/CSGD_Direct3D.h"


CEarthBoss::CEarthBoss(void) : CEnemy(100)
{
	CEntity::m_eType = ENT_ENEMY;
	SetEnemyType(EarthBoss);
	m_nExpPts = 500;

	AnimationManager::GetInstance()->LoadAnimationFile("config/Boss1_Walk_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Boss1_Shoot_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Boss1_Attack_Animation.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Boss1_Death_Animation.xml");

	// Set the player's starting animation to "idle"

	GetAnimInfo()->SetAnimationName("Boss1_Walk_Animation");
	SetBaseAnimations(ENT_ENEMY);

	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Self Destruct", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);

	m_nCoolDown			= 5.0f;

	m_fPunchCoolDown	= 2.5f;
	m_bPunching			= false;

	m_bEvadeUp			= false;
	m_fPosXOld			= 0.0f;
	m_fMoveAway			= 0.0f;
	m_fPosYOld			= 0.0f;
	m_fUpdateOldPos		= 0.0f;
	speed = 75;

	m_fSpecialTimer		= 15.0f;

}
CEarthBoss::~CEarthBoss(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CEarthBoss::Update(float fElapsedTime)
{
	CEnemy::Update(fElapsedTime);

	if (m_pEState->GetState() == EntityState::DEAD)
	{
		return;
	}

	m_fUpdateOldPos += fElapsedTime;
	if(m_nCoolDown > 0)
		m_nCoolDown -= fElapsedTime;

	if( m_fPunchCoolDown > 0.0f  && m_bPunching == true)
		m_fPunchCoolDown -= fElapsedTime;
	else
	{
		m_fPunchCoolDown = 2.5f;
		m_bPunching = false;
	}

	if (m_fSpecialTimer > 0.0f)
		m_fSpecialTimer -= fElapsedTime;
	else
	{
		CCreate_Rock_Fall_Message* pMsg = new CCreate_Rock_Fall_Message();
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;

		m_fSpecialTimer = 15.0f;
	}

	//if not rushing do basic path finding to the player
	if(GetTarget() != nullptr)
	{
		float tar_pos_x = GetTarget()->GetPosX();
		float tar_pos_y = GetTarget()->GetPosY();
		if(tar_pos_x > GetPosX())
		{
			//set Grunt's animation's facing to the right
			SetFlipped(true);
		}
		else
		{
			SetFlipped(false);
		}


		if(m_fMoveAway <= 0)
		{

			//	tar_pos_x += (m_bFlipped) ? -236 : 236;

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

					if( tar_pos_x > (GetPosX() - 64) && tar_pos_x < (GetPosX() + 64) )
					{


						if( tar_pos_y > (GetPosY() - 32) && tar_pos_y < (GetPosY() + 32) && m_bPunching == false)
						{

							GetAnimInfo()->SetAnimationName("Boss1_Attack_Animation");
							m_bPunching = true;
							//dynamic_cast<CPlayer*>(GetTarget())->SetState(CEntityState::KNOCKED_DOWN);
						}
					}
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
			if(m_nCoolDown <= 0 && GetPosX() - tar_pos_x < 128 && GetPosX() - tar_pos_x > -128 )
			{
				if(GetPosY() - tar_pos_y < 5 && GetPosY() - tar_pos_y > -5 && m_bPunching == false)
				{

					//GetAnimInfo()->SetAnimationName("Boss1_Shoot_Animation");
					////if in range rushing starts
					//CCreate_Projectile_Message* c_pProjectMSG = new CCreate_Projectile_Message(ENT_ENEMY, IsFlipped());
					//c_pProjectMSG->SetPosX(this->GetPosX());
					//c_pProjectMSG->SetPosY(this->GetPosY());

					//CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
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

/*virtual*/ void CEarthBoss::Render(void)
{
	CCamera* Game_Camera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), GetRenderColor());

	CEnemy::Render();
}

/*virtual*/ void CEarthBoss::HandleEvent(CEvent* pEvent)
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
	}
}

/*virtual*/ void CEarthBoss::HandleActiveRectCollision ( const IEntity* pOther )
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

				// Reduce the special timer by 1 second 
				m_fSpecialTimer -= 1.0f;
			}
		}
		break;
	default:
		break;
	}
}

