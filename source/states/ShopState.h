#pragma once
#include "IGamestate.h"
#include "../CSGD/IListener.h"
class CItemManager;
class CShopState : public IGameState, public IListener
{
public:

	enum CURSOR {HEALTH, SPECIAL, LIVES, FIRE, WIND, ICE, EARTH, EXIT};

	static CShopState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
	void		 HandleEvent( CEvent* pEvent );

private:

	CShopState						( void );
	CShopState						( const CShopState& );
	virtual         ~CShopState		( void );
	CShopState& operator=			( const CShopState& );

	int			m_nSelect		 ;
	int			m_nSubSelect	 ;
	int			m_nCursorX		 ;
	int			m_nCursorY		 ;	
	int			m_nSubPage		 ;
	int			m_nItemID		 ;
	float	    m_fBuy			 ;
	int			m_nMenuArtid	 ;
	int			m_nScrollImgid   ;

	CItemManager*   m_pIM;


};

