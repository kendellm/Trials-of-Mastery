#include "Player.h"

#include "Trap.h"

#include "../Game.h"
#include "../Camera.h"

#include "../CSGD/CSGD_DirectInput.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/RemoveEntityMessage.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CEvent.h"

#include "../Inputs/Controller.h"
#include "../Inputs/Player2Controller.h"
#include "../Inputs/AudioManager.h"
#include "../Util/PopUpManager.h"
#include "../TileSystem/TileManager.h"

#include "../AnimationSystem/AnimationManager.h"
#include "../AnimationSystem/Animation.h"
#include "../AnimationSystem/Frame.h"

#include "../Messages/DropEXPMessage.h"
#include "../Messages/Create_Projectile_Message.h"
#include "../Messages/Perform_SubZero_Special_MSG.h"
#include "../Messages/Perform_Shockwave_Special_MSG.h"
#include "../Messages/Perform_Sweeping_Wind_MSG.h"
#include "../Messages/Perform_Fireball_Special_MSG.h"


#include "../Util/SLList.h"

#include "../objects/Projectile.h"

#include <sstream>

#define CHEAT     0
#define ANIMATION_QUEUE_USER_STORY 0


CPlayer::CPlayer( int HP, int SP, int lives, int exp, int lvl, int fire, int wind, int ice, int earth, bool bIsPlayerOne, bool bPvP )
{
	CEntity::m_eType = ENT_PLAYER;

	m_nLevel		  =	lvl		;
	m_nExpPts		  =	exp		;
	m_nHealth		  =	HP		;
	m_nMaxHealth      =	HP		;
	m_nSpecial		  =	0		;
	m_nMaxSpecial     =	SP		;
	m_nLives		  =	lives	;
	m_nMaxLives       = lives   ;
	m_nFireSpl		  =	fire	;	
	m_nWindSpl		  =	wind	;	
	m_nIceSpl		  =	ice		;
	m_nEarthSpl		  =	earth	;	
	m_nCurrElement    =	0		;

	m_bIsPlayerOne	  = bIsPlayerOne;
	if (bIsPlayerOne == true)

	m_bPvP			  = bPvP;

	m_bPop_Ups.push_back(false);
	m_bPop_Ups.push_back(false);
	m_bPop_Ups.push_back(false);
	m_bPop_Ups.push_back(false);
	m_bPop_Ups.push_back(false);



	m_nfireImg			= -1;
	m_nWindImg			= -1;
	m_nIceImg			= -1;
	m_nEarthImg			= -1;

	m_fCheckPointX		= -1;
	m_fCheckPointY		= -1;

	m_nKey1ImgID		= -1;
	m_nKey2ImgID		= -1;
	m_nKey3ImgID		= -1;
	m_nSuccessImgID		= -1;
	m_nFailImgID		= -1;	

	m_nSpeed	= 175;

	m_nCurrComboDepth = 0;

	m_fPrevInputTime	= 0.0f;
	m_fInputTimer		= 0.0f;
	m_fSuccessTimer		= 0.0f;
	m_fFailTimer		= 0.0f;
	m_fDashTimer		= 0.0f;
	m_fRetreatTimer		= 0.0f;


	m_pAudio			= CAudioManager::GetInstance();

	if (m_bIsPlayerOne == true)
		m_pCtrl = CController::GetInstance();
	else
		m_pCtrl =  CPlayer2Controller::GetInstance();

	SetFlipped(true);

	m_bFailedCombo		= false;
	m_bSuccessfulCombo  = false;

	m_bWasJoystickDown	= false;
	m_bIsJoystickDown	= false;

	m_bSoundPlayed		= false;


	m_pCM = new ComboManager();
	m_pEState = new CEntityState(this);

	// Have the animation manager load my animations
	AnimationManager::GetInstance()->LoadCharacterAnimations(this);

	// Set the player's starting animation to "idle"
	GetAnimInfo()->SetAnimationName("Player_Idle");

	SetBaseAnimations(ENT_PLAYER);


	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	m_nKey1ImgID = pTM->LoadTexture(_T("resource/key1.png"));
	m_nKey2ImgID = pTM->LoadTexture(_T("resource/key2.png"));
	m_nKey3ImgID = pTM->LoadTexture(_T("resource/key3.png"));
	m_nFailImgID = pTM->LoadTexture(_T("resource/Fail_Bitmap.png"));
	m_nSuccessImgID = pTM->LoadTexture(_T("resource/Success_Bitmap.png"));
	AnimationManager::GetInstance()->LoadAnimationFile("config/Wind_Special.xml");

	CSGD_EventSystem::GetInstance()->RegisterClient("ModifyHealth", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Input", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Hit_By_Fireball", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Respawn", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("CheckPoint", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ChangeVelY", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("ChangeVelX", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Dashing_Punch", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Retreat_Kick", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Set_State", this);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # 6 / 56
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	CSGD_EventSystem::GetInstance()->RegisterClient("Game Paused", this);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # 6 / 56
	//////////////////////////////////////////////////////////////////////////


	if(m_nFireSpl)  m_vElements.push_back(0);
	if(m_nWindSpl)  m_vElements.push_back(1);
	if(m_nIceSpl)   m_vElements.push_back(2);
	if(m_nEarthSpl) m_vElements.push_back(3);
}

CPlayer::~CPlayer(void)
{
	SAFE_DELETE(m_pCM);

	SAFE_DELETE(m_pEState);

	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}

void CPlayer::ReturnToCheckPoint()
{
	this->SetPosX(this->GetCheckPointX());
	this->SetPosY(this->GetCheckPointY());
}

void CPlayer::SetCheckPoint( float x, float y )
{
	SetCheckPointX(x);
	SetCheckPointY(y);
}

void CPlayer::SetComboTimers  (float fSuccessTime, float fPrevInputTime, float fInputTimer, float fFailTimer)
{
	m_fSuccessTimer		= fSuccessTime;
	m_fPrevInputTime	= fPrevInputTime;
	m_fInputTimer		= fInputTimer;
	m_fFailTimer		= fFailTimer;
}

void CPlayer::ModifyHealth( int damage )
{
	m_pEState->SetState(EntityState::FLINCHING);

	m_nHealth = max( m_nHealth - damage, 0);

	if(m_nHealth == 0)
		m_pEState->SetState(EntityState::DEAD);
}

void CPlayer::ModifyEXP( int nExp )
{
	m_nExpPts = min( m_nExpPts + nExp, 9999 );
}

void CPlayer::ModifySP( int nSpl )
{
	m_nSpecial = min(m_nSpecial + nSpl, m_nMaxSpecial);
}

void CPlayer::SetState( CEntityState::ENTITY_STATE c )
{
	m_pEState->SetState( c );
}

CEntityState* CPlayer::GetState(void)
{
	return m_pEState;
}


// Update
// - Updates the Player
void CPlayer::Update( float fElapsedTime )
{
	m_pEState->Update(fElapsedTime);
	// Update Image Timers
	if (m_fFailTimer >= 0.0f)
		m_fFailTimer -= fElapsedTime;

	if (m_fSuccessTimer >= 0.0f)
		m_fSuccessTimer -= fElapsedTime;

	if(m_bPop_Ups[0] == true && GetAnimInfo()->GetAnimationName() == "Player_Dashing_Punch")
	{
		m_bPop_Ups[0] = false;
		CSGD_EventSystem::GetInstance()->SendEventNow("Pop_Up_Completed");
	}
	if(m_bPop_Ups[1] == true && m_nSpecial >= 30)
	{
		m_bPop_Ups[1] = false;
		CSGD_EventSystem::GetInstance()->SendEventNow("Pop_Up_Completed");
	}  


#if CHEAT

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_F4) == true)
	{
		//designed to test hud updates
		ModifyHealth(10);
		ModifySP(-5);

	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_F5) == true)
	{
		//Gain benefits
		m_nHealth = 100;
		m_nSpecial = 50;


		CDropEXPMessage* free_money = new CDropEXPMessage(100);
		CSGD_MessageSystem::GetInstance()->SendMsg(free_money);
		free_money = nullptr;
	}

	if ( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_J) )
	{
		GetAnimInfo()->SetAnimationName("Player_Stunned");
	}

	if ( CSGD_DirectInput::GetInstance()->KeyPressed(DIK_H) )
	{
		GetAnimInfo()->SetAnimationName(GetAnimations("Knocked Down"));
	}


	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_F8)==true)
	{
		CPopUpManager::GetInstance()->Load(1);
	}

#endif

	// Update the joystick bools
	m_bWasJoystickDown = m_bIsJoystickDown;
	m_bIsJoystickDown = m_pCtrl->IsJoystickDown();

	// If the player is not in an attack animation allow Input
	if ( ((GetAnimInfo()->GetAnimationName() == GetAnimations("Idle") || GetAnimInfo()->GetAnimationName() == GetAnimations("Walk"))
		&& m_pEState->GetTimer() == false) ) 
	{ 
		m_bSuccessfulCombo = false;
		m_bFailedCombo = false;

		P1MovementInput(m_pCtrl, fElapsedTime);

		// Make sure the player doesn't go off screen
		HandleMovement();

	}
	else if ( m_fDashTimer > 0.0f )
	{
		m_fDashTimer -= fElapsedTime;

		// Dash to the left
		if (IsFlipped() == true)
			SetVelX(m_nSpeed * fElapsedTime * 4);
		else // Dash to the right
			SetVelX(-m_nSpeed * fElapsedTime * 4);

		// Make sure the player doesn't go off screen
		HandleMovement();

	}
	else if (m_fRetreatTimer > 0.0f)
	{
		m_fRetreatTimer -= fElapsedTime;

		// Backflip to the right
		if (IsFlipped() == true)
			SetVelX(-m_nSpeed * fElapsedTime * 2);
		else // Backflip to the left
			SetVelX(m_nSpeed * fElapsedTime * 2);

		// Make sure the player doesn't go off screen
		HandleMovement();
	}

	// So long as the player has correct input. Allow them to chain attacks
	if (m_bFailedCombo != true && m_bSuccessfulCombo != true && m_pEState->GetTimer() != true)
	{
		P1ComboInput(m_pCtrl);
	}

	// If the player is Walking or Idle allow them to move, Otherwise keep them in the same place
	if(GetAnimInfo()->GetAnimationName() == "Player_Walk" || GetAnimInfo()->GetAnimationName() == "Player_Idle")
	{

	}
	else
	{
		SetVelX(0.0f);
		SetVelY(0.0f);	
	}

	// Check for Input to use the Player's Special Abilites
	P1SpecialInput(m_pCtrl);


	// Set the Previous Timer to the Current Timer
	m_fPrevInputTime = m_fInputTimer;

	// Update the Current Timer
	if (m_fInputTimer > 0.0f)
		m_fInputTimer -= fElapsedTime;

	// The current atttack animation just ended
	if (m_fInputTimer <= 0.0f && m_fPrevInputTime > 0.0f && m_bSuccessfulCombo == false)
	{
		m_nCurrComboDepth = 0;
		m_pCM->ResetPossibleCombos();
		ButtonPresses.clear();
		m_fFailTimer = 1.0f;
	}

	AnimationManager::GetInstance()->Update(fElapsedTime, *GetAnimInfo());

	TileManager::GetInstance()->CheckTiles(this);
	if (m_pEState->GetTimer())
		m_pEState->CEntityState::Update(fElapsedTime);

}

void CPlayer::Render( void )
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();

	//if (m_fFailTimer > 0.0f)
	//	pTM->Draw(m_nFailImgID, 468, 500);

	//if (m_fSuccessTimer > 0.0f)
	//	pTM->Draw(m_nSuccessImgID, 445, 500);

	for (unsigned int i = 0; i < ButtonPresses.size(); ++i)
	{
		pTM->Draw(ButtonPresses[i], 200 + 70 * i, 525);
	}

	//Gets the camera so that the rendering can be done in camera window.
	CCamera* Game_Camera = CCamera::GetInstance();

	//Render the Animations
	if (m_bIsPlayerOne == true)
		AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), GetRenderColor());
	else
		AnimationManager::GetInstance()->Render(GetPosX() - Game_Camera->GetPosX(), GetPosY() - Game_Camera->GetPosY(), IsFlipped(), *GetAnimInfo(), D3DCOLOR_ARGB(255, 255, 225, 225));

#if HIT_BOXES

	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetCollisionRect(), D3DCOLOR_ARGB(255, 0, 255, 0), 1); 

	CSGD_Direct3D::GetInstance()->DrawHollowRect(GetActiveRect().GetRECT(),  D3DCOLOR_ARGB(255, 255, 0, 0), 1);

	//	CSGD_Direct3D::GetInstance()->DrawHollowRect(CEntity::GetSourceRect(), D3DCOLOR_ARGB(255, 0, 0, 0), 1);

#endif

}

void CPlayer::HandleCollision( const IEntity* pOther, const RECT rOverlap )
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

/*virtual*/ void CPlayer::HandleActiveRectCollision( const IEntity* pOther )
{
	switch (pOther->GetType())
	{
	case ENT_ENEMY:
		{
			// a boolean to check if an object has already been hit
			bool found = false;

			// Have I already hit this enemy this animation?

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

				// Create an event to deal damage to the enemy
				CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)pOther, this);

				// Add the enemy to the player's enemies hit list
				GetHitEnemies().addHead(pOther);

				// Increase the player's special
				ModifySP(nDamage);

				if (m_bSoundPlayed == false)
				{
					m_pAudio->Play( (SFX)(rand() % 3 + 4));
					m_bSoundPlayed = true;
				}


				// Check if the player hit an enemy with a combo
				string ComboName = GetAnimInfo()->GetAnimationName();
				if (ComboName == "Player_Upperkick" || ComboName == "Player_Leg_Sweep")
					CSGD_EventSystem::GetInstance()->SendEventNow("Set_State", (void*)2, (void*)pOther, this);
				else if (ComboName == "Player_Hurricane_Kick" || ComboName == "Player_Falcon_Kick")
					GetAnimInfo()->AllowMultipleHits();
				/////////////////////////////////////////////////////////////////
				// BUG FIX
				// Reference Bug # 31 / 56
				// BUG FIX START
				/////////////////////////////////////////////////////////////////
				else if (ComboName == "Player_Subzero_lvl1" || ComboName == "Player_Subzero_lvl2" || ComboName == "Player_Subzero_lvl3")
					CSGD_EventSystem::GetInstance()->SendEventNow("Set_State", (void*)4, (void*)pOther, this);
				///////////////////////////////////////////////////////////////////////////
				// BUG FIX END  Reference # 31 / 56
				//////////////////////////////////////////////////////////////////////////


			}
		}
		break;

	case ENT_PROJECTILE:
		{
			const CProjectile* temp = dynamic_cast<const CProjectile*>(pOther);
			if(temp->GetOwnerType() != ENT_PLAYER)
			{
				int nDamage = AnimationManager::GetInstance()->GetCurrentFrame(temp->GetAnimInfo())->GetDamage();
				CSGD_EventSystem::GetInstance()->SendEventNow("ModifyHealth", &nDamage, (void*)this, (void*)temp);
				CSGD_EventSystem::GetInstance()->SendEventNow("PROJECTILE_HIT", (void*)0, (void*)pOther);
			}
		}
		break;


	default:
		{
		}
		break;
	}
}

void CPlayer::HandleEvent( CEvent* pEvent )
{
	if (pEvent->GetDestination() == this)
	{
		if (pEvent->GetEventID() == "ModifyHealth")
			ModifyHealth( *(int*)(pEvent->GetParam()) );
		if (pEvent->GetDestination() == this && pEvent->GetEventID() == "Set_State")
			m_pEState->SetState((CEntityState::ENTITY_STATE)(int)(pEvent->GetParam()) );
		if (pEvent->GetEventID() == "Input")
		{
			CFrame * event_frame = (CFrame*)pEvent->GetParam();
			m_fInputTimer = event_frame->GetDuration();
		}

		if (pEvent->GetEventID() == "Respawn")
			CSGD_EventSystem::GetInstance()->SendEventNow( "PLAYER_DEAD" );

		if (pEvent->GetEventID() == "ChangeVelY")
			SetVelY(GetVelY()+  *(float*)(pEvent->GetParam()) );

		if (pEvent->GetEventID() == "ChangeVelX")
			SetVelX(GetVelX()+  *(float*)(pEvent->GetParam()) );

		if (pEvent->GetEventID() == "CheckPoint")
		{
			SetPosX(GetCheckPointX());
			SetPosY(GetCheckPointY());
		}

		if (pEvent->GetEventID() == "Dashing_Punch")
		{
			m_fDashTimer = 0.35f;
		}
		if (pEvent->GetEventID() == "Retreat_Kick")
		{
			m_fRetreatTimer = 0.5f;
		}
		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # 6 / 56
		// BUG FIX START
		/////////////////////////////////////////////////////////////////
		if (pEvent->GetEventID() == "Game Paused")
		{
			GetAnimInfo()->SetAnimationName(GetAnimations("Idle"));
		}
		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # 6 / 56
		//////////////////////////////////////////////////////////////////////////

	}
	if (m_bPop_Ups[2] == true && pEvent->GetEventID() == "Hit_By_Fireball")
	{
		m_bPop_Ups[2] = false;
		CSGD_EventSystem::GetInstance()->SendEventNow("Pop_Up_Completed");
	}
}

void CPlayer::SpecialAttack( unsigned int nElement )
{

	if((GetAnimInfo()->GetAnimationName() == "Player_Idle" || GetAnimInfo()->GetAnimationName() == "Player_Walk"))
	{
		switch (nElement)
		{
		case FIRE:
			{
				if( m_nSpecial >= 10 * m_nFireSpl && m_nFireSpl )
				{
					GetAnimInfo()->SetAnimationName("Player_Fireball");

					CPerform_Fireball_Special_MSG* c_pProjectMSG = new CPerform_Fireball_Special_MSG(
						this->GetPosX(), this->GetPosY(), this->Fire(), ENT_PLAYER, IsFlipped() );

					CSGD_MessageSystem::GetInstance()->SendMsg(c_pProjectMSG);
					ModifySP(-10 * m_nFireSpl);	
				}
			}
			break;
		case WIND:
			{
				if(m_nSpecial >= 10 * m_nWindSpl && m_nWindSpl)
				{
					GetAnimInfo()->SetAnimationName("Player_Fireball");
					CPerform_Sweeping_Wind_MSG* pushMsg = new CPerform_Sweeping_Wind_MSG(
						this->GetPosX(), this->GetPosY(), this->Earth(), ENT_PLAYER, IsFlipped());
					CSGD_MessageSystem::GetInstance()->SendMsg(pushMsg);

					ModifySP(-10 * m_nWindSpl);
				}
			}
			break;
		case ICE:
			{
				if( m_nSpecial >= 10 * m_nIceSpl && m_nIceSpl )
				{
					/////////////////////////////////////////////////////////////////
					// BUG FIX
					// Reference Bug # 31 / 56
					// BUG FIX START
					/////////////////////////////////////////////////////////////////
					switch (Ice())
					{
					case 1:	GetAnimInfo()->SetAnimationName("Player_Subzero_lvl1");
						break;
					case 2:	GetAnimInfo()->SetAnimationName("Player_Subzero_lvl2");
						break;
					case 3: GetAnimInfo()->SetAnimationName("Player_Subzero_lvl3");
						break;
					}

					//CPerform_SubZero_Special_MSG* freezeMsg = new CPerform_SubZero_Special_MSG(
					//	this->GetPosX(), this->GetPosY(), this->Ice(), ENT_PLAYER);
					//CSGD_MessageSystem::GetInstance()->SendMsg(freezeMsg);


					//CPerform_SubZero_Special_MSG* freezeMsg = new CPerform_SubZero_Special_MSG(
						//this->GetPosX(), this->GetPosY(), this->Ice(), ENT_PLAYER);
					//CSGD_MessageSystem::GetInstance()->SendMsg(freezeMsg);

					ModifySP(-10 * m_nIceSpl);
				}
			}
			break;
		case EARTH:
			{
				if( m_nSpecial >= 10 * m_nEarthSpl && m_nEarthSpl )
				{
					GetAnimInfo()->SetAnimationName("Player_Special");
					CPerform_Shockwave_Special_MSG* shockMsg = new CPerform_Shockwave_Special_MSG(
						this->GetPosX(), this->GetPosY(), this->Earth(), ENT_PLAYER, IsFlipped());
					CSGD_MessageSystem::GetInstance()->SendMsg(shockMsg);

					ModifySP(-10 * m_nEarthSpl);
				}
			}
			break;
		default:
			break;
		}
	}
}

void CPlayer::HandleMovement()
{
	float tempX = GetPosX();
	float tempY = GetPosY();
	bool goDown = true;

	CCamera* GameCamera = CCamera::GetInstance();

	//keep the play one screen.
	while(!(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, 0) ) )
	{
		if(TileManager::GetInstance()->CheckBlockedTiles(tempX, tempY, 0, 0))
		{
			SetPosX(tempX);
			SetPosY(tempY);
		}
		else
		{
			(goDown)? tempY++ : tempY--;
			if(tempY > GameCamera->GetPosY() + GameCamera->GetHeight())
			{
				goDown = false;
			}
			else if (tempY <= 0)
			{
				goDown = true;
			}

		}
	}

	if( GetPosY() >= 600 )
	{
		SetPosY( (GetPosY() - (GetPosY() - 599)) );
	}

	if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), GetVelX(), 0))
	{
		SetPosX(GetPosX() + GetVelX());
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
			SetPosY(GetPosY() + GetVelY());
	}
	else
	{
		if(TileManager::GetInstance()->CheckBlockedTiles(GetPosX(), GetPosY(), 0, GetVelY()))
			SetPosY(GetPosY() + GetVelY());
		if( GetPosY() >= 600 )
		{
			SetPosY( (GetPosY() - (GetPosY() - 599)) );
		}
	}

	SetVelX(0);
	SetVelY(0);

	//Keeps the player on screen durning screen locks
	CCamera* Game_Camera = CCamera::GetInstance();
	if(GetPosX() < Game_Camera->GetPosX())
		SetPosX(Game_Camera->GetPosX());
	else if(GetPosX() > Game_Camera->GetPosX() + Game_Camera->GetWidth())
		SetPosX(Game_Camera->GetPosX() + Game_Camera->GetWidth());
}


/////////////////////////////////////////
// Player One Input Functions
/////////////////////////////////////////
void CPlayer::P1MovementInput(IBaseInput* pCtrl, float fElapsedTime)
{
	if (pCtrl->MoveRight())
	{
		SetVelX(m_nSpeed * fElapsedTime);
		SetFlipped(true);
	}
	if (pCtrl->MoveLeft())
	{
		SetVelX(-m_nSpeed * fElapsedTime);
		SetFlipped(false);
	}
	if (pCtrl->MoveUp())
	{
		SetVelY(-m_nSpeed * fElapsedTime);
	}
	if (pCtrl->MoveDown())
	{
		SetVelY(m_nSpeed * fElapsedTime);
	}
	if (pCtrl->MoveKeysPressed())
	{
		GetAnimInfo()->SetAnimationName(GetAnimations("Walk"));
	}
	if (pCtrl->MoveKeysReleased())
	{
		if (pCtrl->MoveKeysDown() == false)
		{		
			GetAnimInfo()->SetAnimationName(GetAnimations("Idle"));
		}
	}
	if (m_bWasJoystickDown == true && m_bIsJoystickDown == false)
	{
		GetAnimInfo()->SetAnimationName(GetAnimations("Idle"));
	}
}

void CPlayer::P1ComboInput(IBaseInput* pCtrl)
{
	if (pCtrl->LightAtk())
	{	
		if (m_pCM->HandleInput(this, 1, m_nCurrComboDepth, "Player_Light_Punch" ) == true)
			ButtonPresses.push_back(m_nKey1ImgID);
	}
	else if (pCtrl->MedAtk())
	{
		if (m_pCM->HandleInput(this, 2, m_nCurrComboDepth, "Player_Medium_Kick" ) == true)
			ButtonPresses.push_back(m_nKey2ImgID);
	}
	else if (pCtrl->HeavyAtk())
	{
		if (m_pCM->HandleInput(this, 3, m_nCurrComboDepth, "Player_Heavy_Kick") == true)
			ButtonPresses.push_back(m_nKey3ImgID);
	}
}

void CPlayer::P1SpecialInput(IBaseInput* pCtrl)
{
	if( pCtrl->Fireball(false) && Fire() )
	{
			m_nCurrElement = FIRE;
			SpecialAttack(m_nCurrElement);
		
	}

	if( pCtrl->SweepingWind(false) && Wind() )
	{
			m_nCurrElement = WIND;
			SpecialAttack(m_nCurrElement);	
	}

	if(pCtrl->SubZero(false) && Ice())
	{
			m_nCurrElement = ICE;
			SpecialAttack(m_nCurrElement);
	}

	if(pCtrl->ShockWave(false) && Earth() )
	{

			m_nCurrElement = EARTH;
			SpecialAttack(m_nCurrElement);		
	}

	if(pCtrl->SpecAtk())
	{
		SpecialAttack(m_nCurrElement);
	}

	if(pCtrl->Next_Special())
	{
		if((unsigned int)m_nCurrElement + 1 >=  m_vElements.size() )
			m_nCurrElement = 0;
		else
			m_nCurrElement = m_vElements[m_nCurrElement+1];
	}

	if(pCtrl->Prev_Special() )
	{
		if(m_nCurrElement - 1 < 0)
			m_nCurrElement = m_vElements.size()-1;
		else
			m_nCurrElement = m_vElements[m_nCurrElement-1];

	}

}
/////////////////////////////////////////



/////////////////////////////////////////
// Player Two Input Functions
/////////////////////////////////////////
void CPlayer::P2MovementInput(IBaseInput* pCtrl, float fElapsedTime)
{

}

void CPlayer::P2ComboInput(IBaseInput* pCtrl)
{

}

void CPlayer::P2SpecialInput(IBaseInput* pCtrl)
{

}

void CPlayer::SwitchElement( int modElement )
{
	m_nCurrElement = min( m_nCurrElement + modElement, 3);

	if(m_nCurrElement <= -1 )
		m_nCurrElement =3;

}

/////////////////////////////////////////



