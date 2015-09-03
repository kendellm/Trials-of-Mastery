/*
File:		EntityState.h
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1308
Purpose:	CEntityState class handles the switch of state of
entities in the game.
*/

class CHuman;

#ifndef ENTITY_STATE_H
#define ENTITY_STATE_H

class CEntityState
{
public:

	enum ENTITY_STATE {  IDLE, FLINCHING, KNOCKED_DOWN, DEAD, FROZEN, STUNNED, SUPER, PUSHED };

	CEntityState(CHuman* pOwner);
	~CEntityState(void);

	// Interface
	void Update(float fElapsedTime);
	void SetState(ENTITY_STATE eState);


	// Accesors
	ENTITY_STATE	GetState(void) const	{ return m_eState;	}
	bool			GetTimer(void) const	{ return m_fTimer > 0.0f; }



private:

	float		m_fPrevTimer;
	float		m_fTimer;
	float       m_fElaspedTime;
	CHuman*		m_pOwner;

	ENTITY_STATE	m_eState;

	};

#endif