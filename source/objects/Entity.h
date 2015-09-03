
#pragma once
#include "IEntity.h"
#include "../CSGD/IListener.h"
#include <string>
#include "../Util/SLList.h"

#define EntityState CEntityState::ENTITY_STATE
#define HIT_BOXES 0

class CAnimInfo;
class AnimationManager;
class CCamera;
struct Rect;

class CEntity : public IEntity
{
public:

	enum	EntityType		{   
		                        ENT_BASE          , ENT_HUMAN         , ENT_PLAYER     , ENT_ENEMY    , ENT_PROJECTILE,
		                        ENT_TRAP          , ENT_SPECIAL       , ENT_FIREBALL   , ENT_SUBZERO  , ENT_SHOCKWAVE ,
								ENT_SWEEPING_WIND , ENT_WRECKING_BALL , ENT_ROCK_FALL  , ENT_POPUP	  , ENT_BOSS      ,
							};

	CEntity(void);
	virtual ~CEntity(void);



	virtual void Update( float fElapsedTime )	override;
	virtual void Render( void )					override;

	virtual int  GetType( void ) const			override	{	return m_eType;		}
	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;
	RECT GetSourceRect( void ) const;
			
	bool IsOffScreen();	

	virtual void HandleCollision( const IEntity* pOther, const RECT rOverlap );
	virtual void HandleActiveRectCollision ( const IEntity* pOther )	override;
	virtual void HandleEvent( CEvent* pEvent ) = 0;

	virtual void ModifyHealth(int damage);

	virtual void AddRef  ( void )				final;
	virtual void Release ( void )				final;

	float			m_fStartPosX;
	float			m_fStartPosY;

	//Accesors
	float			GetPosX			( void ) const		{	return	m_fPosX;		}
	float			GetPosY			( void ) const		{	return	m_fPosY;		}
	float			GetVelX			( void ) const		{	return	m_fVelX;		}
	float			GetVelY			( void ) const		{	return	m_fVelY;		}
	float			GetRotation		( void ) const		{	return	m_fRotation;	}
	int				GetWidth		( void ) const		{	return	m_nWidth;		}
	int				GetHeight		( void ) const		{	return	m_nHeight;		}
	bool			IsFlipped		( void ) const		{   return  m_bFlipped;		}
	CAnimInfo*		GetAnimInfo		( void ) const		{	return m_pAnimInfo;		}

	SLList<const IEntity*>&	GetHitEnemies	 ( void )			{   return m_lEnemiesHit;	}
	virtual const char *	GetAnimations	 ( const char* name ) const;


	// Mutators				 
	void	SetPosX		 ( float	x  )		{	m_fPosX		= x ;	}
	void	SetPosY		 ( float	y  )		{	m_fPosY		= y ;	}
	void	SetVelX		 ( float	vx )		{	m_fVelX		= vx;	}
	void	SetVelY		 ( float	vy )		{	m_fVelY		= vy;	}
	void	SetRotation	 ( float r  )			{	m_fRotation	= r;	}
	void	SetWidth	 ( int	w )				{	m_nWidth	= w ;	}
	void	SetHeight	 ( int	h )				{	m_nHeight	= h ;	}
	void	SetFlipped	 ( bool b )				{   m_bFlipped  = b;	}
	void	SetIdleAnim  ( char* c)				{   m_szIdleAnim = c;	}
	void	SetPosition  (float x, float y);
	void	SetVelocity  (float vx, float vy);
	void    SetSize		 (int w, int h);
	virtual void SetBaseAnimations( EntityType eType);


protected:

	EntityType			m_eType;

private:

	float			m_fPosX;
	float			m_fPosY;
	float			m_fVelX;
	float			m_fVelY;
	float			m_fRotation;
	int				m_nWidth;
	int				m_nHeight;

	const char*		m_szIdleAnim;

	bool			m_bFlipped;

	unsigned int	m_unRefCount;

	SLList<const IEntity*>		m_lEnemiesHit;

	CAnimInfo*				m_pAnimInfo;
	AnimationManager*		m_pAM;
	CCamera*				m_pGC;

};

