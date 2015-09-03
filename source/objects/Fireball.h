#pragma once
#include "special.h"
class CFireball : public CSpecial
{
public:


	CFireball(int lvl);
	virtual ~CFireball(void);

	virtual void Update(float fElapsedTime) override;
	virtual void Render(void)				override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;

};

