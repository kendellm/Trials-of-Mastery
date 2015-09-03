/*
	File:		Human.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CHuman dervies from CEntity and functionality neccessary for
				playerable characters and enemies, but not other CEntities.
*/

#ifndef HUMAN_H
#define HUMAN_H

#include "entity.h"



class CHuman : public CEntity
{

public:
	CHuman(void);
	virtual ~CHuman(void);

	virtual void Update( float fElapsedTime )	override;
	virtual void Render( void )					override;

	virtual void HandleCollision( const IEntity* pOther, const  RECT rOverlap )		override;
	virtual void HandleActiveRectCollision ( const IEntity* pOther )		override;

	// Accessors
	DWORD			GetRenderColor	( void ) const		{   return m_dwRenderColor; }
	virtual const char*	 GetAnimations	( const char* name ) const;		

	// Mutators
	void	SetBaseAnimations(EntityType eType);
	void	SetRenderColor(int a, int r, int g, int b);	 


private:
	DWORD m_dwRenderColor;


	const char*     m_szWalkingAnim;
	const char*		m_szFlinchAnim;
	const char*		m_szFrozenAnim;
	const char*		m_szKnockDownAnim;
	const char*		m_szDeadAnim;
};

#endif