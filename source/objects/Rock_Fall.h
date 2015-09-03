#pragma once
#include "Projectile.h"

class CRock_Fall: public CProjectile
{
public:
	CRock_Fall(void);
	~CRock_Fall(void);

	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;
};

