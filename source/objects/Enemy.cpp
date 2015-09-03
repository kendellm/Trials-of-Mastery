#include "Enemy.h"
#include "EntityState.h"
#include "Trap.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/SGD_Util.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/Animation.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"

#include "../Messages/DropEXPMessage.h"

#include "../Inputs/AudioManager.h"

#include "../objects/Projectile.h"
#include "../objects/Fireball.h"
#include "../objects/Shockwave.h"

#include "../Camera.h"

#include <sstream>

CEnemy::CEnemy(int nHealth)
{
	CEntity::m_eType = ENT_ENEMY;

	m_nHealth = nHealth;
	m_nMaxHealth = nHealth;

	GetAnimInfo()->SetAnimationName("MVC_TEST");

	m_pEState = new CEntityState(this);

	SetPosX(700);
	SetPosY(300);
	m_pTarget = nullptr;
	MoveLeft = true;
	speed = 100;
	m_nExpPts = 10;
	CSGD_EventSystem::GetInstance()->RegisterClient("Set_State", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Dodge_Pitfall", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Kill_Self", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("New_Player", this);

	m_bOnScreen = false;
}

CEnemy::~CEnemy(void)
{
	if(m_pTarget != nullptr)
		m_pTarget->Release();

	SAFE_DELETE(m_pEState);

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

/*virtual*/ void CEnemy::Update(float fElapsedTime)
{
	//Update the Animation Manager
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());

	if (GetHealth() <= 0 && m_pEState->GetState() != CEntityState::DEAD)
	{
		m_pEState->SetState(CEntityState::DEAD);
		return;
	}


	//Get Camera pointer to check position on screen
	CCamera* Game_Camera = CCamera::GetInstance();

	if( GetPosX() < (Game_Camera->GetWidth() + Game_Camera->GetPosX()) && GetPosX() > Game_Camera->GetPosX() )
		m_bOnScreen = true;

	if( m_bOnScreen == true )
	{
		//Check to see if the enemy goes off camera
		if( GetPosX() < Game_Camera->GetPosX() )
		{
			SetVelX(0);
			SetVelY(0);
			SetPosX(Game_Camera->GetPosX());
		}
		else if( GetPosX() > (Game_Camera->GetWidth() + Game_Camera->GetPosX()) )
		{
			SetVelX(0);
			SetVelY(0);
			SetPosX((Game_Camera->GetPosX() + Game_Camera->GetWidth()));
		}
	}
	if(GetPosY() > 600)
		SetPosY(599);
	else if(GetPosY() < 344)
		SetPosY(345);
}

/*virtual*/ void CEnemy::Render(void)
{
#if HIT_BOXES

	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetCollisionRect(), D3DCOLOR_ARGB(255, 0, 255, 0), 1);

	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetActiveRect().GetRECT(),  D3DCOLOR_ARGB(255, 255, 0, 0), 1);

	std::wostringstream wss;
	wss << GetHealth();
	CSGD_Direct3D::GetInstance()->DrawText(wss.str().c_str(), (int)(GetPosX() - GetWidth() / 2 - CCamera::GetInstance()->GetPosX()), (int)(GetPosY() - GetHeight() - CCamera::GetInstance()->GetPosY()), D3DCOLOR_ARGB(255, 255, 255, 255) );
	
	if(m_nHealth > 0)
	{
		float HP = float(m_nHealth);
		float MAX_HP = float(m_nMaxHealth);
		CCamera * GameCamera = CCamera::GetInstance();
		RECT temp;
		temp.left = (LONG)(GetPosX() - 30 - GameCamera->GetPosX());
		temp.top = (LONG)(GetPosY() + 5);
		temp.bottom = temp.top + 5;
		temp.right = temp.left + (LONG)(60*(HP / MAX_HP));
		CSGD_Direct3D::GetInstance()->DrawRect(temp, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
#endif
}

/*virtual*/ RECT CEnemy::GetCollisionRect( void ) const
{
	return CEntity::GetCollisionRect();
}

/*virtual*/ Rect CEnemy::GetActiveRect( void ) const
{
	return CEntity::GetActiveRect();
}

/*virtual*/ void CEnemy::HandleCollision(const IEntity* pOther, const RECT rOverlap)
{
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

/*virtual*/ void CEnemy::HandleActiveRectCollision ( const IEntity* pOther )
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
			}
		}
		break;
	default:
		break;
	}
}

/*virtual*/ void CEnemy::HandleEvent(CEvent* pEvent)
{
	if (pEvent->GetEventID() == "New_Player")
		SetTarget((CEntity*)(pEvent->GetParam()));
	if (pEvent->GetDestination() == this)
	{
		if (pEvent->GetEventID() == "ModifyHealth")
		{
			if (pEvent->GetDestination() == this)
			{
				ModifyHealth( *(int*)(pEvent->GetParam()) );
				m_pEState->SetState(m_pEState->FLINCHING);
			}
		}
		if (pEvent->GetDestination() == this && pEvent->GetEventID() == "Set_State")
		{
			m_pEState->SetState((CEntityState::ENTITY_STATE)(int)(pEvent->GetParam()) );
		}

		if (pEvent->GetDestination() == this && pEvent->GetEventID() == "Dodge_Pitfall")
		{
			HandleCollision((const IEntity*)pEvent->GetSender(), *(const RECT*)pEvent->GetParam());
		}

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

void CEnemy::SetTarget(CEntity* target)
{
	if(m_pTarget != nullptr)
		m_pTarget->Release();
	m_pTarget = target;
	if(m_pTarget != nullptr)
		m_pTarget->AddRef();
}

/*virtual*/ void CEnemy::ModifyHealth(int damage)
{
	m_nHealth -= damage;

}

void CEnemy::ModifyMovement( float fElaspedTime )
{
	CEntity::Update(fElaspedTime);
}
