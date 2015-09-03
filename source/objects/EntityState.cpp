/*
File:		EntityState.cpp
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1308
Purpose:	CEntityState class handles the switch of state of
entities in the game.
*/

#include "EntityState.h"
#include "Entity.h"
#include "Enemy.h"
#include "Human.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/Animation.h"
#include "../AnimationSystem/AnimInfo.h"

#include "../CSGD/CSGD_EventSystem.h"

#include "../TileSystem/TileManager.h"




CEntityState::CEntityState(CHuman* pOwner)
{
	m_pOwner = pOwner;

	m_fTimer		= 0.0f;
	m_fPrevTimer	= 0.0f;
	m_fElaspedTime  = 0.0f;
	m_eState = IDLE;
}



CEntityState::~CEntityState(void)
{
}



void CEntityState::SetState(ENTITY_STATE eState)
{
	m_eState = eState;

	if (m_pOwner->GetType() == CEntity::EntityType::ENT_PLAYER)
	{
		m_pOwner->GetAnimInfo()->GetNextAnimation().clear();
	}

	switch (eState)
	{
	case IDLE: // safe state
		m_pOwner->SetRenderColor(255, 255, 255, 255);
		break;

	case FLINCHING: // hit by an attack while not in super
		m_fTimer = 0.5;
		m_pOwner->GetAnimInfo()->SetAnimationName(m_pOwner->GetAnimations("Flinch"));
		break;

	case KNOCKED_DOWN: // hit by an ability that knocks down
		{
			const char * name = m_pOwner->GetAnimations("Knocked Down");
			if ( name != nullptr)
			{
				m_pOwner->GetAnimInfo()->SetAnimationName(name);
				m_fTimer = AnimationManager::GetInstance()->GetAnimationDuration(name);
			}
			else
			{
				m_pOwner->SetRenderColor(255, 0, 0, 0);
			}

		}
		break;

	case DEAD: // life points < 0
		{
			// set the timer for the owner's death animation length
			m_fTimer = 3.0f;
			m_pOwner->GetAnimInfo()->SetAnimationName(m_pOwner->GetAnimations("Dead"));
		}
		break;

	case FROZEN: // hit by an ability that freezes
		{	
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # 33 / 56
		// BUG FIX START
		/////////////////////////////////////////////////////////////////
			m_fTimer = 3.0f;
			const char * anim = m_pOwner->GetAnimations("Frozen");
			if (anim != nullptr)
				m_pOwner->GetAnimInfo()->SetAnimationName(anim);
			m_pOwner->SetRenderColor(255, 0, 111, 244);
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # 33 / 56
		//////////////////////////////////////////////////////////////////////////
		}
		break;

	case STUNNED: // for boss fights
		{
			m_fTimer = 1.5f;
			m_pOwner->SetRenderColor(255, 128, 64, 64);
		}
		break;

	case SUPER:  // cannot be flinched when hit
		{
			// Set the super for the length of the current frame's animation
			m_fTimer = 0.3f;
			m_pOwner->SetRenderColor(255, 0, 255, 0);
		}
		break;
	case PUSHED:  // cannot be flinched when hit
		{
			// Set the super for the length of the current frame's animation
			m_fTimer = 1.7f;
		}
		break;
	default:
		break;
	} 
}



void CEntityState::Update(float fElapsedTime)
{
	m_fElaspedTime = fElapsedTime;
	m_fPrevTimer = m_fTimer;

	if (m_fTimer > 0.0f)
		m_fTimer -= fElapsedTime;

	// if the timer ran out this update
	if (m_fTimer <= 0.0f && m_fPrevTimer > 0.0f)
	{
		m_fPrevTimer = m_fTimer = 0.0f;
		SetState(IDLE);
	}

	if(m_eState == ENTITY_STATE::PUSHED)
	{
		if(m_pOwner->GetType() == CEntity::ENT_ENEMY )
		{
			CEnemy* pEnemy = dynamic_cast<CEnemy*>(m_pOwner);

			if(TileManager::GetInstance()->CheckBlockedTiles(pEnemy->GetPosX(), pEnemy->GetPosY(), pEnemy->GetVelX(), 0))
			{
				( pEnemy->IsFlipped() ) ? pEnemy->SetVelX(-350) : pEnemy->SetVelX(350);

				m_pOwner->SetPosX(pEnemy->GetPosX() + pEnemy->GetVelX() * fElapsedTime);
				if(TileManager::GetInstance()->CheckBlockedTiles(pEnemy->GetPosX(),
					pEnemy->GetPosY(), 0, pEnemy->GetVelY())) 
				{
					pEnemy->SetPosY(pEnemy->GetPosY() + pEnemy->GetVelY() * fElapsedTime);
				}
			}	
		}
	}
}
