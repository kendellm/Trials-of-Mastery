#pragma once
#include "IGameState.h"
#include "../CSGD/IListener.h"
class GameInfo;
class CAudioManager;
class CHomeMenuState : public IGameState, public IListener
{
public:
	enum CURSOR {SHOP, SAVE, EXIT, TUTORIAL, LEVEL1, LEVEL2, LEVEL3, LEVEL4};

	static CHomeMenuState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
		
	void		 HandleEvent( CEvent* pEvent );

private:

	CHomeMenuState						( void );
	CHomeMenuState						( const CHomeMenuState& );
	virtual         ~CHomeMenuState		( void );
	CHomeMenuState& operator=			( const CHomeMenuState& );

	int			m_nSelect;
	int			m_nCursorKim;
	int			m_nCursorX;
	int			m_nCursorY;
	int			m_nLvlImg[5];


	int			m_nMenuArtid;


	CAudioManager*  m_pAudio;
	GameInfo*		m_pGi;
};
