#pragma once
#include "enemy.h"
#include "../CSGD/IListener.h"
#include <vector>

class CPlayer;
struct Rect;

class CFighter : public CEnemy
{
public:
	
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;

	virtual void HandleActiveRectCollision ( const IEntity* pOther )	override;


	CFighter(float fHealthScale);
	~CFighter(void);

private:
	float cooldown;

	bool m_bEvadeUp;
	bool m_bAttacking;
	bool m_bSecondAttackQueued;

	float m_fPosXOld;
	float m_fPosYOld;
	float m_fMoveAway;
	float m_fUpdateOldPos;

	DWORD m_dwRenderColor;

	std::vector<const char *> m_vAttacks;
};

