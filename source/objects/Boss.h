/*
File:		Boss.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1308
Purpose:	CBoss class is a base class for all the bosses
*/

#ifndef BOSS_H
#define BOSS_H

#include "enemy.h"
#include <vector>


struct WayPoint
{
	float x;
	float y;
};


class CBoss : public CEnemy
{
public:
	CBoss(void);
	CBoss(float fWayPointTimer, float fSpecialTimer, bool bAtWayPoint, int nWayPointCounter, int nHealth = 20);
	virtual ~CBoss(void);

	virtual void Update(float fElapsedTime)				override;
	virtual void Render(void)							override;
	virtual void HandleEvent(CEvent* pEvent)			override;

	// Accessors 
	vector<WayPoint>&	GetWayPoints		( void )		{ return m_vWaypoints;			}
	WayPoint			GetWayPoint			( int value)	{ return m_vWaypoints[value];	}
	float				GetWayPointTimer	( void )		{ return m_fWayPointTimer;		}
	float				GetSpecialTimer		( void )		{ return m_fSpecialTimer;		}
	bool				AtWayPoint			( void )		{ return m_bAtWayPoint;			}
	int					GetWayPointCounter	( void )		{ return m_nWayPointCounter;	}
	const char *		GetSpecialAnim		( void )		{ return m_szSpecialAnim;		}

	// Mutators
	void				SetWayPointTimer	( float value )			{ m_fWayPointTimer = value;		}
	void				SetAtWayPoint		( bool value )			{ m_bAtWayPoint = value;		}
	void				SetWayPointCounter  ( int value )			{ m_nWayPointCounter = value;	}
	void				SetSpecialTimer		( float value )			{ m_fSpecialTimer = value;		}
	void				SetSpecialAnim		( const char * value)	{ m_szSpecialAnim = value;		}



private:

	//Waypoint for holding previous target position
	WayPoint Prev_Tar_Pos;

	//vector of waypoints for enemy to travel to 
	vector<WayPoint> m_vWaypoints;

	//Waypoint Timer
	float		m_fWayPointTimer;

	//Bool to check if you are currently at a waypoint
	bool		m_bAtWayPoint;

	//Counter to cycle through Waypoints
	int			m_nWayPointCounter;

	//Special timer
	float m_fSpecialTimer;

	// Special Animation Name
	const char * m_szSpecialAnim;

	// Attacking 
	float		m_nCoolDown;
	float		m_fPunchCoolDown;
	bool		m_bPunching;
	bool		m_bInAttackRange;

	// Pathfinding
	bool		m_bEvadeUp;
	float		m_fPosXOld;
	float		m_fMoveAway;
	float		m_fPosYOld;
	float		m_fUpdateOldPos;
};


#endif
