#pragma once
#include "enemy.h"
#include "../CSGD/IListener.h"

class CPlayer;
struct Rect;

class CGrunt : public CEnemy
{
public:
	
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;

	CGrunt(float fHealthScale);
	~CGrunt(void);

private:

	float cooldown;
	bool  punching;
	bool  m_bFlipped;

	bool  m_bEvadeUp;
	float m_fPosXOld;
	float m_fPosYOld;
	float m_fMoveAway;
	float m_fUpdateOldPos;

	
};

