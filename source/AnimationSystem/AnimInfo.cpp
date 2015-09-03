/*
File:		AnimInfo.cpp
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CAnimInfo class contains object specific animation info
*/

#include "AnimationManager.h"
#include "AnimInfo.h"
#include "../objects/Entity.h"

#include <string>
#include <map>
using namespace std;


CAnimInfo::CAnimInfo(CEntity* owner)
{
	m_szAnimationName = "";
	m_nCurrFrame = 0;

	m_fTimeOnFrame = 0.0f;

	m_bTriggerSent		= false;
	m_bMultipleHits		= false;

	m_pOwner = owner;
}

CAnimInfo::~CAnimInfo(void)
{
}

void CAnimInfo::SetAnimationName(string anim_name)
{
	m_szAnimationName = anim_name;	
	m_nCurrFrame = 0;
	m_fTimeOnFrame = 0.0f;
	m_bMultipleHits = false;
}

void CAnimInfo::QueueAnimation(std::string value)
{
	if (m_pOwner->GetType() == CEntity::ENT_PLAYER)
	{
		if (m_szAnimationName == m_pOwner->GetAnimations("Idle") || 
			m_szAnimationName == m_pOwner->GetAnimations("Walk")
			)
			SetAnimationName(value);
		else
			m_lNextAnimation.addHead(value);
	}
	else
	{
		if (m_szAnimationName == m_pOwner->GetAnimations("Idle"))
			SetAnimationName(value);
		else
			m_lNextAnimation.addHead(value);
	}

}

std::string CAnimInfo::DequeueAnimation(void)
{
	if (m_lNextAnimation.size() == 0)
		return (string)"";

	else if (m_lNextAnimation.size() == 1)
	{
		SLLIter<std::string> iter(m_lNextAnimation);
		iter.begin();
		string anim_name = iter.current();
		m_lNextAnimation.remove(iter);
		return anim_name;
	}
	else
	{
		SLLIter<std::string> iter(m_lNextAnimation);
		iter.begin();

		unsigned int last_item = 0;

		for (; last_item < m_lNextAnimation.size() - 1; ++last_item)
		{
			++iter;
		}
		string anim_name = iter.current();
		m_lNextAnimation.remove(iter);
		return anim_name;
	}

}



