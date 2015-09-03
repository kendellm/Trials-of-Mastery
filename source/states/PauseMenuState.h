#pragma once
#include "IGamestate.h"
class CPauseMenuState : public IGameState
{
public:
	enum CURSOR { CONTINUE, COMBO, OPTIONS ,CONTROLS, QUIT};


	static CPauseMenuState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;

private:

	CPauseMenuState						( void );
	CPauseMenuState						( const CPauseMenuState& );
	virtual         ~CPauseMenuState	( void );
	CPauseMenuState&   operator=		( const CPauseMenuState& );

	int			m_nSelect;
	int			m_nCursorX;
	int			m_nCursorY;
	int			m_nSubPage;
	int			m_nMenuArtid;


};