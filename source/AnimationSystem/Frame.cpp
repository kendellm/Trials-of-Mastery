/*
	File:		Frame.cpp
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CFrame contains the informatino of a frame for an animation
*/

#include "Frame.h"


CFrame::CFrame(void)
{
	m_nAnchorPointX		= -1;
	m_nAnchorPointY		= -1;
	m_nDamage			= -1;
	m_fDuration			= -1;

	trigger.m_nData		= -1;
	trigger.m_szName	= "";
}

CFrame::~CFrame(void)
{
}

void CFrame::SetSourceRect(int left, int top, int right, int bottom)
{
	m_rSourceRect.left = left;
	m_rSourceRect.top = top;
	m_rSourceRect.right = right;
	m_rSourceRect.bottom = bottom;
}

void CFrame::SetCollisionRect(int left, int top, int right , int bottom)
{
	m_rCollisionRect.left = left;
	m_rCollisionRect.top = top;
	m_rCollisionRect.right = right;
	m_rCollisionRect.bottom = bottom;
}

void CFrame::SetActiveRect(int left, int top, int right, int bottom)
{
	m_rActiveRect.left = left;
	m_rActiveRect.top = top;
	m_rActiveRect.right = right;
	m_rActiveRect.bottom = bottom;
}


void CFrame::SetTrigger(std::string trig, int value)
{
	trigger.m_szName = trig;
	trigger.m_nData = value;
}




