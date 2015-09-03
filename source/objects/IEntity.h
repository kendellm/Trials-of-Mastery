#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct Rect;

class IEntity
{
public:

	virtual ~IEntity(void)=0{}

	virtual void Update( float fElapsedTime )	= 0;
	virtual void Render( void )					= 0;

	virtual int  GetType( void ) const			= 0;
	virtual RECT GetCollisionRect( void ) const					= 0;
	virtual Rect GetActiveRect( void ) const					= 0;
	virtual const char* GetAnimations(const char* name) const	= 0;

	virtual void HandleCollision( const IEntity* pOther, const  RECT rOverlap )	= 0;
	virtual void HandleActiveRectCollision ( const IEntity* pOther ) = 0;
	virtual void ModifyHealth(int damage)					= 0;

	friend bool CheckCollision( const IEntity* pEntity1, const IEntity* pEntity2, RECT& rOverlap );
	friend bool CheckActiveRectCollision( const IEntity* pEntity1, const IEntity* pEntity2, RECT& rOverlap );

	virtual void AddRef ( void )				= 0;
	virtual void Release( void )				= 0;
};

