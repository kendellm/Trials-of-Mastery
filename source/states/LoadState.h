#pragma once
#include "IGamestate.h"
#include <vector>
class GameInfo;
class CLoadState : public IGameState
{
public:
	enum CURSOR {SLOT1, SLOT2, SLOT3, EXIT};

	static CLoadState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;
	
	void		 Load	(int _slot);
	void		 RefreshSlots  (void);
private:

	CLoadState						( void );
	CLoadState						( const CLoadState& );
	virtual      ~CLoadState		( void );
	CLoadState&  operator=			( const CLoadState& );

	int			m_nSelect;
	int			m_nCursorX;
	int			m_nCursorY;
	int			m_nMenuArtid;
	int			m_nScrollImgid;



	GameInfo*	m_pGi;
	std::vector<GameInfo*> m_vSlots;
};

