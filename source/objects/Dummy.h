#pragma once
#include "Enemy.h"
#include "../CSGD/IListener.h"

class CPlayer;
struct Rect;

class CDummy : public CEnemy
{
	public:
	int GetHealth(){return m_nHealth;}
	virtual void ModifyHealth(int damage)	override;
	
	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;

	virtual void HandleActiveRectCollision ( const IEntity* pOther )	override;
	virtual void HandleEvent(CEvent* pEvent)			override;


	CDummy(void);
	~CDummy(void);

private:
	int m_nHealth;

	bool m_bFlipped;



};