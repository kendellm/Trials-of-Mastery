#pragma once

#include "special.h"

class CEvent;

class CShockwave : public CSpecial
{
private: 
	int		m_nCurrFrame;
	SLList<const IEntity*> m_lEnemiesHit;

public:
	CShockwave(int lvl);
	virtual ~CShockwave(void);
	virtual void Update(float fElapsedTime) override;
	virtual void Render(void)				override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;


	//SLList<const IEntity*> GetConstHitEnemies() const ;
};

