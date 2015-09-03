/*
	File:		AnimationManager.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	AnimationManager contains all of the animations in game and 
				handles rendering those animations
*/

#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <map>

#include <vector>
#include <string>
#include "../Util/Rect.h"

class CAnimation;
class CAnimInfo;
class CFrame;
class CPlayer;
class CEntity;

struct Rect;

class AnimationManager
{

public:
	// Singleton
	static AnimationManager* GetInstance(void);

	void UnloadAnimations();

	// Accesor
	std::map<std::string, CAnimation>& GetAnimations() { return m_mAnimations;}

	void Render(float posX, float posY, bool bFlipped, CAnimInfo& anim, DWORD dwcolor = 0xFFFFFFFF);

	void Update(float fElapsedTime, CAnimInfo& anim);

	void TestUpdate(float fElapsedTime, CAnimInfo& info);

	bool LoadAnimationFile(const char* szFileName);

	bool LoadCharacterAnimations(const CEntity*  pEntity);

	bool CreateAnimInfo(std::string AnimName, CAnimInfo& Info);

	std::string CheckTrigger(CAnimInfo& info);

	bool CheckAnimationQueue(CAnimInfo& info);

	Rect GetSourceRect(CAnimInfo* info);
	Rect GetCollisionRect(CAnimInfo*);
	Rect GetActiveRect(CAnimInfo*);

	CFrame* GetCurrentFrame(CAnimInfo*);

	float GetAnimationDuration(std::string AnimName);

private:
	std::map<std::string, CAnimation> m_mAnimations;

	bool	m_bLoadedAnimations[7];

	// Singleton!
	AnimationManager(void);
	~AnimationManager(void);
	AnimationManager (const AnimationManager&);
	AnimationManager& operator=(const AnimationManager&);
};


#endif
