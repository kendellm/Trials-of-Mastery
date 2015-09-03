#pragma once

#include "Human.h"

class CEntityState;
class CAnimInfo;
class CEntityState;
struct Rect;

class CEnemy : public CHuman, public IListener
{
public:

	enum Types {Grunt, Dummy, Fighter, Tank, EarthBoss, Thrower, FireBoss, IceBoss, WindBoss};

	void SetEnemyType(int type)		{ m_nType = type;	}

	CEnemy(int nHealth = 20);
	virtual ~CEnemy(void);

	virtual void Update(float fElapsedTime)				override;
	virtual void Render(void)							override;
	
	virtual RECT GetCollisionRect	( void ) const			override;
	virtual Rect GetActiveRect		( void ) const			override;

	virtual void HandleCollision( const IEntity* pOther, const RECT rOverlap) override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther )	override;
	virtual void HandleEvent(CEvent* pEvent);

	virtual void ModifyHealth(int damage)				override;
			void ModifyMovement(float fElaspedTime);

	// Accessors
	int				GetHealth		( void )	const	{ return m_nHealth;	}
	CEntity*		GetTarget		( void )	const	{ return m_pTarget;	}
	virtual int		GetExp			( void )    const   { return m_nExpPts; }
	int				GetEnemyType	( void )	const	{ return m_nType;	}
	CEntityState*	GetState		( void )	const	{ return m_pEState;	}

	// Mutators
			void	SetTarget	(CEntity* target);
			void	SetHealth	(int value)			{ m_nHealth = value;	}
	virtual void    SetExp		(int nExp)			{m_nExpPts = nExp;		}


protected:

	int  speed; 
	bool MoveLeft;
	int  m_nExpPts;
	CEntityState*	m_pEState;

	bool m_bOnScreen;

private:

	int m_nType;
	CEntity* m_pTarget;
	int m_nHealth;
	int m_nMaxHealth;
};

