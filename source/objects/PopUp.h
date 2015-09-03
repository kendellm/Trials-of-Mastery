#pragma once

#include  "Entity.h"
#include "../CSGD/IListener.h"

class CPopUp : public CEntity, IListener
{
public:
	enum PopUp_Type { DASHING_PUNCH, SP_GAIN, FIREBALL_SPECIAL, RETREATING_KICK, TRAPS };

	CPopUp(PopUp_Type nType);
	virtual ~CPopUp(void);

	int		Type   ( void ) const { return m_nType	     ;}
	void	Type  	( 	int		ntype	 )  { m_nType	 = ntype	;} 


	bool    Active  ( void ) const { return m_bActive     ;}
	void    Active  (   bool    bActive  )  { m_bActive  = bActive  ;}

	void    Timer		(float nTime)  {m_fTimer = nTime;}
	float   Timer     ( void ) const	{return m_fTimer;}


	virtual void Update(float fElapsedTime) ;
	virtual void Render(void);

	virtual void HandleEvent(CEvent* pEvent)			override;


private:

	int			m_nType		;
	float		m_fTimer	;
	bool		m_bActive   ;
	int			m_nBackgroundImgid;

protected:

	DWORD		m_dwRenderColor;


};
