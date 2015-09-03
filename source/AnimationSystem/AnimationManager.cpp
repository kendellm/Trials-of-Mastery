/*
File:		AnimationManager.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	AnimationManager contains all of the animations in game and 
handles rendering those animations
*/


#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../CSGD/CSGD_Direct3D.h"
#include "../Inputs/Controller.h"

#include "AnimationManager.h"
#include "Animation.h"
#include "AnimInfo.h"
#include "Frame.h"
#include "../Util/Rect.h"
#include "../objects/Entity.h"
#include "../objects/Enemy.h"
#include "../objects/Player.h"

#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "../TinyXML/tinyxml.h"






/*static*/ AnimationManager* AnimationManager::GetInstance(void)
{
	static AnimationManager s_Instance;
	return &s_Instance;
}

AnimationManager::AnimationManager(void)
{
	for ( int i = 0; i < 7; ++i)
		m_bLoadedAnimations[i] = false;
}

AnimationManager::~AnimationManager(void)
{
}

void AnimationManager::UnloadAnimations()
{
	//CSGD_TextureManager::GetInstance()->UnloadTexture(m_mAnimations["TestIdle"].GetImgID());

	m_mAnimations.clear();
}

Rect AnimationManager::GetSourceRect(CAnimInfo* info)
{
	return m_mAnimations[info->GetAnimationName()].GetFrame()[info->GetCurrFrame()].GetSourceRect();
}

Rect AnimationManager::GetCollisionRect(CAnimInfo* info)
{

	return m_mAnimations[info->GetAnimationName()].GetFrame()[info->GetCurrFrame()].GetCollisionRect();
}

Rect AnimationManager::GetActiveRect(CAnimInfo* info)
{
	return m_mAnimations[info->GetAnimationName()].GetFrame()[info->GetCurrFrame()].GetActiveRect();
}

CFrame* AnimationManager::GetCurrentFrame(CAnimInfo* info)
{
	return &m_mAnimations[info->GetAnimationName()].GetFrame()[info->GetCurrFrame()];
}

void AnimationManager::Update(float fElapsedTime, CAnimInfo& anim)
{
	// Valdiate the anim info
	if ( dynamic_cast<CEntity*>(anim.GetOwner())->GetAnimations("Idle") == "" )
		return;

	// Increase the Time on the Current Frame
	anim.SetTimeOnFrame(anim.GetTimeOnFrame() + fElapsedTime);

	// Check if its time to switch to the next frame
	if (anim.GetTimeOnFrame() >= m_mAnimations[anim.GetAnimationName()].GetFrame()[anim.GetCurrFrame()].GetDuration())
	{
		// Check the current frame for a trigger before we we switch frames
		string trig_name = m_mAnimations[anim.GetAnimationName()].GetFrame()[anim.GetCurrFrame()].GetTrigger().m_szName;

		if ( trig_name != "-1" && anim.IsTriggerSent() != true)
		{
			if (trig_name == "Sound")
			{
				int nData = m_mAnimations[anim.GetAnimationName()].GetFrame()[anim.GetCurrFrame()].GetTrigger().m_nData;
				int * pData = &nData;

				CSGD_EventSystem::GetInstance()->SendEventNow("Play Sound", (void*)nData);
			}
			else
				CSGD_EventSystem::GetInstance()->SendEventNow(trig_name, GetCurrentFrame(&anim), anim.GetOwner(), nullptr);
			
			anim.SetTriggerSent(true);
		}

		if (anim.AreMultipleHitsAllowed() == true)
		{
			// Allow the player to hit the same enemies thye hit last frame
			anim.GetOwner()->GetHitEnemies().clear();

			// Allow the sound to play every frame
			((CPlayer*)(anim.GetOwner()))->SetSoundPlayed(false);
		}

		anim.SetTimeOnFrame(0.0f);
		anim.SetCurrFrame(anim.GetCurrFrame() + 1);
		anim.SetTriggerSent(false);

		// Are we on the last frame?
		if (anim.GetCurrFrame() == m_mAnimations[anim.GetAnimationName()].GetMaxFrames())
		{
			// Repeat the animation?
			if (m_mAnimations[anim.GetAnimationName()].IsLooping() == true)
				anim.SetCurrFrame(0);
			else
			{
				// Stop on a good frame
				anim.SetCurrFrame(m_mAnimations[anim.GetAnimationName()].GetMaxFrames() - 1);

				// Check if there is a queued up animation
				if (CheckAnimationQueue(anim))
				{
					anim.SetAnimationName(anim.DequeueAnimation());
				}
				else
				{
					if (anim.GetOwner()->GetType() == CEntity::ENT_PLAYER && CController::GetInstance()->MoveKeysDown() == true)					
						anim.SetAnimationName(anim.GetOwner()->GetAnimations("Walk"));					
					else if (anim.GetOwner()->GetType() != CEntity::ENT_PROJECTILE)
						anim.SetAnimationName(anim.GetOwner()->GetAnimations("Idle"));			
				}

				// Clear the list of enemies hit this frame
				anim.GetOwner()->GetHitEnemies().clear();

				// Set the anim info's allow multiple hits bool to false for the next animation
				anim.DisableMultipleHits();

				// Allow the player to play a sound next animation
				if (anim.GetOwner()->GetType() == CEntity::ENT_PLAYER)
						dynamic_cast<CPlayer*>(anim.GetOwner())->SetSoundPlayed(false);
			}
		}
	}
}

void AnimationManager::Render(float posX, float posY, bool bFlipped, CAnimInfo& anim, DWORD dwcolor)
{
	// Get the active frame
	CFrame frame = m_mAnimations[anim.GetAnimationName()].GetFrame()[anim.GetCurrFrame()];

	if (bFlipped)
	{		
		CSGD_TextureManager::GetInstance()->Draw(
			m_mAnimations[anim.GetAnimationName()].GetImgID(), (int)( posX - frame.GetAnchorPointX() * -1),
			(int)( posY - frame.GetAnchorPointY()), 
			-1.0f, 1.0f, &frame.GetSourceRect().GetRECT(),
			0.0f, 0.0f, 0.0f, dwcolor);
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(
			m_mAnimations[anim.GetAnimationName()].GetImgID(), (int)( posX - frame.GetAnchorPointX() ),
			(int)( posY - frame.GetAnchorPointY() ), 
			1.0f, 1.0f, &frame.GetSourceRect().GetRECT(),
			0.0f, 0.0f, 0.0f, dwcolor);
	}	

}

bool AnimationManager::LoadAnimationFile(const char* szFilename)
{
	// string = child->Attribute("attributename");

	TiXmlDocument doc;

	if (doc.LoadFile(szFilename) == false)
	{
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();

	if (pRoot == nullptr)
		return false;

	// Load the file path of the animations sprite sheet
	TiXmlElement* pImagePath = pRoot->FirstChildElement("Filename");
	string SpriteSheetPath = pImagePath->Attribute("path");

	TiXmlElement* pAnimation = pImagePath->NextSiblingElement();

	while (pAnimation != nullptr)
	{
		//Load the Current Animations Info
		string AnimName = pAnimation->Attribute("name");
		int MaxFrames;
		pAnimation->Attribute("maxframes", &MaxFrames);
		string looping;
		looping = pAnimation->Attribute("islooping");

		//Convert the text in XML to a bool
		bool IsLooping;
		if (looping == "true")
			IsLooping = true;
		else 
			IsLooping = false;

		// Create an Animation to add frames to
		CAnimation LoadedAnimation(AnimName, MaxFrames, IsLooping);

		//Get a pointer to the first frame in the animation
		TiXmlElement* pFrame = pAnimation->FirstChildElement("Frame");

		//Load the First Frame of the Current Animation
		while (pFrame != nullptr)
		{
			CFrame frame;

			int left, top, right, bottom, apx, apy, damage, trig_data;
			double duration;
			string trigger;


			pFrame->Attribute("damage", &damage);
			frame.SetDamage(damage);

			pFrame->Attribute("duration", &duration);
			frame.SetDuration((float)duration);

			trigger = pFrame->Attribute("trigger");		
			if (trigger == "Sound")
			{
				pFrame->Attribute("data", &trig_data);
				frame.SetTrigger(trigger, trig_data);
			}
			else
				frame.SetTriggerName(trigger);

			TiXmlElement* pAnchorPoint = pFrame->FirstChildElement("AnchorPoint");
			pAnchorPoint->Attribute("x", &apx);
			pAnchorPoint->Attribute("y", &apy);

			frame.SetAnchorPointX(apx);
			frame.SetAnchorPointY(apy);

			TiXmlElement* pRect = pAnchorPoint->NextSiblingElement();

			pRect->Attribute("left", &left);
			pRect->Attribute("top", &top);
			pRect->Attribute("right", &right);
			pRect->Attribute("bottom", &bottom);

			frame.SetSourceRect(left, top, right, bottom);

			pRect = pRect->NextSiblingElement();

			pRect->Attribute("left", &left);
			pRect->Attribute("top", &top);
			pRect->Attribute("right", &right);
			pRect->Attribute("bottom", &bottom);

			frame.SetCollisionRect(left, top, right, bottom);

			pRect = pRect->NextSiblingElement();

			if (pRect != nullptr)
			{
				pRect->Attribute("left", &left);
				pRect->Attribute("top", &top);
				pRect->Attribute("right", &right);
				pRect->Attribute("bottom", &bottom);

				frame.SetActiveRect(left, top, right, bottom);

				pFrame = pFrame->NextSiblingElement();
			}
			else
			{
				pFrame = pFrame->NextSiblingElement();
			}

			// Add the frame to the currently loading animation
			LoadedAnimation.AddFrame(frame);
		}

		// Convert the file path to a TCHAR*
		wostringstream wos;
		wos << "resource/Animations/" << SpriteSheetPath.c_str();

		// Create the Animation's ImgID
		LoadedAnimation.SetImgID(CSGD_TextureManager::GetInstance()->LoadTexture(wos.str().c_str()));

		// Add the animation with all its frames to the AnimationManager's map class
		m_mAnimations.insert(pair<string, CAnimation>(LoadedAnimation.GetName(), LoadedAnimation));

		// Move to the next Animation in the file
		pAnimation = pAnimation->NextSiblingElement();
	}


	return true;
}

bool AnimationManager::LoadCharacterAnimations(const CEntity*  pEntity)
{
	switch(pEntity->GetType())
	{
	case CEntity::ENT_PLAYER:
		{
			if (m_bLoadedAnimations[0] == true)
				return true;

			// Load all of the players animations
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_idle.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_walk_animation.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_dead.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_knocked_down.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_get_up.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_flinch.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_stunned.xml");

			AnimationManager::GetInstance()->LoadAnimationFile("config/MVC_TEST.xml");

			// Load Basic Attacks
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_light_punch.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_medium_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_heavy_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_fireball.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_special.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_subzero.xml");

			// Load Chain Combos
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_double_punch.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_roundhouse.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_double_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_high_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_launching_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_sliding_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_jump_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_straight_kick.xml");

			// Load Combos
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_dashing_punch.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_retreating_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_upperkick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_leg_sweep.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_hurricane_kick.xml");
			AnimationManager::GetInstance()->LoadAnimationFile("config/player_falcon_kick.xml");

			m_bLoadedAnimations[0] = true;
		}
		break;

	case CEntity::ENT_ENEMY:
		{
			const CEnemy* obj =  dynamic_cast<const CEnemy*>(pEntity);

			switch(obj->GetEnemyType())
			{
			case CEnemy::Types::Grunt:
				{
					if (m_bLoadedAnimations[1] == true)
						return true;

					m_bLoadedAnimations[1] = true;
				}
				break;
			case CEnemy::Types::Dummy:
				{
					if (m_bLoadedAnimations[2] == true)
						return true;

					m_bLoadedAnimations[2] = true;
				}
				break;
			case CEnemy::Types::Fighter:
				{
					if (m_bLoadedAnimations[3] == true)
						return true;

					m_bLoadedAnimations[3] = true;
				}
				break;
			case CEnemy::Types::Tank:
				{
					if (m_bLoadedAnimations[4] == true)
						return true;

					m_bLoadedAnimations[4] = true;
				}
				break;
			case CEnemy::Types::EarthBoss:
				{
					if (m_bLoadedAnimations[5] == true)
						return true;

					m_bLoadedAnimations[5] = true;
				}
				break;
			case CEnemy::Types::Thrower:
				{
					if (m_bLoadedAnimations[6] == true)
						return true;


					m_bLoadedAnimations[6] = true;
				}
				break;
			}
		}
		break;
	}

	return false;
}



string AnimationManager::CheckTrigger(CAnimInfo& info)
{
	return m_mAnimations[info.GetAnimationName()].GetFrame()[info.GetCurrFrame()].GetTrigger().m_szName;
}

bool AnimationManager::CheckAnimationQueue(CAnimInfo& info)
{
	if ( info.GetNextAnimation().size() == 0)
		return false;
	else
		return true;
}

float AnimationManager::GetAnimationDuration(std::string AnimName)
{
	return m_mAnimations[AnimName].GetDuration();
}









