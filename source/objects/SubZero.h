#pragma once
#include "special.h"
class CSubZero : public CSpecial
{
public:


	CSubZero(int lvl);
	virtual ~CSubZero(void);

	virtual void Update(float fElapsedTime) override;
	virtual void Render(void)				override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;

};

