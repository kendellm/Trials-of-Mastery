#pragma once

#include "entity.h"

#include "../CSGD/IListener.h"
class CSpecial : public CEntity, public IListener
{
	enum SKILL { FIREBALL, SUBZERO, SWEEPING_WIND, SHOCKWAVE};

public:
	CSpecial(void);
	virtual ~CSpecial(void);


	virtual int   GetOwnerType() const {return Owner_Type;}
	virtual void  SetOwnerType(int t){Owner_Type = t;}


	virtual int   GetLevel    ( void )	const	  {return m_nLevel;}
	virtual void  SetLevel    (int nlevel )   {m_nLevel = nlevel;}

			
	void  Timer		(float nTime)  {m_fTimer = nTime;}
	float Timer     ( void ) const	{return m_fTimer;}

	float  GetSpeed	(void )	{return m_nSpeed;}
	void   SetSpeed   (float nSpd )	{m_nSpeed = nSpd;}

	virtual void Update(float fElapsedTime) ;
	virtual void Render(void);

	virtual RECT GetCollisionRect( void ) const			override;
	virtual Rect GetActiveRect( void ) const			override;
	virtual void HandleEvent(CEvent* pEvent)			override;
	virtual void SetBaseAnimations( EntityType eType)	override;

private:

	int		m_nSkill;
	int		m_nLevel;
	int		Owner_Type;

	float	m_nSpeed;
	float   m_fTimer;

	const char*     m_szLv1Anim;
	const char*     m_szLv2Anim;
	const char*     m_szLv3Anim;

};

