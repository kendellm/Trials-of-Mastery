#pragma once

#include <list>

class IGameState;
class StateManager
{
public:
	StateManager(void);
	~StateManager(void);

	void Initialize ( void );
	void Terminate  ( void );

	void RenderAll	( void );
	void Update  	( float fElapsedTime);
	bool Input		( void );

	void AddState		( IGameState* pState );
	void RemoveState	( IGameState* pState );
	void ChangeState	( IGameState* pState );
	void LoadState		( IGameState* pState );
	void RevertState	( IGameState* pState );
	void RemoveAll		( void );


private:

	IGameState*  m_pCurrState;
	std::list<IGameState*> m_lStates;

};

