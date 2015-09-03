#pragma once

#include "boss.h"
#include "../CSGD/IListener.h"
#include <vector>

class CPlayer;
class CCamera;
struct Rect;

class CIceBoss : public CBoss
{
public:

	virtual void Update(float fElapsedTime)				override;

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;

	virtual void HandleEvent(CEvent* pEvent)			override;

	CIceBoss(void);
	~CIceBoss(void);

private:

	enum state { Normal, Frenzy, };


	// Frenzy Waypoints
	vector<WayPoint> CurrentFrenzyPositions;
	vector<WayPoint> RFrenzyPos;
	vector<WayPoint> LFrenzyPos;

	float	ScreenLeft;
	float	ScreenRight;
	int		FrenzyYPos[3];


	// Frenzy counter
	int fpc;
	int iceball_count;
	float frenzy_timer;

	// Current Boss Stage
	int		stage;

	state m_eCurrState;
	
	//For projectiles && Punching
	float		m_fIceBallOffsetX;
	float		m_fIceBallOffsetY;
	float		m_fIceBallSpawnPosX;
	float		m_fIceBallSpawnPosY;


	// Helper Functions
	void CalculateIceBallSpawn(void);
	void SetUpWaypoints(CCamera* cam);


};

