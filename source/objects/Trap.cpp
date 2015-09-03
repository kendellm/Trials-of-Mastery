#include "Trap.h"
#include "Player.h"
#include "Enemy.h"
#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/AnimInfo.h"
#include "../AnimationSystem/Frame.h"
#include "../Camera.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../Messages/Create_Wrecking_Ball_Message.h"
#include "../Messages/Create_Rock_Fall_Message.h"

CTrap::CTrap(void)
{
	AnimationManager::GetInstance()->LoadAnimationFile("config/Pit_Fall.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Conveyor_Belt_Down.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Conveyor_Belt_Up.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Conveyor_Belt_Left.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Conveyor_Belt_Right.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Wrecking_Ball_Warning.xml");
	AnimationManager::GetInstance()->LoadAnimationFile("config/Rock_Fall_Warning.xml");

	m_fTime = 0.0f;
	m_eType = ENT_TRAP;
}
CTrap::~CTrap(void)
{

}

void CTrap::SetTrapType(int t)
{
	Trap_Type = t;
	switch (Trap_Type)
	{
	case Pit_Fall:
		{
			GetAnimInfo()->SetAnimationName("Pit_Fall");
			SetIdleAnim("Pit_Fall");
			break;
		}
	case Wrecking_Ball:
		{
			GetAnimInfo()->SetAnimationName("Wrecking_Ball_Warning");
			SetIdleAnim("Wrecking_Ball_Warning");
			break;
		}
	case Rock_Slide:
		{
			GetAnimInfo()->SetAnimationName("Rock_Fall_Warning");
			SetIdleAnim("Rock_Fall_Warning");
			break;
		}
	case Conveyor_Up:
		{
			GetAnimInfo()->SetAnimationName("Conveyor_Belt_Up");
			SetIdleAnim("Conveyor_Belt_Up");
			break;
		}
	case Conveyor_Down:
		{
			GetAnimInfo()->SetAnimationName("Conveyor_Belt_Down");
			SetIdleAnim("Conveyor_Belt_Down");
			break;
		}
	case Conveyor_Left:
		{

			GetAnimInfo()->SetAnimationName("Conveyor_Belt_Left");
			SetIdleAnim("Conveyor_Belt_Left");
			break;
		}
	case Conveyor_Right:
		{
			GetAnimInfo()->SetAnimationName("Conveyor_Belt_Right");
			SetIdleAnim("Conveyor_Belt_Right");
			break;
		}
	default:
		break;
	}
}

void CTrap::HandleEvent(CEvent* pEvent)
{

}
void CTrap::Update(float fElapsedTime)
{
	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());
	switch (Trap_Type)
	{
	case Pit_Fall:
		{
			break;
		}
	case Wrecking_Ball:
		{
			(m_fTime > 0) ? m_fTime -= fElapsedTime : m_fTime = 0;
			break;
		}
	case Rock_Slide:
		{
			(m_fTime > 0) ? m_fTime -= fElapsedTime : m_fTime = 0;
			break;
		}
	case Conveyor_Up:
		{

			break;
		}
	case Conveyor_Down:
		{

			break;
		}
	case Conveyor_Left:
		{

			break;
		}
	case Conveyor_Right:
		{

			break;
		}
	default:
		break;
	}
}
void CTrap::Render(void)
{
	CCamera* GameCamera = CCamera::GetInstance();
	AnimationManager::GetInstance()->Render(GetPosX() - GameCamera->GetPosX(), GetPosY() - GameCamera->GetPosY(), IsFlipped(), *GetAnimInfo());
}

/*virtual*/ void CTrap::HandleActiveRectCollision( const IEntity* pOther)
{
	switch(pOther->GetType())
	{
	case ENT_PLAYER:
		{
			switch (this->GetTrapType())
			{
			case CTrap::Pit_Fall:
			{
				int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(GetAnimInfo())->GetDamage();
				CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)pOther, (void*)0);
				CSGD_EventSystem::GetInstance()->SendEventNow("CheckPoint", (void*)0, (void*)pOther, (void*)0);
				break;
			}
			case CTrap::Conveyor_Down:
			{
				float ammount = 3.0;
				CSGD_EventSystem::GetInstance()->SendEventNow("ChangeVelY", &ammount, (void*)pOther, (void*)0);
				break;
			}
			case CTrap::Conveyor_Up:
			{
				float ammount = -3.0f;
				CSGD_EventSystem::GetInstance()->SendEventNow("ChangeVelY", &ammount, (void*)pOther, (void*)0);
				break;
			}
			case CTrap::Conveyor_Left:
			{
				float ammount = -3.0f;
				CSGD_EventSystem::GetInstance()->SendEventNow("ChangeVelX", &ammount, (void*)pOther, (void*)0);
				break;
			}
			case CTrap::Conveyor_Right:
			{
				float ammount = 3.0f;
				CSGD_EventSystem::GetInstance()->SendEventNow("ChangeVelX", &ammount, (void*)pOther, (void*)0);
				break;
			}
			case CTrap::Wrecking_Ball:
			{
				if(m_fTime <=0)
				{
					m_fTime = 4;
					//create wreckingBall
					CCreate_Wrecking_Ball_Message* c_pProjectMSG = new CCreate_Wrecking_Ball_Message(0, true);
					c_pProjectMSG->SetPosX(this->GetPosX() + 40);
					c_pProjectMSG->SetPosY(this->GetPosY());
					CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
				}
				break;
			}
			case CTrap::Rock_Slide:
			{
				if(m_fTime <=0)
				{
					m_fTime = 15;
					//create Rock Fall
					CCreate_Rock_Fall_Message* c_pProjectMSG = new CCreate_Rock_Fall_Message(0, true);
					c_pProjectMSG->SetPosX(this->GetPosX());
					c_pProjectMSG->SetPosY(this->GetPosY());
					CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
				}
				break;
			}

			default:
				break;

			}
		break;
		}
	case ENT_ENEMY:
		{
			RECT rOverlap = { };
			const CEnemy* Enemy = dynamic_cast< const CEnemy* >( pOther );
			switch (GetTrapType())
			{
			case CTrap::Pit_Fall:
				{
					if(Enemy->GetState()->GetState() != Enemy->GetState()->PUSHED)
					{
						RECT rEntity1 = pOther->GetCollisionRect();
						RECT rEntity2 = this->GetActiveRect().GetRECT();
						IntersectRect(&rOverlap, &rEntity2, &rEntity1 );
						CSGD_EventSystem::GetInstance()->SendEventNow("Dodge_Pitfall", &rOverlap, (void*)pOther, this);
					}
					else
						CSGD_EventSystem::GetInstance()->SendEventNow("Kill_Self", (void*)0, (void*)pOther);
				}
				break;

			default:
				break;
			}
			break;
		}
	}
}