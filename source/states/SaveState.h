#pragma once
#include "IGamestate.h"
#include <vector>

class GameInfo;
class CSaveState :	public IGameState
{
public:
	enum SUB_CURSOR {YES, NO, DEL};
	enum CURSOR {SLOT1, SLOT2, SLOT3, EXIT};


	static CSaveState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;


private:

	CSaveState						( void );
	CSaveState						( const CSaveState& );
	virtual         ~CSaveState		( void );
	CSaveState& operator=			( const CSaveState& );

	int			m_nSelect;
	int			m_nSlot;
	int			m_nCursorX;
	int			m_nCursorY;
	int			m_nSubPage;
	int			m_nMenuArtid;
	int			m_nScrollImgid;


	GameInfo*	m_pGi;
	std::vector<GameInfo*> m_vSlots;

	void		 RefreshSlots  (void);

};
