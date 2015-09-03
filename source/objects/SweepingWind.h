#pragma once


#include "special.h"
#include "../Util/SLList.h"

class CHuman;

class CSweepingWind : public CSpecial
{
public:
	CSweepingWind(void);
	virtual ~CSweepingWind(void);

	virtual void Update(float fElapsedTime) override;
	virtual void Render(void)				override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;

};

