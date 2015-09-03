#pragma once
#include "IGamestate.h"
class CGameOverState :public IGameState
{
public:
	enum CURSOR { EXIT};

	static CGameOverState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
	void SetWin(bool b) {m_bWin = b;}
	void SetLevelFinished(int i) {m_nLevel_Finished = i;}

private:

	CGameOverState						( void );
	CGameOverState						( const CGameOverState& );
	virtual         ~CGameOverState		( void );
	CGameOverState& operator=			( const CGameOverState& );

	int			m_nSelect;
	int			m_nCursorImg; 
	int			m_nCursorX;
	int			m_nCursorY;
	int			m_nWindBGid;
	int			m_nLoseBGid;
	int			m_nLevel_Finished;
	
	bool		m_bWin;


};


