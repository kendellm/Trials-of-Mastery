#pragma once
#include "Projectile.h"
class CWrecking_Ball : public CProjectile
{
public:
	CWrecking_Ball(void);
	~CWrecking_Ball(void);

	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;

};

