#pragma once
#include "IGamestate.h"
class IGameState;
class CLoadingState :public IGameState
{
public:

	static CLoadingState* GetInstance( void );

	virtual	void  Update	(float fElaspedTime) override;
	virtual	void  Render    (void)				 override;
	virtual bool  Input	    (void)				 override;
	virtual void  Enter	    (void)				 override;
	virtual void  Exit	    (void)				 override;
			float Timer     (void)	{return m_fTimer;}
			void  SetState  (IGameState* pState) {m_pState = pState;}
private:

	CLoadingState						( void );
	CLoadingState						( const CLoadingState& );
	virtual         ~CLoadingState		( void );
	CLoadingState&   operator=			( const CLoadingState& );

	int			m_nMenuArtid;
	float		m_fTimer;
	float       m_fPrevTime;

	IGameState*  m_pState;
};
