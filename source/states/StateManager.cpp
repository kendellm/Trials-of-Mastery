#include "StateManager.h"
#include "../CSGD/CSGD_EventSystem.h"

#include "MainMenuState.h"
#include "HomeMenuState.h"
#include "LoadingState.h"
#include "IGameState.h"
#include <iterator>
#include "AudioTestState.h"
StateManager::StateManager(void)
{
	m_pCurrState = nullptr;


}


StateManager::~StateManager(void)
{
	

}

void StateManager::Initialize( void )
{
	ChangeState( CMainMenuState::GetInstance() );
}

void StateManager::Terminate( void )
{
}

void StateManager::RenderAll( void )
{
	for (std::list<IGameState*>::iterator itr = m_lStates.begin(), end = m_lStates.end(); itr != end; ++itr)
	{
		m_pCurrState = *itr;
		if(m_pCurrState->Active())
			m_pCurrState->Render();
	}
}

void StateManager::Update( float fElapsedTime )
{
		m_pCurrState = m_lStates.back();
		m_pCurrState->Update(fElapsedTime );
}

bool StateManager::Input( void )
{
		m_pCurrState = m_lStates.back();
		return m_pCurrState->Input();
}


void StateManager::ChangeState( IGameState* pState )
{
	if( m_pCurrState != nullptr )
		RemoveState(m_pCurrState);

	m_lStates.push_back(pState);
	m_pCurrState = m_lStates.back();

	// Enter the new state (if any)
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();	
}



void StateManager::AddState( IGameState* pState )
{
	if( pState != nullptr )
	{
		m_lStates.push_back(pState);
		m_pCurrState = m_lStates.back();

		// Enter the new state (if any)
		if( m_pCurrState != nullptr )
			m_pCurrState->Enter();
	}
}

void StateManager::RemoveState( IGameState* pState )
{
	if( pState != nullptr )
	{
		pState->Exit();
		m_lStates.pop_back();
	}
}

void StateManager::RemoveAll( void )
{	
	int count = m_lStates.size();
	for(int i = 0; i <count; ++i)
	{
		m_pCurrState = m_lStates.back();
		
		if( m_pCurrState != nullptr )
		{
			m_pCurrState->Exit();
			m_lStates.pop_back();
		}
	}
	
	m_pCurrState = nullptr;
	
}

//clears the stack of states and adds a single state
void StateManager::RevertState(  IGameState* pState )
{
	RemoveAll();
	ChangeState( pState );
}

void StateManager::LoadState( IGameState* pState )
{
	if( m_pCurrState != nullptr )
		RemoveState(m_pCurrState);

	m_lStates.push_back( CLoadingState::GetInstance() );
	m_pCurrState = m_lStates.back();		
	CLoadingState::GetInstance()->SetState(pState);
	
	// Enter the new state (if any)
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();	
}


