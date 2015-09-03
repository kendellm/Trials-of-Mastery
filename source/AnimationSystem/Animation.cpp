/*
	File:		Animation.cpp
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CAnimation contains the look up info for an animation
*/

#include "Animation.h"
#include "Frame.h"


CAnimation::CAnimation()
{
	m_szName		= "";

	m_nMaxFrames	= -1;
	m_nImageID		= -1;

	m_fDuration		= 0.0f;

	m_bLooping		= false;
}

CAnimation::CAnimation(std::string szName, int nMaxFrames, bool bLooping)
{
	m_szName = szName;
	m_nMaxFrames = nMaxFrames;
	m_bLooping = bLooping;

	m_fDuration		= 0.0f;

	// Set the image id to a safe value
	m_nImageID = -1;
}


CAnimation::~CAnimation(void)
{
}

void CAnimation::AddFrame(CFrame& frame)
{
	m_vFrames.push_back(frame);

	m_fDuration += frame.GetDuration();
}

