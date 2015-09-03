#pragma once

#include "Projectile.h"
#include "../Util/Rect.h"



class CBoss_Special : public CProjectile
{
public:
	CBoss_Special( float x, float y, string anim, string name, float speed );
	~CBoss_Special( void );
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect	( void ) const			override;
	virtual Rect GetActiveRect		( void ) const			override;
	virtual void HandleEvent(CEvent* pEvent)			override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;


private:

};