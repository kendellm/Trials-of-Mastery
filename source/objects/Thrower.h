#pragma once
#include "enemy.h"
#include "../CSGD/IListener.h"

class CPlayer;
struct Rect;

class CThrower : public CEnemy
{
public:
	CThrower(float fHealthScale);
	~CThrower(void);

	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;

	virtual void HandleEvent(CEvent* pEvent);


private:

	DWORD m_dwRenderColor;
	float cooldown;
	bool punching;
	bool m_bFlipped;
	float m_nCoolDown;

	bool m_bEvadeUp;
	float m_fPosXOld;
	float m_fMoveAway;
	float m_fPosYOld;
	float m_fUpdateOldPos;


};
