#pragma once

#include "Enemy.h"
#include "../CSGD/IListener.h"

class CPlayer;
struct Rect;

class CEarthBoss : public CEnemy
{
public:
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual void HandleActiveRectCollision ( const IEntity* pOther )	override;
	virtual void HandleEvent(CEvent* pEvent)			override;


	CEarthBoss(void);
	~CEarthBoss(void);

private:
	// Attacking 
	float		m_nCoolDown;
	float		m_fPunchCoolDown;
	bool		m_bPunching;
	float		m_fSpecialTimer;

	// Pathfinding
	bool		m_bEvadeUp;
	float		m_fPosXOld;
	float		m_fMoveAway;
	float		m_fPosYOld;
	float		m_fUpdateOldPos;
};

