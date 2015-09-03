/*
	File:		Animation.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CAnimation contains the look up info for an animation
*/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>

class CFrame;

class CAnimation
{
	std::string		m_szName;

	int				m_nMaxFrames;
	int				m_nImageID;

	float			m_fDuration;

	bool			m_bLooping;

	std::vector<CFrame> m_vFrames;
	

public:
	CAnimation();
	CAnimation(std::string szName, int nMaxFrames, bool bLooping);
	~CAnimation(void);

	void AddFrame(CFrame& frame);

	// Accessors
	int						GetMaxFrames	(void)	const	{ return m_nMaxFrames;	}
	int						GetImgID		(void)	const	{ return m_nImageID;	}
	bool					IsLooping		(void)	const	{ return m_bLooping;	}
	float					GetDuration		(void)			{ return m_fDuration;	}
	std::string				GetName			(void)	const	{ return m_szName;		}
	std::vector<CFrame>&	GetFrame		(void)			{ return m_vFrames;		}
		
	//Mutators
	void SetImgID(int nImgID) { m_nImageID = nImgID; }
	


};

#endif