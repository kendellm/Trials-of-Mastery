/*
	File:		Frame.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CFrame contains the informatino of a frame for an animation
*/

#ifndef FRAME_H
#define FRAME_H

#define WIND32_LEAN_AND_MEAN
#include <Windows.h>

#include "../Util/Rect.h"
#include <string>

class CFrame
{
	struct Trigger
	{
		std::string		m_szName;
		int				m_nData;
	};

	int				m_nAnchorPointX;
	int				m_nAnchorPointY;
	int				m_nDamage;

	float			m_fDuration;	

	Rect			m_rSourceRect;
	Rect			m_rCollisionRect;
	Rect			m_rActiveRect;

	Trigger trigger;

public:
	CFrame(void);
	CFrame(int nAnchorPointX, int nAnchorPointY, int nDamage, 
		float fDuration);
	~CFrame(void);


	//Accessor
	int		GetAnchorPointX()	{ return m_nAnchorPointX;	}
	int		GetAnchorPointY()	{ return m_nAnchorPointY;	}
	int		GetDamage()			{ return m_nDamage;			}
	
	float	GetDuration()		{ return m_fDuration;	}
	
	Rect	GetSourceRect()		{ return m_rSourceRect;		}
	Rect	GetCollisionRect()	{ return m_rCollisionRect;	}
	Rect	GetActiveRect()		{ return m_rActiveRect;		}
	
	Trigger GetTrigger()		{ return trigger;	}


	//Mutator
	void SetAnchorPointX	(int apX)		{ m_nAnchorPointX = apX;	}
	void SetAnchorPointY	(int apY)		{ m_nAnchorPointY = apY;	}
	void SetDamage			(int value)		{ m_nDamage = value;		}
	void SetDuration		(float time)	{ m_fDuration = time;		}

	void SetTriggerName		(std::string trig)	{ trigger.m_szName = trig; }
	void SetTriggerData		(int value)			{ trigger.m_nData = value; }

	void SetTrigger	(std::string trig, int value);

	void SetSourceRect		(int left, int top, int right, int bottom);
	void SetCollisionRect	(int left, int top, int right , int bottom);
	void SetActiveRect		(int left, int top, int right, int bottom);

};

#endif