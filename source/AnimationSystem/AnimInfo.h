/*
File:		AnimInfo.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CAnimInfo class contains object specific animation info
*/

#ifndef ANIMINFO_H
#define ANIMINFO_H

#include <string>
#include <vector>
#include "../Util/SLList.h"

class CAnimation;
class CEntity;

class CAnimInfo
{
public:
	CAnimInfo(CEntity* owner);
	~CAnimInfo(void);

	//Accessors
	std::string	GetAnimationName()			{ return m_szAnimationName;	}
	int			GetCurrFrame()				{ return m_nCurrFrame;		}
	float		GetTimeOnFrame()			{ return m_fTimeOnFrame;	}
	bool		IsTriggerSent()				{ return m_bTriggerSent;	}
	bool		AreMultipleHitsAllowed()	{ return m_bMultipleHits;	}
	CEntity*	GetOwner()					{ return m_pOwner;			}
	SLList<std::string>&		 GetNextAnimation()  { return m_lNextAnimation; }

	//Mutators
	void 			SetCurrFrame(int value)				{ m_nCurrFrame = value;		}
	void			SetTimeOnFrame(float value)			{ m_fTimeOnFrame = value;	}
	void 			SetTriggerSent(bool value)			{ m_bTriggerSent = value;	}
	void			AllowMultipleHits(void)				{ m_bMultipleHits = true;	}
	void			DisableMultipleHits(void)			{ m_bMultipleHits = false;	}
	void			SetAnimationName(std::string anim);
	void			QueueAnimation(std::string value);
	std::string 	DequeueAnimation(void);

private:

	std::string		m_szAnimationName;
	int				m_nCurrFrame;
	float			m_fTimeOnFrame;
	bool			m_bTriggerSent;
	bool			m_bMultipleHits;

	CEntity*		m_pOwner;

	SLList<std::string>		m_lNextAnimation;


};

#endif