#pragma once
#include "enemy.h"
#include "../CSGD/IListener.h"

class CPlayer;
struct Rect;

class CTank : public CEnemy
{
public:
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;
	virtual void CTank::HandleCollision(const IEntity* pOther, const RECT rOverlap);
	virtual void CTank::HandleActiveRectCollision ( const IEntity* pOther );

	CTank(float fHealthScale);
	~CTank(void);

private:

	DWORD m_dwRenderColor;

	bool m_bFlipped;

	float Rush_Timer;
	bool  m_bRushing;
	int   m_nRushDirection;
	float RushWait;

	bool m_bEvadeUp;
	float m_fPosXOld;
	float m_fPosYOld;
	float m_fMoveAway;
	float m_fUpdateOldPos;


	//bool for death animation
	bool m_bIsDead;

};

