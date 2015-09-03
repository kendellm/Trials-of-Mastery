#pragma once
#include "entity.h"
#include "../CSGD/IListener.h"


class CProjectile : public CEntity, public IListener
{
private:
	int Owner_Type;
	bool m_bFlipped;
	float m_nSpeed;
	int Project_Type;

	//Bool for multiple hit projectiles
	bool isSpecial;

public:
	CProjectile(void);
	CProjectile(string szName, float fSpeed, bool bFlipped);
	CProjectile(string anim, string name, float speed, bool s = true);
	virtual ~CProjectile(void);

	virtual void Update(float fElapsedTime);
	virtual void Render(void);

	virtual RECT GetCollisionRect	( void ) const			override;
	virtual Rect GetActiveRect		( void ) const			override;
	virtual void HandleEvent(CEvent* pEvent)			override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) override;

	void SetProject_Type(int p){ Project_Type = p;}
	int GetProject_Type(){return Project_Type;}

	void SetSpeed(float s){m_nSpeed = s;}
	float GetSpeed(){return m_nSpeed;}

	int GetOwnerType() const {return Owner_Type;}
	void SetOwnerType(int t){Owner_Type = t;}

	bool GetFlipped()	{return m_bFlipped;}
	void SetFlipped(bool b){ m_bFlipped = b;}


	enum Projectile_Type {PROJECTILE_THROWER, PROJECTILE_FIREBALL, PROJECTILE_SHOCKWAVE, PROJECTILE_SWEEPING_WIND, PROJECTILE_SUB_ZERO};
};

