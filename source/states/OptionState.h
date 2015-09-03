
#pragma once
#include "IGamestate.h"

class GameInfo;
class COptionState : public IGameState
{
public:
	enum CURSOR { WINDOW, LANG ,VOLUME, EXIT, LANG1, LANG2, BGM, SFX, };
	enum PAGE   {MAIN, FULLSCREEN, LANGUAGE, SOUND, BGM_Volume, SFX_volume};

	static COptionState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
	
			void Import (GameInfo* pInfo);

private:

	COptionState						( void );
	COptionState						( const COptionState& );
	virtual         ~COptionState		( void );
	COptionState&   operator=			( const COptionState& );

	int			m_nSelect;

	int			m_nCursorX;
	int			m_nCursorY;

	int			m_nCurrPage;
	int		    m_nSubPage;
	int			m_nMenuArtid;


	GameInfo*	m_pGi;



};

