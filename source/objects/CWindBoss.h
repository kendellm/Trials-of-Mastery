#pragma once
#include "enemy.h"
#include "../CSGD/IListener.h"
#include <vector>

class CPlayer;
struct Rect;

class CWindBoss : public CEnemy
{
public:
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;

	virtual void HandleEvent(CEvent* pEvent)			override;


	CWindBoss(void);
	~CWindBoss(void);

private:
	struct WayPoint
	{
		float x;
		float y;
	};

	DWORD m_dwRenderColor;

	//vector of waypoints for enemy to travel to 
	vector<WayPoint> WayPoints;

	//Waypoint Timer
	float WayPoint_Timer;

	//Bool to check if you are currently at a waypoint
	bool inWaypoint;

	//Counter to cycle through Waypoints
	int Waypoint_Counter;

	//Special timer
	float Special_Timer;

	//Waypoint for holding previous target position
	WayPoint Prev_Tar_Pos;


	//Vector of waypoints for frenzy mode
	vector<WayPoint> FrenzyPos;

	//Float for frenzy timer 
	float	FrenzyTimer;

	//Counter for frenzy vector
	int FrenzyCount;
	
	//For projectiles && Punching
	float m_nCoolDown;
	float m_fPunchCoolDown;
	bool m_bPunching;

	bool m_bEvadeUp;
	float m_fPosXOld;
	float m_fMoveAway;
	float m_fPosYOld;
	float m_fUpdateOldPos;

};

